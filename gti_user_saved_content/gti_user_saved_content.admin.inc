<?php
function _get_groups(){
    $results = db_query("SELECT * from gti_user_saved_content_groups");
    $groups = array();
    while($row = $results->fetchAssoc()){
        $groups[$row['saved_content_group_id']]['name'] = $row['name'];
        $groups[$row['saved_content_group_id']]['savebtntext'] = $row['savebtntext'];
        $groups[$row['saved_content_group_id']]['removebtntext'] = $row['removebtntext'];
        if($group_bundles = _get_group_content_types($row['saved_content_group_id'])){
            $groups[$row['saved_content_group_id']]['bundles'] = $group_bundles;
        }
    }
    if(empty($groups)){
        return false;
    }else{
        return $groups;
    }
}

function _get_group_content_types($group_id)
{
    $results = db_query("SELECT bundle FROM gti_user_saved_content_group_types WHERE saved_content_group_id = :id", array(":id" => $group_id));
    if(!$results){
        return false;
    }
    $content_types = array();
    while($row = $results->fetchAssoc()) {
        $content_types[$row['bundle']] = $row['bundle'];
    }
    return $content_types;
}


/**
 * Returns form array for user saved settings form
 *
 */
function gti_user_saved_content_settings()
{

    $groups = _get_groups();
    $rows = array();
    if(!empty($groups)){
        foreach($groups as $group_id => $group_details){
            $bundles = '';
            foreach($group_details['bundles'] as $bundle){
                if(!empty($bundles)){
                    $bundles .= ','.$bundle;
                }else{
                    $bundles .= $bundle;
                }
            }
            $rows[] = array($group_details['name'],$group_details['savebtntext'],$group_details['removebtntext'],$bundles,l("remove",'/user-saved-content/remove-group/'.$group_id));
        }
    }

    $variables = array(
        "header" =>array(t('Name'),t('Save button text'),t('Remove button text'),t('Bundles'),t('Remove')),
        "rows" =>$rows,
        "attributes"=>array(),
        "caption"=>"User saved content groups",
        "colgroups"=>array(),
        "empty"=>"No groups exist yet",
        "sticky"=>true,
    );

    $group_table =theme_table($variables);


    $form = array();
    $form['groups'] = array(
        '#markup' => $group_table
    );
    $form['name'] = array(
        '#title'=> 'Group name',
        '#type'=>'textfield',
        '#default_value'=>'',
        '#required'=>true,
    );

    $bundles = field_info_bundles("node");
    $options = array();
    foreach($bundles as $bundle => $entity_info){
        $options["$bundle"] = $entity_info['label'];
    }

    $form['content_types'] = array(
        '#title'=> 'Content types',
        '#type'=>'select',
        '#default_value'=>'',
        '#options'=>$options,
        '#multiple'=> true,
        '#size' => 10,
        '#required'=>true,
    );

    $form['save_button_text'] = array(
        '#title'=> 'Text for save button',
        '#type'=>'textfield',
        '#default_value'=>'',
        '#required'=>true,
    );

    $form['remove_button_text'] = array(
        '#title'=> 'Text for remove button',
        '#type'=>'textfield',
        '#default_value'=>'',
        '#required'=>true,
    );

    $form['submit'] = array('#type' => 'submit', '#value' => t('Create group'));

    return $form;
}

/**
 * Implementation of hook_submit
 *
 * @param $form
 * @param $form_state
 */
function gti_user_saved_content_settings_submit($form,$form_state){

    db_insert('gti_user_saved_content_groups')
        ->fields(array(
            'name' => $form_state['values']['name'],
            'savebtntext' => $form_state['values']['save_button_text'],
            'removebtntext' => $form_state['values']['remove_button_text'],
        ))->execute();
    $saved_content_group_id = Database::getConnection()->lastInsertId();
    foreach($form_state['values']['content_types'] as $value){
        db_insert('gti_user_saved_content_group_types')
            ->fields(array(
                'saved_content_group_id' => $saved_content_group_id,
                'bundle' => $value,
            ))->execute();
    }
    drupal_set_message("Saved settings");
}




function delete_user_saved_content_group($group_id){
    db_delete("gti_user_saved_content_groups")
        ->condition("saved_content_group_id",$group_id)
        ->execute();
    db_delete("gti_user_saved_content_group_types")
        ->condition("saved_content_group_id",$group_id)
        ->execute();
    drupal_goto('admin/config/gti/user-saved-content');
    drupal_set_message("Group removed");
}

/**
 * Application Statuses form
 *
 * @param $form
 * @param $form_state
 *
 * @return mixed
 */
function gti_usc_application_status_form($form, &$form_state) {

  $results = _get_statues();

  $form['gti_usc_application_statuses']['#tree'] = TRUE;
  $form['add']['#tree'] = TRUE;
  foreach ($results as $item) {

    $form['gti_usc_application_statuses'][$item->statusId]['id'] = [
      '#type' => 'hidden',
      '#default_value' => $item->statusId,
    ];
    $form['gti_usc_application_statuses'][$item->statusId]['ats_name'] = [
      '#type' => 'textfield',
      '#title' => 'ATS message status name',
      '#title_display' => 'invisible',
      '#default_value' => $item->ats_name,
      '#size' => 30,
      '#maxlength' => 128,
    ];
    $form['gti_usc_application_statuses'][$item->statusId]['name'] = [
      '#type' => 'textfield',
      '#title' => 'Application status name',
      '#title_display' => 'invisible',
      '#default_value' => $item->name,
      '#size' => 30,
      '#maxlength' => 128,
    ];
    $form['gti_usc_application_statuses'][$item->statusId]['message'] = [
      '#type' => 'textfield',
      '#title' => 'Message',
      '#title_display' => 'invisible',
      '#default_value' => $item->message,
      '#size' => 30,
      '#maxlength' => 512,
    ];
    $form['gti_usc_application_statuses'][$item->statusId]['button_text'] = [
      '#type' => 'textfield',
      '#title' => 'Button text',
      '#title_display' => 'invisible',
      '#default_value' => $item->button_text,
      '#size' => 30,
      '#maxlength' => 125,
    ];
    $form['gti_usc_application_statuses'][$item->statusId]['delete'] = [
      '#type' => 'submit',
      '#value' => t('delete'),
      '#submit' => ['gti_ats_delete_application_status'],
      '#name' => $item->statusId,
    ];
  }

  $form['add']['gti_usc_application_status'] = [
    '#type' => 'fieldset',
    '#title' => 'Add new application status',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  ];

  $form['add']['gti_usc_application_status']['ats_name'] = [
    '#type' => 'textfield',
    '#title' => 'ATS message status name',
    '#size' => 30,
    '#maxlength' => 85,
  ];
  $form['add']['gti_usc_application_status']['name'] = [
    '#type' => 'textfield',
    '#title' => 'Status name',
    '#size' => 30,
    '#maxlength' => 85,
  ];
  $form['add']['gti_usc_application_status']['message'] = [
    '#type' => 'textfield',
    '#title' => 'Message',
    '#size' => 30,
    '#maxlength' => 512,
  ];
  $form['add']['gti_usc_application_status']['button_text'] = [
    '#type' => 'textfield',
    '#title' => 'Button text',
    '#size' => 30,
    '#maxlength' => 125,
  ];
  $form['submit'] = [
    '#type' => 'submit',
    '#value' => t('Save changes'),
  ];

  $form['#theme'] = 'gti_usc_application_status_form';

  return $form;

}


/**
 * Returns all statuses configuration
 *
 * @return mixed
 */
function _get_statues() {

  try {
    $query = db_select('gti_user_saved_content_application_statuses', 'aas')
      ->fields('aas', ['statusId', 'ats_name', 'name', 'message', 'button_text']);
    $results = $query->execute()->fetchAll();

    return $results;

  } catch (Exception $e) {
    watchdog('gti_ats', 'Couldn\'t retrieve application statuses gti_user_saved_content_application_statuses', [], WATCHDOG_ERROR);
  }

}


/**
 * Render setting form as a table
 *
 * @param $variables
 *
 * @return string
 * @throws \Exception
 */
function theme_gti_usc_application_status_form($variables) {

  $header = [
    t('ATS message status name'),
    t('Status name'),
    t('Message'),
    t('Button text'),
    t(''),
  ];
  $form = $variables['form'];

  $rows = [];
  foreach (element_children($form['gti_usc_application_statuses']) as $key) {

    $rows[] = [
      'data' => [
        drupal_render($form['gti_usc_application_statuses'][$key]['ats_name']),
        drupal_render($form['gti_usc_application_statuses'][$key]['name']),
        drupal_render($form['gti_usc_application_statuses'][$key]['message']),
        drupal_render($form['gti_usc_application_statuses'][$key]['button_text']),
        drupal_render($form['gti_usc_application_statuses'][$key]['delete']),
      ],
    ];
  }

  $output = theme('table', [
    'header' => $header,
    'rows' => $rows,
  ]);

  // render remaining form elements
  $output .= drupal_render_children($form);


  return $output;
}


/**
 * Submit new ATS Application Status
 *
 * @param $form
 * @param $form_state
 */
function gti_usc_application_status_form_submit(&$form, &$form_state) {

  // update existing ATS application status
  if (!empty($form_state['values']['gti_usc_application_statuses'])) {
    foreach ($form_state['values']['gti_usc_application_statuses'] as $key => $item) {

      try {
        db_update('gti_user_saved_content_application_statuses')
          ->fields([
            'ats_name' => $item['ats_name'],
            'name' => $item['name'],
            'message' => $item['message'],
            'button_text' => $item['button_text'],
          ])
          ->condition('statusId', $key, '=')
          ->execute();
      } catch (Exception $e) {
        watchdog('gti_ats', "Error updating gti_user_saved_content_application_statuses row" . $e->getMessage(), [], WATCHDOG_ERROR);
      }
    }
  }

  // Add new ATS application status
  if (!empty($form_state['values']['add']['gti_usc_application_status']['name'])) {

    try {
      $result = db_insert('gti_user_saved_content_application_statuses')
        ->fields([
          'ats_name' => $form_state['values']['add']['gti_usc_application_status']['ats_name'],
          'name' => $form_state['values']['add']['gti_usc_application_status']['name'],
          'message' => $form_state['values']['add']['gti_usc_application_status']['message'],
          'button_text' => $form_state['values']['add']['gti_usc_application_status']['button_text'],
        ])
        ->execute();
    } catch (Exception $e) {
      watchdog('gti_ats', "Error inserting gti_user_saved_content_application_statuses row" . $e->getMessage(), [], WATCHDOG_ERROR);
    }

  }
}


/**
 * Delete ATS Application Status
 *
 * @param $form
 * @param $form_state
 */
function gti_ats_delete_application_status($form, &$form_state) {
  $statusId = $form_state['triggering_element']['#name'];
  db_delete('gti_user_saved_content_application_statuses')
    ->condition('statusId', $statusId, '=')
    ->execute();
}
