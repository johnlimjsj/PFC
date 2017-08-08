#ifndef PFC_WIFI_h
#define PFC_WIFI_h

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <HttpClient.h>
#include "../PFC_SETTINGS/PFC_SETTINGS.h"


class PFC_WIFI 
{

    public:   

        PFC_WIFI(){};
        void initPeriodicSettings(PeriodicSettings cs);

        void initPathSettings(PathSettings ps, String user, String apiKey);

        String getPathUpdate(){ return _pathUpdate; }
        String getPathManualSettings(){ return _pathGetManualSettings; }
        String getPathSettings(){ return _pathGetSettings; }

        bool connectWiFi(String ssid, String password);
        bool scanAndSelectNetwork();
        
        int getRequest(String url);
        int getRequestWithStop(String url);

    	void sendDataToServer(String url);
    	String getJsonFromServer(String url);

	   void sendPeriodicRequest(String getStr);

    private:
    // Client Settings such as server and path
        char* _server = "www.lifeplanters.com";
        String _pathUpdate;
        String _pathGetSettings;
        String _pathGetManualSettings;

    // Periodic Settings Stuff
    	unsigned long _period;
    	unsigned int _timeout;
    	unsigned long _prevTime = 0;
    	bool _currentAttemptSuccess = false;
    	bool _prevAttemptSuccess = false;

    
};

#endif