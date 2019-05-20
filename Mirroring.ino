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
#define BRIDGE "Lab Red"
//#define BRIDGE "Sieg Master"

const int button1Pin = 14;
const int button2Pin = 12;
const int button3Pin = 13;
const int button4Pin = 15;

int countButton1 = 0;
int countButton2 = 0;
int countButton3 = 0;
int countButton4 = 0;

int button1State = 0;
int button2State = 0;
int button3State = 0;
int button4State = 0;

int emotion = 1;
int count = countButton1;

String ip; //Hue Bridge IP Address
String api_token; //Hue Bridge Authentication api_token


void changeGroup(byte groupNum, byte transitiontime, String parameter, String newValue, String parameter2 = "",
                 String newValue2 = "", String parameter3 = "", String newValue3 = "",String parameter4 = "", String newValue4 = "") {

  String req_string = "http://" + ip + "/api/" + api_token + "/groups/" + groupNum + "/action";
  HTTPClient http;
  http.begin(req_string);

  String put_string = "{\"" + parameter + "\":" + newValue + ", \"transitiontime\": " +
                      transitiontime;
  if (!parameter2.equals("")) put_string += + ", \"" + parameter2 + "\": " + newValue2;
  if (!parameter3.equals("")) put_string += + ", \"" + parameter3 + "\": " + newValue3;
  if (!parameter4.equals("")) put_string += ", \"" + parameter4 + "\" : " + newValue4;
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
  pinMode(button4Pin, INPUT);

  setup_wifi();

  changeGroup(4, 3, "on", "true", "hue", "0", "bri", "100", "sat", "0");
 
}

void loop() {
  // put your main code here, to run repeatedly:
  button1();
  button2();
  button3();
  button4();

  getEmotion();
  visualize();

//  Serial.print("button 1 = ");
//  Serial.println(countButton1);
//
//  Serial.print("button 2 = ");
//  Serial.println(countButton2);
//
//  Serial.print("button 3 = ");
//  Serial.println(countButton3);
//
//  Serial.print("button 4 = ");
//  Serial.println(countButton4);
}

//Happy _ yellow & orange
void button1() {
  button1State = digitalRead(button1Pin);
  //Serial.println(button1State);
  if (button1State == HIGH) {
    countButton1 += 1;
    delay(500);
  } else if(button1State == LOW) {
    
  }
}

//Okay _ green_orange_white
void button2() {
  button2State = digitalRead(button2Pin);
  //Serial.println(button2State);
  if (button2State == HIGH) {
    countButton2 += 1;
    delay(500);
  } else if(button2State == LOW) {
    
  }
}

//Sad _ Blue_purple
void button3() {
  button3State = digitalRead(button3Pin);
  //Serial.println(button3State);
  if (button3State == HIGH) {
    countButton3 += 1;
    delay(500);
  } else if(button3State == LOW) {
    
  }
}

//Mad_ Red_Orange
void button4() {
  button4State = digitalRead(button4Pin);
  //Serial.println(button4State);
  if (button4State == HIGH) {
    countButton4 += 1;
    delay(500);
  } else if(button4State == LOW) {
    
  }
}

void getEmotion() {
  if(count < countButton2){
    count = countButton2;
    emotion = 2;
  }
  if(count < countButton3){
    count = countButton3;
    emotion = 3;
  }
  if(count < countButton4){
    count = countButton4;
    emotion = 4;
  }  
  Serial.print(count);
  Serial.print(" presses for button: ");
  Serial.println(emotion);
}

void visualize () {
  if(emotion == 1){ //Happy
    changeGroup(4, 3, "on", "true", "hue", "50000", "bri", "150", "sat", "150");
    changeGroup(5, 3, "on", "true", "hue", "5000", "bri", "150", "sat", "150");
  }
  if(emotion == 2){ //Okay
    changeGroup(4, 3, "on", "true", "hue", "40000", "bri", "150", "sat", "150");
    changeGroup(5, 3, "on", "true", "hue", "20000", "bri", "150", "sat", "150");
  }
  if(emotion == 3){ //Sad
    changeGroup(4, 3, "on", "true", "hue", "10000", "bri", "150", "sat", "150");
    changeGroup(5, 3, "on", "true", "hue", "50000", "bri", "150", "sat", "150");
  }
 if(emotion == 4){ //Mad
    changeGroup(4, 3, "on", "true", "hue", "30000", "bri", "150", "sat", "150");
    changeGroup(5, 3, "on", "true", "hue", "50000", "bri", "150", "sat", "150");
  }
}




