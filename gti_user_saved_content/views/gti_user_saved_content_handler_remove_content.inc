<?php
/**
 * @file
 * Definition of mymodule_handler_handlername.
 */

/**
 * Description of what my handler does.
 */
class gti_user_saved_content_handler_remove_content extends views_handler_field {
    /**
     * Add some required fields needed on render().
     */
    function construct() {
        parent::construct();
        $this->additional_fields = array();
        //we must always have the uid/nid available to the handler
        $this->additional_fields['id_of_the_user'] = 'uid';
        $this->additional_fields['nid_of_the_content'] = 'nid';
    }

    /**
     * Loads additional fields.
     */
    function query() {
        $this->table = 'gti_user_saved_content';
        $this->relationship = null;
        if($this->ensure_my_table()){
            $this->add_additional_fields($this->additional_fields);
        }else{
            watchdog("User saved content","Base table must be gti_user_saved_content");
        }
    }

    /**
     * Default options form.
     */
    function option_definition() {
        $options = parent::option_definition();
        //allow the user to alter the link text
        $options['text_for_link'] = array('default' => '');
        //allow the user to add their own classes
        $options['user_defined_classes'] = array('default' => '');
        return $options;
    }

    /**
     * Creates the form item for the options added.
     */
    function options_form(&$form, &$form_state) {
        parent::options_form($form, $form_state);

        $form['text_for_link'] = array(
            '#type' => 'textfield',
            '#title' => t('Text for link'),
            '#default_value' => $this->options['text_for_link'],
            '#description' => t('What text do you want for the <a>link</a>'),
            '#weight' => -10,
        );

        $form['user_defined_classes'] = array(
            '#type' => 'textfield',
            '#title' => t('Classes to add to link'),
            '#default_value' => $this->options['user_defined_classes'],
            '#description' => t('Add some classes to the link?'),
            '#weight' => -9,
        );
    }

    /**
     * Renders the field handler.
     */
    function render($values) {
        drupal_add_library('system', 'drupal.ajax');  // This enables the nojs/ajax url replacement functionality

        return format_string('<a href="/delete-saved-content-js/nojs/@id_of_node_to_delete/@view_name/@block_name" class="remove-link use-ajax @user_defined_classes">@text_for_link</a>', array(
            '@user_defined_classes' => $this->options['user_defined_classes'],
            '@text_for_link' => $this->options['text_for_link'],
            '@id_of_node_to_delete' => $values->gti_user_saved_content_nid,
            '@view_name' => $this->query->view->name,
            '@block_name' => $this->query->view->current_display,
            //'@id_of_user' => $values->gti_user_saved_content_uid,
        ));
    }
}
