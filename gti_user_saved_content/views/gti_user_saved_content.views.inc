<?php

/**
 * Implements hook_views_pre_build().
 */
function gti_user_saved_content_views_pre_build(&$view){
    static $script_added;
//we only want to add the script once regardless of the number of views being output
    if(!$script_added){
        if($view->base_table == 'gti_user_saved_content'){
            $script_added = true;
            drupal_add_library('system', 'drupal.ajax');  // This enables the nojs/ajax url replacement functionality
        }
    }
}
/**
 * @file
 * Views definitions for gti_user_saved_content module.
 */

/**
 * Implements hook_views_data().
 */
function gti_user_saved_content_views_data() {
    $data = array();
    //define views field handler
    $data['gti_user_saved_content']['remove_content'] = array(
        'title' => t('Link to remove content'),
        'help' => t('Removes associated content from users repository.'),
        'field' => array(
            'handler' => 'gti_user_saved_content_handler_remove_content',
        ),
    );

    //define views data for new table
    $data['gti_user_saved_content']['table']['group'] = t('User saved content');

    $data['gti_user_saved_content']['table']['base'] = array(
        'title' => t('User saved content'),
        'help' => t('This table contains ids linked to content / nodes saved by a user.'),
    );

    $data['gti_user_saved_content']['table']['join'] = array(
        'node' => array(
            'left_field' => 'nid',
            'field' => 'nid',
        ),
        'users' => array(
          'left_field' => 'uid',
          'field' => 'uid',
        ),
        'gti_user_saved_content_application_statuses' => array(
          'left_field' => 'statusId',
          'field' => 'statusId',
        ),
    );

    $data['gti_user_saved_content']['nid'] = array(
        'title' => t('Node id'),
        'help' => t('What node the saved content record references'),
        'field' => array(
            'handler' => 'views_handler_field_node',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
        'relationship' => array(
            'base' => 'node', // The name of the table to join with.
            'base field' => 'nid', // The name of the field on the joined table.
            'handler' => 'views_handler_relationship',
            'label' => t('Saved nodes'),
            'title' => t('User saved nodes'),
            'help' => t('Joins user saved content to nodes'),
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_node_nid',
            'numeric' => TRUE,
            'validate type' => 'nid',
        ),
    );

//    $data['gti_user_saved_content']['table']['join'] = array(
//        'users' => array(
//            'left_field' => 'uid',
//            'field' => 'uid',
//        ),
//    );

    $data['gti_user_saved_content']['uid'] = array(
        'title' => t('User ID'),
        'help' => t('ID of the user this record belongs to.'),
        'field' => array(
            'handler' => 'views_handler_field_user',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
        'relationship' => array(
            'base' => 'users', // The name of the table to join with.
            'base field' => 'uid', // The name of the field on the joined table.
            'handler' => 'views_handler_relationship',
            'label' => t('User'),
            'title' => t('Link to user'),
            'help' => t('User that the saved content record relates to'),
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_user_uid',
            'numeric' => TRUE,
            'validate type' => 'uid',
        ),
    );

//    $data['gti_user_saved_content']['applied'] = array(
//        'title' => t('Applied'),
//        'help' => t('Whether the user has applied for this content or not'),
//        'field' => array(
//            'handler' => 'views_handler_field_numeric',
//        ),
//        'sort' => array(
//            'handler' => 'views_handler_sort',
//        ),
//        'filter' => array(
//            'handler' => 'views_handler_filter_numeric',
//        ),
//    );

    $data['gti_user_saved_content']['position'] = array(
      'title' => t('Priority'),
      'help' => t('The position or priority given to this item'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    );

    $data['gti_user_saved_content']['rating'] = array(
      'title' => t('Rating'),
      'help' => t('The rating given to this item by the user'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    );

    $data['gti_user_saved_content']['notes'] = array(
      'title' => t('Notes'),
      'help' => t('Notes entered by the user'),
      'field' => array(
        'handler' => 'views_handler_field',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
    );

    $data['gti_user_saved_content']['applied'] = array(
      'title' => t('Applied'),
      'help' => t('When user applied for this content'),
      'field' => array(
        'handler' => 'views_handler_field_date',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_date',
      ),
    );




  $data['gti_user_saved_content_application_statuses']['table']['group'] = t('Aplication Statuses');
  $data['gti_user_saved_content_application_statuses']['table']['base'] = array(
    'title' => t('Application Statuses'),
    'field' => 'stateId',
    'help' =>('This table contains aplication states'),
  );

  $data['gti_user_saved_content_application_statuses']['name'] = array(
    'title' => t('Message state'),
    'help' => t('Message state'),
    'field' => array(
      'real field' => 'name',
      'handler' => 'views_handler_field', // Use default handler for string
    ),
    'sort' => array(
      'handler' => 'views_handler_sort', // Use default handler for sort
    ),
    'filter' => array(
      'handler' => 'views_handler_filter',//'gti_message_handler_filter_states',
    ),
  );

  $data['gti_user_saved_content_application_statuses']['message'] = array(
    'title' => t('Application status message'),
    'help' => t('Application status message'),
    'field' => array(
      'real field' => 'message',
      'handler' => 'views_handler_field', // Use default handler for string
    ),
    'sort' => array(
      'handler' => 'views_handler_sort', // Use default handler for sort
    ),
    'filter' => array(
      'handler' => 'views_handler_filter',//'gti_message_handler_filter_states',
    ),
  );

  $data['gti_user_saved_content']['statusId'] = array(
    'title' => t('Status ID'),
    'help' => t('status instance'),
    'field' => array(
      'handler' => 'views_handler_field', // Use default handler for string
    ),
    'sort' => array(
      'handler' => 'views_handler_sort', // Use default handler for sort
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric', // application_id is int now
    ),
    'relationship' => array(
      'base' => 'gti_user_saved_content_application_statuses', // The name of the table to join with.
      'base field' => 'statusId', // The name of the field on the joined table.
      'handler' => 'views_handler_relationship',
      'label' => t('Application Status'),
      'title' => t('Application Status'),
      'help' => t('Joins application statuses table to data table'),
    ),
  );

  return $data;
}

