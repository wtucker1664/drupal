<?php
$plugin = array(
  'single' => TRUE,
  'title' => t('GTI user saved content count'),  // Title to show up on the pane screen.
  'description' => t('User saved content count'), // Description to show up on the pane screen.
  'content_types' => array('gti_user_saved_content'),
  'category' => t('GTI Media Ltd'), // A category to put this under.
  'edit form' => 'gti_user_saved_content_count_edit_form', // A function that will return the settings form for the pane.
  'render callback' => 'gti_user_saved_content_count_render', // A function that will return the renderable content.
  'required context' => new ctools_context_required(t('User'), 'user'), //Only available if user context provided
  'defaults' => array( // Array of defaults for the settings form.
    'label' => '',
    'type' => '',
    'link' => '',
    'redirect' => '',
    'fragment' => '',
  ),
);

/**
 * Pane config form
 */
function gti_user_saved_content_count_edit_form($form, &$form_state){
  $types = array_keys(node_type_get_types());
  $types[] = 'saved_search';
  $conf = $form_state['conf'];

  //custom text for displaying text before the count
  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Text to output before count'),
    '#default_value' => $conf['label'] ? $conf['label'] : '',
  );
  //the node type to count
  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Content type to get count for'),
    '#options' => array_combine($types, $types),
    '#default_value' => $conf['type'] ? $conf['type'] : '',
  );
  $form['redirect'] = array(
    '#type' => 'checkbox',
    '#title' => t('Make pane redirect on click'),
    '#default_value' => $conf['redirect'] ? $conf['redirect'] : '',
  );
  $form['link'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect url'),
    '#default_value' => $conf['link'] ? $conf['link'] : '',
  );
  $form['fragment'] = array(
    '#type' => 'textfield',
    '#title' => t('fragment'),
    '#default_value' => $conf['fragment'] ? $conf['fragment'] : '',
  );

  return $form;
}

function gti_user_saved_content_count_edit_form_validate($form, $form_state){
  if($form_state['values']['redirect']) {
    if(empty($form_state['values']['link'])){
      form_error($form['link'], t('URL must not be empty if pane is set to redirect on click'));
    }
  }
}

function gti_user_saved_content_count_edit_form_submit(&$form, &$form_state){
  $form_state['conf']['type'] = $form_state['values']['type'];
  $form_state['conf']['label'] = $form_state['values']['label'];
  $form_state['conf']['redirect'] = $form_state['values']['redirect'];
  $form_state['conf']['link'] = $form_state['values']['link'];
  $form_state['conf']['fragment'] = $form_state['values']['fragment'];
}

/**
 * Render out count for user saved content
 */
function gti_user_saved_content_count_render($subtype, $conf, $args, $contexts){
  //we usually want to use the user being viewed context
    if(is_array($contexts)){
        foreach($contexts as $context){
            if(isset($context->identifier)){
                if($context->identifier == 'User being viewed'){
                    $user = user_load($context->argument);
                }
            }

        }
    }

  //default to logged in user if there is no context from the page.
  if(empty($user)){
    global $user;
  }

  $type = $conf['type'];
  $label = $conf['label'];
  $redirect = $conf['redirect'];
  $link = $conf['link'];

  if(!is_array($contexts)) {
    $new_link = ctools_context_keyword_substitute($link, array(), array($contexts));
  }
  else{
    $new_link = ctools_context_keyword_substitute($link, array(), $contexts);
  }

  $block = new stdclass();
  if($type == 'saved_search'){ // if $type is saved_search
    $count = gti_user_saved_search_count($user);
  }else{
    $count = gti_user_saved_content_count($user, $type);
  }
  $block->content[] = array(
    '#type' => 'markup',
    '#markup' => $label,
    '#prefix' => '<span class="content-count-label">',
    '#suffix' => '</span>'
  );
  $block->content[] = array(
    '#type' => 'markup',
    '#markup' => $count,
    '#prefix' => '<span class="content-count">',
    '#suffix' => '</span>'
  );

  if($redirect) {
    $options = array();
    if(!empty($conf['fragment'])){
      $options['fragment'] = $conf['fragment'];
    }

    drupal_add_js(drupal_get_path('module', 'gti_user_saved_content') . '/gti_user_saved_content.js', 'file');
    $block->content[] = array(
      '#type' => 'link',
      '#href' => $new_link,
      '#attributes' => array('class' => array('gti-saved-content-count-redirect')),
      '#options' => $options,
    );
  }
  return $block;
}
