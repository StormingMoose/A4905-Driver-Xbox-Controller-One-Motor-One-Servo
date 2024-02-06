 /*
   Adapted from the following:
   
   ESP32 Web Server Demo using Accesspoint
   https://circuits4you.com
   21-11-2018

   Many thanks to Rui Santos and his crew over at randomnerdturtorials.com
*/

#include <ESP32Servo.h>   //  servo stuff
Servo myservo;  // create servo object to control steering

static const int servoPin = 19;      

int pos = 90; // intitial servo postion to be at 90 degrees to make it straight different from 90 for mechanical adjustment

int MOTOR_PWM_A = 26;//  Speed  
int MOTOR_DIR_A = 27; // 

#define dir_Forward 0
#define dir_Back 255
#define PWM_STOP 0

double PWM_SPEED = 0; // INTIAL PWM duty cycle

const int freq = 909;  // 1.1ms per bit
const int resolution = 8;
const int ledChannel_PWM_A = 10;
const int ledChannel_Dir_A = 11;

#include <WiFi.h>
#include <WebServer.h>
#include "index.h"  //Web page header file

//Enter your desired WiFi SSID and PASSWORD
const char* ssid = "your home router";
const char* password = "its_password";  // change the *'s to your favourite vegetable

WebServer server(80);

//===============================================================
// Setup
//===============================================================
void setup(void) {

   //Serial.begin(115200);
 
  pinMode(servoPin, OUTPUT);

  myservo.attach(servoPin); 

  myservo.write(pos);

  pinMode(MOTOR_PWM_A, OUTPUT);
  pinMode(MOTOR_DIR_A, OUTPUT);

  ledcSetup(ledChannel_PWM_A, freq, resolution);
  ledcSetup(ledChannel_Dir_A, freq, resolution);
 
  ledcAttachPin(MOTOR_PWM_A, ledChannel_PWM_A);
  ledcAttachPin(MOTOR_DIR_A, ledChannel_Dir_A);

  ledcWrite( ledChannel_Dir_A, dir_Forward);
  ledcWrite( ledChannel_PWM_A, PWM_STOP );

  WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid, password);    //Password length minimum 8 char

  server.on("/", handle_OnConnect);
  server.on("/SPEED", handle_Set_Speed);
  server.on("/A", handle_Forward);
  server.on("/B", handle_Back);
  server.on("/off", handle_Stop);
  server.on("/STEER", handle_Steer);
 
  server.onNotFound(handle_NotFound);

  server.begin();                  //Start server
  }

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void loop(void) {
  server.handleClient();
}

void handle_NotFound () {
}

void handle_OnConnect() {
  String s = MAIN_page; //Read HTML contents
  server.send(200, "text/html", s); //Send web page
}

void handle_Set_Speed() {
  if(server.arg("value") ){PWM_SPEED = server.arg("value").toInt(); }
  server.send(200, "text/plain", "ok");
}

void handle_Forward() {
  ledcWrite( ledChannel_Dir_A, dir_Forward ); // direction = forward
  ledcWrite( ledChannel_PWM_A, PWM_SPEED ); // PWM speed = fast
  server.send(200, "text/plain", "ok");
}

void handle_Back() {
  ledcWrite( ledChannel_Dir_A, dir_Back ); // direction = forward
  ledcWrite( ledChannel_PWM_A, 255-PWM_SPEED );   // see note in the handle_Stop routine below.
  server.send(200, "text/plain", "ok");
}

void handle_Stop() {
  ledcWrite( ledChannel_Dir_A, dir_Forward );    // odd thing about these drivers, the stop function means stop when going forward, if in reverse it means go as fast as you can.
  ledcWrite( ledChannel_PWM_A, PWM_STOP );       // Make sure your direction is forward when you want to stop.
  server.send(200, "text/plain", "ok");
}

void handle_Steer() { // Receive an HTTP GET request for steering.  Depending on the position of sg90 servo could change name to Lefter
  if(server.arg("value") ){pos = server.arg("value").toInt(); }
  myservo.write(pos);
  server.send(200, "text/plain", "ok");
}

