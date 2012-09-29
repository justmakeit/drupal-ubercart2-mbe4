<?php

class mbe4 {
//   var $mbe4_username;
//   var $mbe4_password; 
//   var $mbe4_clientid;
//   var $mbe4_serviceid;
//   var $mbe4_contentclass;
//   var $mbe4_url;
  
  function __construct($username, $password, $clientid, $serviceid, $url="https://billing.mbe4.de/widget/singlepayment") {
      $this->username = $username;
      $this->password = $password;
      $this->clientid = $clientid;
      $this->serviceid = $serviceid;
      $this->url = $url;
  }

  /*
  *  Senden der Daten an mbe4.
  *  params: 
  *   $amount: Die Transaktionssumme in EUR
  *   $contentid: Die Art des zu Buchenden Contents.
  *		1: News/Info
  *		2. Chat/Flirt
  *		3. Game
  *		4. Klingelton
  *		5. Bild/Logo
  *		6. Videoclip
  *		7. Musikdatei
  *		8. Lokalisierung
  *		9. Voting
  *		10. Gewinnspiel
  *		11. Portal Zugang
  *		12. Software
  *		13. Dokument
  *		14. Ticket
  *		15. Horoskop
  *		16. Freizeit
  *		17. Unterwegs
  *		18. Finanzen
  *		19. Shopping
  *		20. E-Mail
  *		21. Spende
  *  return:
  *	Liefert ein Key/Value-Array zurück, welches per GET an mbe4 übertragen werden muss.
  *
  */
  function create_transaction($id,$description="mbe4 payment", $amount, $contentclass=1, $returnurl,$urlencode=TRUE){  
  // Timestamp generieren
  $timestamp=date("Y-m-d")."T".date("H:i:s.000")."Z";
  // Hashbase definieren
  $hashbase=
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
  // hash erzeugen
  $hashparam=md5($hashbase);
  // Build the data array that will be translated into hidden form values.
  $data = array(
    // General parameters
    'username' =>$this->username,
    'clientid' => $this->clientid,
    'serviceid' => $this->serviceid,
    'contentclass' => $contentclass,
    'description' => $description,
    'clienttransactionid' => $id,
    'amount' => $amount, // mbe4 wants ct, no eur
    'callbackurl' => $returnurl,
    'timestamp' => $timestamp,
    'hash' => $hashparam,
  );
  // Sollen die Werte mit urlencode() codiert werden?
  if($urlencode==TRUE){
      foreach($data as $element){
	  $element= urlencode($element);
      }
  }
  return $data;
  }
  
  function validate_transaction($data) {
    
  }
  
}
?>