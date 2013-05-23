<?php
//$Id$

/**
* @file
* Problem Settings.
*/

function rap_problems_form(){
  $form = array();
  
  $form['overview'] = array(
	'#markup' => t('This form allows administrators to manage the problems for Report a Problem'),
  );
  
  $form['problem_name'] = array(
	'#title' => t('Problem Name'),
	'#description' => t('Name of the Problem'),
	'#type' => 'textfield',
	'#required' => TRUE,
  );
  
  //RETURN A LIST OF ALL THE DEPARTMENTS - PULLING DATA RIGHT OUT OF THE TAXONOMY DATABASE IN CITY DRUPAL SITE.
  //No need for the department form. Will leave for distribution purposes if needed.
  $options = db_query('SELECT tid, name FROM {cityofs_drupal}.{taxonomy_term_data} WHERE vid = 2')->fetchAllKeyed();

    $form['department_name'] = array(
	'#title' => t('Department Name'),
	'#description' => t('What department does this problem belong to?'),
	'#type' => 'select',
	'#options' => $options,
	'#required' => TRUE,
  );
  
  //Basic Contact Information
  $form['problem_information'] = array(
	'#title' => t('Problem Information'),
	'#description' => t('Problem Contact Information'),
	'#type' => 'fieldset',
	'#collapsible' => TRUE,
  );
  $form['problem_information']['problem_fn'] = array(
	'#title' => t('Contact First Name'),
	'#description' => t('First name of the person who should recieve this problem'),
	'#type' => 'textfield',
	'#required' => TRUE,
  );
  $form['problem_information']['problem_ln'] = array(
	'#title' => t('Contact Last Name'),
	'#description' => t('Last name of the person who should recieve this problem'),
	'#type' => 'textfield',
	'#required' => TRUE,
  );
  $form['problem_information']['problem_email'] = array(
	'#title' => t('Contact Email'),
	'#description' => t('Email Address of the person who should recieve this problem'),
	'#type' => 'textfield',
	'#required' => TRUE,
  );
  $form['problem_information']['problem_phone'] = array(
	'#title' => t('Contact Phone Number'),
	'#description' => t('Phone number of the person who should recieve this problem'),
	'#type' => 'textfield',
	'#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}
/*
 * Add form validation
 */

/*
   * Process a validated Department Form for Report a Problem
   */
  function rap_problems_form_submit($form, &$form_state){
   
   $pname = $form_state['values']['problem_name'];
   $did = $form_state['values']['department_name'];
   $pfname = $form_state['values']['problem_fn'];
   $plname = $form_state['values']['problem_ln'];
   $pemail = $form_state['values']['problem_email'];
   $pphone = $form_state['values']['problem_phone'];
   
   rap_db_save_problem($pname, $did, $pfname, $plname, $pemail, $pphone);
};