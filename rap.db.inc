<?php

/**
 * @file
 * Database wrapper functions.
 */

/**
 * Save a request to database.
 * @param $did
 *   String; The department id the request is for.
 * @param $fields
 *   Array; Values for fields to subscribe.
 */

function rap_db_save_problem($pname, $did, $pfname, $plname, $pemail, $pphone){
    db_query("INSERT INTO {rap_problems} (pname, did, pfname, plname, pemail, pphone) VALUES (:pname, :did, :pfname, :plname, :pemail, :pphone)", array(
      ':pname' => $pname,
      ':did' => $did,
      ':pfname' => $pfname,
      ':plname' => $plname,
      ':pemail' => $pemail,
      ':pphone' => $pphone,
    ));
    drupal_set_message(t('Problem @pname has been entered into database', array('@pname' => $pname)));
}