<?php

class FacetapiDependencyGroupfacet extends FacetapiDependency {

  /**
   * Constructs a FacetapiDependency object.
   *
   * Sets the necessary information in order to check a facet's dependencies.
   *
   * @param array $id
   *   The machine name of the dependency plugin being instantiated as defined
   *   in hook_facetapi_dependencies() implementations.
   * @param FacetapiAdapter $adapter
   *   The adapter associated with facet whose dependencies are being checked.
   * @param array $facet
   *   The facet definition as returned by facetapi_facet_load().
   * @param array $active_items
   *   The facet's active items returned by FacetapiAdapter::getActiveItems().
   * @param stdClass $settings
   *   The facet's global settings.
   */
  public function __construct($id, FacetapiAdapter $adapter, array $facet, stdClass $settings, array $active_items = array()) {
    parent::__construct($id, $adapter, $facet, $settings, $active_items);
  }


  /**
   * Implements FacetapiDependency::execute().
   */
  public function execute() {
    switch ($this->settings['bundle']) {
      case 'referenced':


        // There was no match.
        return FALSE;

      case 'selected':


        // There was no match.
        return FALSE;
    }
  }

  /**
   * Returns an array of enabled facets that filter by bundle.
   *
   * @return array
   *   An array of enabled bundle facets.
   *
   * @see FacetapiDependencyBundle::filterBundleFacets()
   */
  public function getEnabledBundleFacets() {
    $enabled = array_filter($this->adapter->getEnabledFacets(), array($this, 'filterBundleFacets'));
    $active = array_filter($this->activeItems);
    return array_intersect_key($enabled, $active);
  }

  /**
   * Returns TRUE if the facet contains bundle information.
   *
   * @param array $facet
   *   The facet definition as returned by facetapi_facet_load().
   *
   * @return
   *   A boolean flagging whether the item should remain in the array.
   *
   * @see array_filter()
   */
  public function filterBundleFacets($facet) {
    return !empty($facet['field api bundles']);
  }

  /**
   * Overrides FacetapiDependency::settingsForm().
   */
  public function settingsForm(&$form, &$form_state) {

    // Builds array of options.
    $options = array();
    $options['none'] = t('No dependencies.');
    if ($this->facet['field api name']) {
      $options['referenced'] = t('A bundle this field is attached to must be active.');
    }
    $options['selected'] = t('At least one of the selected bundles must be active.');

    $form[$this->id]['bundle'] = array(
      '#title' => t('Dependency settings'),
      '#type' => 'radios',
      '#options' => $options,
      '#default_value' => $this->settings['bundle'],
    );

    $form[$this->id]['bundle_selected'] = array(
      '#title' => t('Required bundles'),
      '#type' => 'checkboxes',
      '#options' => $this->getBundleOptions($form['#facetapi']['adapter']->getTypes()),
      '#default_value' => $this->settings['bundle_selected'],
      '#states' => array(
        'visible' => array(
          'input[name="bundle"]' => array('value' => 'selected'),
        ),
      ),
      '#description' => t('At least one of the selected bundles must be active for this facet to be rendered.')
    );
  }

  /**
   * Returns an array of bundles suitable for use as options in FAPI elements.
   *
   * @param array $entity_types
   *   An array containing the machine names of entities to gather bundles for.
   *
   * @return array
   *   An associative array keyed by machine name of the bundles to escaped
   *   bundle labels.
   */
  public function getBundleOptions(array $entity_types) {
    $options = array();
    foreach ($entity_types as $entity_type) {
      if ($entity_info = entity_get_info($entity_type)) {
        foreach ($entity_info['bundles'] as $bundle => $bundle_info) {
          $options[$bundle] = check_plain($bundle_info['label']);
        }
      }
    }
    return $options;
  }

  /**
   * Overrides FacetapiDependency::getDefaultSettings().
   */
  public function getDefaultSettings() {
    return array(
      'bundle' => 'none',
      'bundle_selected' => array(),
    );
  }
}
