<?php

function gti_keyword_search_endpoints_config(){
  $content = '';
  $form = drupal_render(drupal_get_form('gti_keyword_search_create_group_form'));
  $content .= $form;
  return $content;
}

function gti_keyword_search_create_group_form($form, &$form_state) {

  $form['keyword_facet_group_name'] =  array(
    '#type' => 'fieldset',
    '#title' => "",
    '#collapsible' => FALSE,
    '#collapsed' => FALSE
  );
  $form['keyword_facet_group_name']['group_name'] = array(
    '#type' => 'textfield',
    '#title' => 'Group Name',
    '#maxlength' => 1024,
    '#size' => 60,
    '#description' => t(''),
  );
  $form['keyword_facet_group_name']['bundle'] = array(
    '#type' => 'select',
    '#title' => 'Bundles',
    '#multiple' => TRUE,
    '#options' => _get_bundles(),
    '#description' => t(''),

  );
  $form['keyword_facet_group_name']['add_term_button'] = array(
    '#type' => 'submit',
    '#value' => t('+'),
    '#attributes' => array('class' => array('button-clear-left')),
    '#submit' => array('gti_keyword_search_create_group_form_submit'),
  );

  // Saved overrides defined by the admin user, based on the current vocab
  $overrides = _get_saved_groups();

  // Loop and output all saved term names and mail template ids marked as disabled

  $bundles = _get_bundles();
  foreach ($overrides as $entity_id => $data) {
    $i=0;
    $pre = 'keyword_search_';
    $query = db_select('gti_keyword_search_groups', 'groups');
    $query->fields('groups', array('id', 'group_name','bundles'))->condition('id',$entity_id);

    $result = $query->execute();

    $row = $result->fetchAssoc();

    $types = unserialize($row['bundles']);



    $form[$pre . $entity_id] =  array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#attributes' => array('class' => array('fieldset-float-left-clear')),
    );
    $form[$pre . $entity_id]['group_name'.$entity_id] = array(
      '#type' => 'textfield',
      '#title' => 'Group Name',
      '#value' => $data['group_name'],
      '#maxlength' => 1024,
      '#size' => 8,
      '#disabled' => false,
      '#allow_focus'=>true
    );
    $form[$pre . $entity_id]['bundle'.$entity_id] = array(
      '#type' => 'select',
      '#title' => 'Bundles',
      '#multiple' => TRUE,
      '#options' => $bundles,
      '#description' => t(''),

    );

    foreach($types as $type){
      $form[$pre . $entity_id]['bundle_term_button_'.$i."_".$entity_id] = array(
        '#type' => 'button',
        '#executes_submit_callback' => TRUE,
        '#value' => t('Delete Bundle: '.$bundles[$type]."|" . $entity_id),
        '#attributes' => array('class' => array('button-clear-left')),
      );
      $i++;
    }

    $form[$pre . $entity_id]['edit_term_button'.$entity_id] = array(
      '#type' => 'button',
      '#executes_submit_callback' => TRUE,
      '#value' => t('Edit: ' . $entity_id),
      '#attributes' => array('class' => array('button-clear-left')),
    );
    $form[$pre . $entity_id]['delete_term_button'.$entity_id] = array(
      '#type' => 'button',
      '#executes_submit_callback' => TRUE,
      '#value' => t('Delete: ' . $entity_id),
      '#attributes' => array('class' => array('button-clear-left')),
    );
  }


  return $form;

}

function gti_keyword_search_create_group_form_submit($form, &$form_state) {

  $action = $form_state['triggering_element']['#value'];
  $process = false;
  global  $user;
  $name = $form['keyword_facet_group_name']['group_name']['#value'];
  if($action == '+') {
      $today = new DateTime();
      db_insert('gti_keyword_search_groups')
        ->fields(array(
          'uid' => $user->uid,
          'group_name' => $form['keyword_facet_group_name']['group_name']['#value'],
          'bundles' => serialize($form['keyword_facet_group_name']['bundle']['#value']),
          'created' => $today->format("Y-m-d h:i:s")
        ))
        ->execute();
      $types = $form['keyword_facet_group_name']['bundle']['#value'];
      $process = true;
  }else{
    $action = explode(':',$action);
    if(preg_match('/Edit/',$action[0])){

      $name = $form_state['input']['group_name'.trim($action[1])];
      $bundles = $form_state['input']['bundle'.trim($action[1])];

      $query = db_select('gti_keyword_search_groups', 'groups');
      $query->fields('groups', array('id', 'group_name','bundles'))->condition('id',trim($action[1]));

      $result = $query->execute();

      $row = $result->fetchAssoc();

      $rBundles = unserialize($row['bundles']);

      if(isset($rBundles)){
        $types = unserialize($row['bundles']);
        if(isset($bundles)){
          foreach($bundles as $bundle){
            $nBundles[$bundle] = $bundle;
          }
          $types = array_merge($types,$nBundles);
        }


      }else{
        $types =  $bundles;
      }

      $query = db_update("gti_keyword_search_groups")->fields(array('group_name'=>$name,'bundles'=>serialize($types)))->condition('id', trim($action[1]));

      $query->execute();

      $process = true;

    }else if(preg_match('/Delete Bundle/',$action[0])){
      $bundles = _get_bundles();
      $action = explode('|',$action[1]);
      $query = db_select('gti_keyword_search_groups', 'groups');
      $query->fields('groups', array('id', 'group_name','bundles'))->condition('id',$action[1]);

      $result = $query->execute();

      $row = $result->fetchAssoc();

      $types = unserialize($row['bundles']);

      $keys = array_keys($types);
      $updated = false;
      for($m=0;$m<sizeof($bundles);$m++){
        for($i=0;$i<sizeof($keys);$i++){
          $bundle = $bundles[$keys[$i]];
          if($bundle == trim($action[0])){
            if(!$updated){
              $unsetTypes[$keys[$i]] = $types[$keys[$i]];
              unset($types[$keys[$i]]);
              db_update("gti_keyword_search_groups")->fields(array('bundles'=>serialize($types)))->condition('id', $action[1])->execute();
              $types = $unsetTypes;
              $name = "";
              $process = true;
              $updated = true;
            }

          }
        }

      }

    }else if(preg_match('/Delete/',$action)){

      db_delete("gti_keyword_search_groups")->condition('id', $action[1])->execute();
    }
  }
  if($process){
    foreach($types as $type) {
      $count = db_query('SELECT COUNT(DISTINCT nid) FROM {node} WHERE type=:type',array(':type'=>$type))->fetchField();
      $itteration[] =
        array('gti_keyword_search_save_nodes',array($type,$name,$count));
    }

    $batch = array(
      'operations' => $itteration,
      'title' => t('Adding Facet Group Name'),
      'finished' => 'gti_keyword_search_save_nodes_finished',
      'init_message' => t("Saving Facet Group Name To Node(s)"),
      'progress_message' => t('Processed @current out of @total.'),
      'error_message' => t('Error encountered when saving group facet nodes.'),
      'file' => drupal_get_path('module', 'gti_keyword_search') . '/gti_keyword_search.admin.inc',
    );
    batch_set($batch);
    // Run the batch process.
    batch_process("admin/config/gti/keyword-search/create-group");
  }



}

function gti_keyword_search_save_nodes($type,$name,$count,&$context){

  if (!isset($context['sandbox']['progress'])) {

    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_node'] = 0;
    $context['sandbox']['max'] = $count;
  }

  $save_limit = 50;
  if($context['sandbox']['progress'] < $context['sandbox']['max']){
    $query = db_select('node','node')
      ->extend('PagerDefault')
      ->condition("nid",$context['sandbox']['current_node'],'>')
      ->condition("type",$type)
      ->fields('node', array('nid'))
      ->orderBy('node.nid', 'ASC')
      ->limit($save_limit);
    // Add fields and conditions etc.
    $result = $query->execute();


    foreach($result as $row){

      $node = node_load($row->nid, NULL, TRUE);
      if($node){

        $node->group_facet[LANGUAGE_NONE][0]['value'] = $name;
        node_save($node);
        $context['results'][] = check_plain($node->title);

       // error_log('nid '.$node->nid);
        // Update our progress information.
        $context['sandbox']['progress']++;
        $context['sandbox']['current_node'] = $node->nid;
        $context['message'] = t('Now processing Batch API overview', array('Batch API overview' =>$name ." ". $node->title));
      }else{
        $context['sandbox']['progress']++;
      }



    }
  }

  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }

}

function gti_keyword_search_save_nodes_finished($success, $results, $operations){
  if ($success) {
    drupal_set_message('Adding a Group is complete!');
  }
  else {
    $error_operation = reset($operations);
    $message = t('An error occurred while processing %error_operation with arguments: @arguments', array(
      '%error_operation' => $error_operation[0],
      '@arguments' => print_r($error_operation[1], TRUE)
    ));
    drupal_set_message($message, 'error');
  }
  drupal_set_message(l('Run again', 'gti_keyword_search/save_nodes'));

}



function _get_groups() {
  $groups = _get_saved_groups();
  $list = array();
  foreach ($groups as $entity_id => $data) {

    $list[$entity_id] = $data['group_name'];

  }
  return $list;

}

function _get_bundles() {
  $overrides = array();

  $query = db_select('node_type', 'node');
  $query->fields('node', array('type', 'name'));
  $query->groupBy('type');

  $result = $query->execute();

  while($record = $result->fetchAssoc()){

    $overrides[$record['type']] = $record['name'];

  }
  return $overrides;
}


