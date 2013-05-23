<?php

/**
 * @file
 * Model editing UI.
 *
 * We make very little use of the EntityAPI interface for this - preferring instead to use
 * views. That offers more flexibility to change a UI that will, more often than not,
 * be end-user facing.
 */

/**
 * UI controller.
 */
class ModelUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults. Main reason for doing this is that
   * parent class hook_menu() is optimized for entity type administration.
   */
  public function hook_menu() {
    
    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;

    $items[$this->path] = array(
      'title' => 'Models',
      'description' => 'Add edit and update raps.',
      'page callback' => 'system_admin_menu_block_page',
      'access arguments' => array('access administration pages'),
      'file path' => drupal_get_path('module', 'system'),
      'file' => 'system.admin.inc',
    );
    
    // Change the overview menu type for the list of raps.
    $items[$this->path]['type'] = MENU_LOCAL_TASK;
    
    // Change the add page menu to multiple types of entities
    $items[$this->path . '/add'] = array(
      'title' => 'Add a rap',
      'description' => 'Add a new rap',
      'page callback'  => 'rap_add_page',
      'access callback'  => 'rap_access',
      'access arguments' => array('edit'),
      'type' => MENU_NORMAL_ITEM,
      'weight' => 20,
      'file' => 'rap.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])

    );
    
    // Add menu items to add each different type of entity.
    foreach (rap_get_types() as $type) {
      $items[$this->path . '/add/' . $type->type] = array(
        'title' => 'Add ' . $type->label,
        'page callback' => 'rap_form_wrapper',
        'page arguments' => array(rap_create(array('type' => $type->type))),
        'access callback' => 'rap_access',
        'access arguments' => array('edit', 'edit ' . $type->type),
        'file' => 'rap.admin.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module'])
      );
    }

    // Loading and editing rap entities
    $items[$this->path . '/rap/' . $wildcard] = array(
      'page callback' => 'rap_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'rap_access',
      'access arguments' => array('edit', $id_count + 1),
      'weight' => 0,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'rap.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    $items[$this->path . '/rap/' . $wildcard . '/edit'] = array(
      'title' => 'Edit',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    );
    
    $items[$this->path . '/rap/' . $wildcard . '/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'rap_delete_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'rap_access',
      'access arguments' => array('edit', $id_count + 1),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_INLINE,
      'weight' => 10,
      'file' => 'rap.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    
    // Menu item for viewing raps
    $items['rap/' . $wildcard] = array(
      //'title' => 'Title',
      'title callback' => 'rap_page_title',
      'title arguments' => array(1),
      'page callback' => 'rap_page_view',
      'page arguments' => array(1),
      'access callback' => 'rap_access',
      'access arguments' => array('view', 1),
      'type' => MENU_CALLBACK,
    );
    return $items;
  }
  
  
  /**
   * Create the markup for the add Model Entities page within the class
   * so it can easily be extended/overriden.
   */ 
  public function addPage() {
    $item = menu_get_item();
    $content = system_admin_menu_block($item);

    if (count($content) == 1) {
      $item = array_shift($content);
      drupal_goto($item['href']);
    }    
        
    return theme('rap_add_list', array('content' => $content));
  }
  
}


/**
 * Form callback wrapper: create or edit a rap.
 *
 * @param $rap
 *   The rap object being edited by this form.
 *
 * @see rap_edit_form()
 */
function rap_form_wrapper($rap) {
  // Add the breadcrumb for the form's location.
  rap_set_breadcrumb();
  return drupal_get_form('rap_edit_form', $rap);
}


/**
 * Form callback wrapper: delete a rap.
 *
 * @param $rap
 *   The rap object being edited by this form.
 *
 * @see rap_edit_form()
 */
function rap_delete_form_wrapper($rap) {
  // Add the breadcrumb for the form's location.
  //rap_set_breadcrumb();
  return drupal_get_form('rap_delete_form', $rap);
}


/**
 * Form callback: create or edit a rap.
 *
 * @param $rap
 *   The rap object to edit or for a create form an empty rap object
 *     with only a rap type defined.
 */
function rap_edit_form($form, &$form_state, $rap) {
  // Add the default field elements.
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Model Name'),
    '#default_value' => isset($rap->name) ? $rap->name : '',
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -5,
  );
  
  $form['data']['#tree'] = TRUE;
  $form['data']['sample_data'] = array(
    '#type' => 'checkbox',
    '#title' => t('An interesting rap switch'),
    '#default_value' => isset($rap->data['sample_data']) ? $rap->data['sample_data'] : 1,
  );

  // Add the field related form elements.
  $form_state['rap'] = $rap;
  field_attach_form('rap', $rap, $form, $form_state);

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 400,
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save rap'),
    '#submit' => $submit + array('rap_edit_form_submit'),
  );
  
  if (!empty($rap->name)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete rap'),
      '#suffix' => l(t('Cancel'), 'admin/content/raps'),
      '#submit' => $submit + array('rap_form_submit_delete'),
      '#weight' => 45,
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'rap_edit_form_validate';
  return $form;
}


/**
 * Form API validate callback for the rap form
 */
function rap_edit_form_validate(&$form, &$form_state) {
  $rap = $form_state['rap'];
  
  // Notify field widgets to validate their data.
  field_attach_form_validate('rap', $rap, $form, $form_state);
}


/**
 * Form API submit callback for the rap form.
 * 
 * @todo remove hard-coded link
 */
function rap_edit_form_submit(&$form, &$form_state) {
  
  $rap = entity_ui_controller('rap')->entityFormSubmitBuildEntity($form, $form_state);
  // Save the rap and go back to the list of raps
  
  // Add in created and changed times.
  if ($rap->is_new = isset($rap->is_new) ? $rap->is_new : 0){
    $rap->created = time();
  }

  $rap->changed = time();
  
  $rap->save();
  $form_state['redirect'] = 'admin/content/raps';
}

/**
 * Form API submit callback for the delete button.
 * 
 * @todo Remove hard-coded path
 */
function rap_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/content/raps/rap/' . $form_state['rap']->rap_id . '/delete';
}


/**
 * Form callback: confirmation form for deleting a rap.
 *
 * @param $rap
 *   The rap to delete
 *
 * @see confirm_form()
 */
function rap_delete_form($form, &$form_state, $rap) {
  $form_state['rap'] = $rap;

  $form['#submit'][] = 'rap_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete rap %name?', array('%name' => $rap->name)),
    'admin/content/raps/rap',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );
  
  return $form;
}

/**
 * Submit callback for rap_delete_form
 */
function rap_delete_form_submit($form, &$form_state) {
  $rap = $form_state['rap'];

  rap_delete($rap);

  drupal_set_message(t('The rap %name has been deleted.', array('%name' => $rap->name)));
  watchdog('rap', 'Deleted rap %name.', array('%name' => $rap->name));

  $form_state['redirect'] = 'admin/content/raps';
}



/**
 * Page to add Model Entities.
 *
 * @todo Pass this through a proper theme function
 */
function rap_add_page() {
  $controller = entity_ui_controller('rap');
  return $controller->addPage();
}


/**
 * Displays the list of available rap types for rap creation.
 *
 * @ingroup themeable
 */
function theme_rap_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<dl class="rap-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer rap types')) {
      $output = '<p>' . t('Model Entities cannot be added because you have not created any rap types yet. Go to the <a href="@create-rap-type">rap type creation page</a> to add a new rap type.', array('@create-rap-type' => url('admin/structure/rap_types/add'))) . '</p>';
    }
    else {
      $output = '<p>' . t('No rap types have been created yet for you to use.') . '</p>';
    }
  }

  return $output;
}





/**
 * Sets the breadcrumb for administrative rap pages.
 */
function rap_set_breadcrumb() {
  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('Administration'), 'admin'),
    l(t('Content'), 'admin/content'),
    l(t('Models'), 'admin/content/raps'),
  );

  drupal_set_breadcrumb($breadcrumb);
}



