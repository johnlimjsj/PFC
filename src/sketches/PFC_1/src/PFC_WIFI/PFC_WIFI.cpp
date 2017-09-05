
#include "PFC_WIFI.h"

WiFiClient client2;
HttpClient http2(client2);
PFC_WIFI pfcwifi;

bool PFC_WIFI::connectWiFi(String ssid, String password){
	delay(10);
	WiFi.mode(WIFI_STA);

  // Converting string to char array
  char _ssid[ssid.length()];
  ssid.toCharArray(_ssid, ssid.length() + 1);

  char _password[password.length()];
  password.toCharArray(_password, password.length() + 1);

	WiFi.begin(_ssid, _password);
  Serial.print("Trying to connect to ");
  Serial.print(_ssid);
  Serial.println();
	// do not proceed until connected to wifi
	int i = 0;
	while (WiFi.status() != WL_CONNECTED && i*NETWORK_DELAY < NETWORK_TIMEOUT) {
  	delay(NETWORK_DELAY); 
  	Serial.print(".");
  	i ++;
	}

	if (WiFi.status() == WL_CONNECTED) {
		Serial.println("\nWiFi connected: " + ssid);
		return true;
	}
	else {
		Serial.println("\nNetwork Timeout");
		return false;
	}  
  	return true;  
}

void PFC_WIFI::initPeriodicSettings(PeriodicSettings cs){
  _period = cs.period;
  _timeout = cs.timeout;
}

void PFC_WIFI::initPathSettings(PathSettings ps, String user, String apiKey){

  // Generating path for performing updates
  _pathUpdate = ps.rootUpdate + user + "/" + apiKey + "/";

  // Generating path for getting settings from server
  _pathGetSettings = ps.rootGetSettings + user + "/" + apiKey + "/";

  // Generating path for manual override settings
  _pathGetManualSettings = ps.rootGetManualSettings + user + "/" + apiKey + "/";

}






bool PFC_WIFI::scanAndSelectNetwork(){
  
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

String* PFC_WIFI::getAllNetworks(int &n){          
  n = WiFi.scanNetworks();
  String* wifiNetworks = new String[n];
  
  for (int i = 0; i < n; i++){
    wifiNetworks[i] = WiFi.SSID(i);
  }
  return wifiNetworks;
}


int PFC_WIFI::getRequest(String url){
  char kPath[url.length()];
  url.toCharArray(kPath, url.length());
  int err = http2.get(_server, kPath); 
  return err;     
}

int PFC_WIFI::getRequestWithStop(String url){
  char kPath[url.length()];
  url.toCharArray(kPath, url.length());
  int err = http2.get(_server, kPath); 
  http2.stop();
  return err;     
}

void PFC_WIFI::sendDataToServer(String url){
  delay(500);
  if (client2.connect(_server,80)) {
    Serial.println("connected to " + url);   
    client2.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + SERVER + "\r\n" + "Connection: close\r\n\r\n"); // keep-alive    
  }
  unsigned long timeout = millis();
  while (client2.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client2.stop();
      return;
    }
  }
}


String PFC_WIFI::getJsonFromServer(String url){
  sendDataToServer(url);
  String jsonResponse = "";
  String section="header";
  
  while(client2.available()){
    String line = client2.readStringUntil('\r');
    if(line.substring(1,3) == "{\""){
      String result = line.substring(1);
      jsonResponse += result;
    }
  }

  Serial.println(jsonResponse);
  return jsonResponse;
}


void PFC_WIFI::sendPeriodicRequest(String getStr){
	unsigned long currTime = millis();
  Serial.println("****** Checking whether to send Periodic request");
  
  // check if within time interval
  if(currTime - _prevTime < _period){
    
    if (currTime - _prevTime < _timeout && !_currentAttemptSuccess ){
      
      // attempt to send. if it was successful, then do not send again
      int err = getRequest(getStr);
      Serial.println(err);
      if (err == 0){
        err = http2.responseStatusCode();
        if (err == 200) { 
          Serial.println("success");
          _currentAttemptSuccess = true; 
        }

      }
      http2.stop();
    }
    else{
      // do not send
      Serial.println("not sending");
    }
  }
  else {
    _currentAttemptSuccess = false;
    _prevTime += _period;
  }
}


