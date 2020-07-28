<?php
$plugin = [
  'single' => TRUE,
  'title' => t('GTI user saved content button'),
  'description' => t('User saved content button'),
  'content_types' => ['gti_user_saved_content_button'],
  'category' => t('GTI Media Ltd'),
  'edit form' => 'gti_user_saved_content_button_edit_form',
  'render callback' => 'gti_user_saved_content_button_render',
  'required context' => new ctools_context_required(t('Node'), 'node'),
];


/**
 * Custom pane config form
 *
 * @param $form
 * @param $form_state
 *
 * @return mixed
 */
function gti_user_saved_content_button_edit_form($form, &$form_state) {
  return $form;
}

/**
 * Render save content button
 *
 * @param $subtype
 *   The subtype being rendered.
 * @param $conf
 *   Pane config
 * @param $args
 * @param $contexts
 *
 * @return \stdclass
 */
function gti_user_saved_content_button_render($subtype, $conf, $args, $contexts) {
  if (empty($contexts) || empty($contexts->data)) {
    return;
  }
//  if ($contexts->data->type == 'course_permutation') {
//    $nid = $contexts->data->field_course[LANGUAGE_NONE][0]['target_id'];
//  }
//  else {
    $nid = $contexts->data->nid;
 // }
  if ($nid) {
    $block = new stdclass();
    $block->content[] = [
      '#type' => 'markup',
      '#markup' => _generate_save_button($nid),
    ];
  }
  return $block;
}
