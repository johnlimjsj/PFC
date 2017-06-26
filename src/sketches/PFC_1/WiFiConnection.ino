
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




