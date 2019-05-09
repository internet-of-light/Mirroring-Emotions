#include <ESP8266WiFi.h>
#include "Wire.h"          
#include <PubSubClient.h>  
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

#define ssid "University of Washington"   // Wifi network SSID
#define password ""                       // Wifi network password

#define NUM_LIGHTS 4
#define SERIAL_BAUD_RATE 115200 

//#define BRIDGE "Lab Green"
//#define BRIDGE "Lab Blue"
//#define BRIDGE "Lab Red"
#define BRIDGE "Sieg Master"

const int button1Pin = 12;
const int button2Pin = 13;
const int button3Pin = 15;

int button1State = 0;
int button2State = 0;
int button3State = 0;

String ip; //Hue Bridge IP Address
String api_token; //Hue Bridge Authentication api_token


void changeGroup(byte groupNum, byte transitiontime, String parameter, String newValue, String parameter2 = "",
                 String newValue2 = "", String parameter3 = "", String newValue3 = "") {

  String req_string = "http://" + ip + "/api/" + api_token + "/groups/" + groupNum + "/action";
  HTTPClient http;
  http.begin(req_string);

  String put_string = "{\"" + parameter + "\":" + newValue + ", \"transitiontime\": " +
                      transitiontime;
  if (!parameter2.equals("")) put_string += + ", \"" + parameter2 + "\": " + newValue2;
  if (!parameter3.equals("")) put_string += ", \"" + parameter3 + "\" : " + newValue3;
  put_string +=  + "}";

  Serial.println("Attempting PUT: " + put_string + " for GROUP: " + String(groupNum));

  int httpResponseCode = http.PUT(put_string);
  if (httpResponseCode == 200) {
    String response = http.getString();
    Serial.println("Response code: " + httpResponseCode);
    Serial.println("Response: " + response);
  } else {
    Serial.print("Error on sending PUT Request: ");
    Serial.println(String(httpResponseCode));
  }
  http.end();
}

void changeLight(byte lightNum, byte transitiontime, String parameter, String newValue, String parameter2 = "",
                 String newValue2 = "", String parameter3 = "", String newValue3 = "") {

  String req_string = "http://" + ip + "/api/" + api_token + "/lights/" + lightNum + "/state";
  HTTPClient http;
  http.begin(req_string);

  String put_string = "{\"" + parameter + "\":" + newValue + ", \"transitiontime\":" + transitiontime;
  if (!parameter2.equals("")) put_string += + ", \"" + parameter2 + "\": " + newValue2;
  if (!parameter3.equals("")) put_string += ", \"" + parameter3 + "\" : " + newValue3;
  put_string +=  + "}";

  Serial.println("Attempting PUT: " + put_string + " for LIGHT: " + String(lightNum));


  int httpResponseCode = http.PUT(put_string);
  if (httpResponseCode == 200) {
    String response = http.getString();
    Serial.println("Response code: " + httpResponseCode);
    Serial.println("Response: " + response);
  } else {
    Serial.println("Error on sending PUT Request: ");
    Serial.println(String(httpResponseCode));
  }
  http.end();
}

/////SETUP_WIFI/////
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");  //get the unique MAC address to use as MQTT client ID, a 'truly' unique ID.
  Serial.println(WiFi.macAddress());  //.macAddress returns a byte array 6 bytes representing the MAC address
} 

void setup() {
   if (BRIDGE == "Lab Green") {
    ip = "172.28.219.225"; //Lab Green
    api_token = "lxjNgzhDhd0X-qhgM4lsgakORvWFZPKK70pE0Fja"; //Lab Green
  }
  else if (BRIDGE == "Lab Blue") {
    ip = "172.28.219.235"; //Lab Blue
    api_token = "qn41nLuAOgrvfOAPeNQCYB6qannsoS8uDtyBJtMc"; //Lab Blue
  }
  else if (BRIDGE == "Lab Red") {
    ip = "172.28.219.189"; //Lab Red
    api_token = "Lht3HgITYDN-96UYm5mkJ4CEjKj20d3siYidcSq-"; //Lab Red
  }
  else if (BRIDGE == "Sieg Master") {
    ip = "172.28.219.179"; //Sieg Master
    api_token = "rARKEpLebwXuW01cNVvQbnDEkd2bd56Nj-hpTETB"; //Sieg Master
  }

  
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);
  pinMode(button3Pin, INPUT);

  setup_wifi();
 
}

void loop() {
  // put your main code here, to run repeatedly:
  button1();
  button2();
  button3();
}

//Happy _ yellow & orange
void button1() {
  button1State = digitalRead(button1Pin);
  //Serial.println(button1State);
  if (button1State == HIGH) {
    changeGroup(3, 3, "on", "true", "hue", "60000", "bri", "150");
    changeGroup(4, 3, "on", "true", "hue", "50000", "bri", "150");
    delay(500);
  } else if(button1State == LOW) {
    
  }
}

//Okay _ green_orange_white
void button2() {
  button2State = digitalRead(button2Pin);
  //Serial.println(button2State);
  if (button2State == HIGH) {
    changeGroup(3, 3, "on", "true", "hue", "40000", "bri", "150");
    changeGroup(4, 3, "on", "true", "hue", "30000", "bri", "150");
    delay(500);
  } else if(button2State == LOW) {
    
  }
}

//Sad _ Blue_purple
void button3() {
  button3State = digitalRead(button3Pin);
  //Serial.println(button3State);
  if (button3State == HIGH) {
    changeGroup(3, 3, "on", "true", "hue", "20000", "bri", "150");
    changeGroup(4, 3, "on", "true", "hue", "5000", "bri", "150");
    delay(500);
  } else if(button3State == LOW) {
    
  }
}

//only for blue hub
//group id 3 is bulbs 1 & 3
//group id 4 is bulbs 2 & 4



