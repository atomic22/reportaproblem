<?php
//$Id$

/*
 * @file
 * Report a problem ticket form
 */
function rap_tickets_form(){
  $form = array();
  
  $form['overview'] = array(
    '#markup' => t('Report problems to city management'),
  );
  
  $options = db_query('SELECT pid, pname FROM {rap_problems}')->fetchAllKeyed();
  
  $form['problem'] = array(
    '#title' => t('Problem'),
    '#description' => t('Please select a problem from the drop down menu'),
    '#type' => 'select',
    '#options' => $options,
    '#required' => TRUE,
  );
  $form['title'] = array(
    '#title' => t('Issue Name'),
    '#description' => t('Please give a name to your problem.'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['description'] = array(
    '#title' => t('Description'),
    '#description' => t('Please provide a brief description of the problem.'),
    '#type' => 'textarea',
    '#required' => TRUE,
  );
  $form['contact_information'] = array(
    '#title' => t('Contact Information'),
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
  );
  $form['contact_information']['ticket_fn'] = array(
	'#title' => t('First Name'),
	'#type' => 'textfield',
	'#required' => TRUE,
  );
  $form['contact_information']['ticket_ln'] = array(
	'#title' => t('Last Name'),
	'#type' => 'textfield',
	'#required' => TRUE,
  );
  $form['contact_information']['ticket_email'] = array(
	'#title' => t('Contact Email'),
	'#description' => t('Email Address for follow up on submitted tickets.'),
	'#type' => 'textfield',
	'#required' => FALSE,
  );
  $form['contact_information']['ticket_phone'] = array(
	'#title' => t('Contact Phone Number'),
	'#description' => t('Phone number for follow up on submitted tickets. Please supply if email address is not submitted'),
	'#type' => 'textfield',
	'#required' => FALSE,
  );
  $form['contact_information']['ticket_houseno'] = array(
    '#title' => t('House Number'),
    '#description' => t('If the problem is not at or in your home, please supply the house or nearest house number for the issue'),
    '#type' => 'textfield',
    '#required' => FALSE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

function rap_tickets_form_submit($form, &$form_state){
  $tid = db_insert("rap_ticket")
      ->fields(array(
        'created' => REQUEST_TIME,
        'pid' => $form_state['values']['problem'],
        'title' => $form_state['values']['title'],
        'description' => $form_state['values']['description'],
        'ticket_fn' => $form_state['values']['ticket_fn'],
        'ticket_ln' => $form_state['values']['ticket_ln'],
        'ticket_email' => $form_state['values']['ticket_email'],
        'ticket_phone' => $form_state['values']['ticket_phone'],
        'ticket_houseno' => $form_state['values']['ticket_houseno'],
      ))
      ->execute();
      $form_state['redirect'] = 'rap/ticket/' .$tid;
}