
void connectWiFi(const char* ssid, const char* password){
  Serial.begin(9600);
  delay(10);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  ThingSpeak.begin(client);
}

void sendDataToServer(String postStr){
  if (client.connect(SERVER,80)) {
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    Serial.println(postStr);
  }
  client.stop();
  Serial.println("Waiting…");
  delay(2000);
}

String getCommandFromServer(){
  
  return "command";
}

