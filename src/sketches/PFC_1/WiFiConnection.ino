
void connectWiFi(WifiAP wifiap){
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiap.ssid, wifiap.password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  ThingSpeak.begin(client);
}

int getRequest(String url){
  char kPath[url.length()];
  url.toCharArray(kPath, url.length());
  int err = http.get(SERVER, kPath); 
  return err;     
}

void sendDataToServer(String url){
  delay(500);
  if (client.connect(SERVER,80)) {
    PRINTLN("connected to " + url);   
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

  return jsonResponse;
}

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



