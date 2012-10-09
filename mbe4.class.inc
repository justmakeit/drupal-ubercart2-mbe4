<?php

/**
 * @file
 * Process payments easy using mbe4 mobile payment.
 *
 * Development sponsored by just make IT! GbR - http://www.justmakeit.de/
 */

/**
 * MBE4-Class Definition
 */
class Mbe4 {
  /**
   * Construct-Function, set some variables.
   */
  function __construct($username, $password, $clientid, $serviceid, $url="https://billing.mbe4.de/widget/singlepayment") {
    $this->username = $username;
    $this->password = $password;
    $this->clientid = $clientid;
    $this->serviceid = $serviceid;
    $this->url = $url;
  }

  /**
   *  Send transaction to mbe4.
   *
   *  @param float $amount
   *        The amount to pay
   *  @param int $contentclass
   *        The mbe4-contentclass, look at readme.txt for more informations.
   *  @param string $returnurl
   *        The mbe4-return-url. After redirect to mbe4.
   *  @param bool $urlencode
   *        Should the GET-Url be encoded with urlencode()?
   *
   *  @return: key/value-Array
   */
  public function createTransaction($id, $description="mbe4 payment", $amount=0, $contentclass=1, $returnurl="", $urlencode=TRUE) {
    // Generate Timestamp.
    $timestamp = date("Y-m-d") . "T" . date("H:i:s.000") . "Z";
    // Generate Hashbase.
    $hashbase =
        $this->password .
        $this->username .
        $this->clientid .
        $this->serviceid .
        $contentclass .
        $description .
        $id .
        $amount .
        $returnurl .
        $timestamp;
    // Hash erzeugen.
    $hashparam = md5($hashbase);
    // Build the data array that will be translated into hidden form values.
    $data = array(
      // General parameters.
      'username' => $this->username,
      'clientid' => $this->clientid,
      'serviceid' => $this->serviceid,
      'contentclass' => $contentclass,
      'description' => $description,
      'clienttransactionid' => $id,
      // Mbe4 wants ct, no eur.
      'amount' => $amount,
      'callbackurl' => $returnurl,
      'timestamp' => $timestamp,
      'hash' => $hashparam,
    );
    // Sollen die Werte mit urlencode() codiert werden?
    if ($urlencode == TRUE) {
      foreach ($data as $element) {
        $element = urlencode($element);
      }
    }
    return $data;
  }

  /**
   * Function validate_transaction($mbe4_params).
   *
   * Validierung der Zahlung. Dafür werden die von mbe4 übergebenen Parameter
   * plus das mbe4-Password hintereinander gehängt und eine MD5-Summe aus dem
   * String erstellt.
   *
   *  @param array $mbe4_params
   *        $mbe4_params: Key-Value-Array aus dem per GET übergebenen Parametern
   *
   * 	@return int
   * 	    Response-Code, 0 means everything is fine
   */
  public function validateTransaction($mbe4_params, $mbe4_password) {
    // Check hash Signierung.
    $hashbase =
        $mbe4_password .
        $mbe4_params["transactionid"] .
        $mbe4_params["clienttransactionid"] .
        $mbe4_params["responsecode"] .
        $mbe4_params["description"] .
        $mbe4_params["subscriberid"] .
        $mbe4_params["operatorid"] .
        $mbe4_params["timestamp"];
    if (md5($hashbase) != $mbe4_params["hash"]) {
      // Parameter nicht korrekt oder manipuliert.
      return 999;
    }
    // Transaktionscode zurückgeben.
    return $mbe4_params["responsecode"];
  }

  /**
   * Is the response-code valid.
   */
  public function isValidResponsecode($responsecode) {
    if ($responsecode == 0) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Get the Response-Message by Responsecode.
   */
  public function getResponsemsgByResponsecode($responsecode) {
    switch ($responsecode) {
      case 0: return ('OK');
      case 1: return ('NOT FINAL – request was processed successfully but the answer is not final. (e.g. a TAN was sent to the subscriber and tan received must be called)');
      case 2: return ('authorization failed');
      case 3: return ('capture failed');
      case 4: return ('terminate failed');
      case 5: return ('refund failed');
      case 6: return ('prepair failed');
      case 7: return ('transaction failed');
      case 8: return ('subscription terminate failed');
      case 101: return ('invalid parameter');
      case 109: return ('transaction in wrong status');
      case 110: return ('wrong PIN');
      case 111: return ('too many PIN attempts - transaction closed');
      case 112: return ('subscriber aborted transaction');
      case 113: return ('no route to operator');
      case 121: return ('subscriberid unascertainable');
      case 126: return ('sending TAN SMS failed');
      case 150: return ('subscriptionid unknown');
      case 151: return ('subscriptionid not unique');
      case 152: return ('subscription terminated');
      case 200: return ('internal server error');
      case 999: return ('hash cant be verified');
      case 201: return ('system currently unavailable');
      default: return (900);
    }
  }
}
