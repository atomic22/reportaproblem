<?php

/*
 * RAP controller
 */
class RAPController extends DrupalDefaultEntityController {
    function rap_load($pid = NULL, $reset = FALSE){
        $pids = (isset($pid) ? array($pid) : array());
        $problem = rap_load_multiple($pids, $reset);
        return $problem ? reset($problem) : FALSE;
    }
    
    function rap_load_multiple($pids = array(), $reset = FALSE){
        return entity_load('problems', $pids, $reset);
    }
}