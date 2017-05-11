#include <ESP8266WiFi.h>  // the ESP8266WiFi.h  lib
#include <ESP8266HTTPClient.h>

// strategy here is to use HTTPClient web access thru http proxy 192.168.3.34 to Nexmo account
const char* SSID = "yourwifissid";
const char* PASS = "yourwifipass";
char server[] = "mail.smtpcorp.com";     // smtp2go.com account, free for limited usage

WiFiClient client;

void setup()
{
  Serial.begin(115200);
  delay(10);
  Serial.println("");
  Serial.println("");
  Serial.print("Connecting");
  Serial.println(SSID);
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.print("IPess: ");
  Serial.println(WiFi.localIP());
  pinMode(2,INPUT);
  // byte ret = sendEmail();
}

void loop()
{
  HTTPClient http;
  int httpCode;

  // Fortunately this is active LOW when water detected  
  if(digitalRead(2) == 0) {
    // this is my local http to https apache proxy
    http.begin("http://192.168.3.34/sms/xml?api_key=YYYYYYYY&api_secret=XXXXXXXXXXXXXXXX&from=1ZZZZZZZZZZ&to=1WWWWWWWWWW&text=Water%20alert");
    httpCode = http.GET();
    if(httpCode > 0) {
      if(httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.print(payload);
      }
    } 
    for(;;)
        delay(5000);    // just delay forever now until reset to prevent repeat texts
  }
  // check connectivity and reset on fail since this is a critical monitor
  // 'v' is just a file with a single '.'
  http.begin("http://192.168.3.34/v"); //HTTP
  httpCode = http.GET();
  if(httpCode > 0) {
    if(httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.print(payload);
    }
  } else {
    ESP.restart();
  }
  delay(5000);        // check every 5 seconds
}

