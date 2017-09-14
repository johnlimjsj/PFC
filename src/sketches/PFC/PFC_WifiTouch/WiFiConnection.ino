//=============
// These are known to work. Keep until proven that the PFC_WIFI.cpp also works
//=============

/* ===== WORKS ====
bool connectWiFi(WiFiAP wifiap){
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiap.ssid, wifiap.password);
  // do not proceed until connected to wifi
  int i = 0;
  while (WiFi.status() != WL_CONNECTED && i*NETWORK_DELAY < NETWORK_TIMEOUT) {
    delay(NETWORK_DELAY); 
    Serial.print(".");
    i ++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected");
    return true;
  }
  else {
    Serial.println("\nNetwork Timeout");
    return false;
  }  
}
*/


/*
bool scanAndSelectNetwork(){
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
    }
  }
  Serial.println("");
}
*/

/*
int getRequest(String url){
  char kPath[url.length()];
  url.toCharArray(kPath, url.length());
  int err = http.get(SERVER, kPath); 
  Serial.print("Error code is");
  Serial.println(http.responseStatusCode());
  http.stop();
  return err;     
}
*/

/*
void sendDataToServer(String url){
  delay(500);
  if (client.connect(SERVER,80)) {
    PRINTLN("Success: connected to " + url);   
    client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + SERVER + "\r\n" + "Connection: close\r\n\r\n"); // keep-alive    
  }
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
}
*/

/*
String getJsonFromServer(String url){
  sendDataToServer(url);
  String jsonResponse = "";
  String section="header";
  
  while(client.available()){
    String line = client.readStringUntil('\r');
    if(line.substring(1,3) == "{\""){
      String result = line.substring(1);
      jsonResponse += result;
    }
  }
  Serial.println(jsonResponse);
  return jsonResponse;
}
*/

/*
ClientSettings sendRequestPerTimeInterval(ClientSettings cs, String getStr){
  unsigned long currTime = millis();
  
  // check if within time interval
  if(currTime - cs.prevTime < cs.period){
    
    if (currTime - cs.prevTime < cs.timeout && !cs.currentAttemptSuccess ){
      
      // attempt to send. if it was successful, then do not send again
      int err = getRequest(getStr);
      if (err == 0){
        err = http.responseStatusCode();
        if (err == 200) { 
          Serial.println("success");
          cs.currentAttemptSuccess = true; 
        }
      }
      http.stop();
    }
    else{
      // do not send
      Serial.println("not sending");
    }
  }
  else {
    cs.currentAttemptSuccess = false;
    cs.prevTime += cs.period;
  }
  return cs;
}
*/



/*
String getResponse(String url){
  String response = "";
  char kPath[url.length()];
  url.toCharArray(kPath, url.length());
  int err = http.get(SERVER, kPath); 
  if (err == 0)
  {
    err = http.responseStatusCode();
    PRINTLN(err);
    if (err >= 0)
    {
      PRINTLN(err);
      err = http.skipResponseHeaders();
      if (err >= 0)
      {
        int bodyLen = http.contentLength();
        unsigned long timeoutStart = millis();
        while ( (http.connected() || http.available()) && ((millis() - timeoutStart) < NETWORK_TIMEOUT) )
        {
            if (http.available())
            {
                response += http.read();
                PRINTLN("response:");
                PRINTLN(response);             
                bodyLen--;
                // We read something, reset the timeout counter
                timeoutStart = millis();
            }
            else
            {
                // We haven't got any data, so let's pause to allow some to arrive
                delay(NETWORK_DELAY);
            }
        }
      }
    }
  }
//  String response = "";
//  if (http.skipResponseHeaders() >= 0){
//    while(http.available()){ response += http.read(); }
//  }
  
  
  return response;
}
*/



