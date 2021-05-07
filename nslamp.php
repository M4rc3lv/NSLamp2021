<?php

 date_default_timezone_set("Europe/Amsterdam");
 $d = getdate();
 $Uur = $d['hours'];
 if($Uur>18 || $Uur<5) {
  // 's Avonds hoef ik de status niet te weten
  // Ik doe dan ook geen call naar de NS om API-calls te beperken
  Uit();
 }

 // Hardcodeed: HRT voor station Horst-Sevenum
 $url = "https://gateway.apiportal.ns.nl/reisinformatie-api/api/v2/departures?station=HRT";
 $client = curl_init($url);
 curl_setopt($client,CURLOPT_RETURNTRANSFER,true);

 curl_setopt($client, CURLOPT_HTTPHEADER, array(
  "Ocp-Apim-Subscription-Key: API-KEYvanDeNS"
 ));

 $response = curl_exec($client);
 $result = json_decode($response);

 $len = count($result->payload->departures);

 if($len<=0) Rood("Geen vertrekkende treinen");
 for($i=0; $i<$len; $i++) {
  if($result->payload->departures[$i]->cancelled) Rood("Geannuleerde trein");
  if($result->payload->departures[$i]->plannedDateTime !== $result->payload->departures[$i]->actualDateTime) Rood("Vertraging");
 }
 Groen();
 // OK


 function Uit() {
  echo "UIT";
  die();
 }

 function Rood($Reden) {
  echo "ROOD\n$Reden";
  die();
 }

 function Groen() {
  echo "GROEN";
  die();
 }
?>





