#include <SmartThingsESP32WiFi.h>
#include "WiFiCredentials.h"

//******************************************************************************************
// ST_Anything Library 
//******************************************************************************************
#include <Constants.h>       //Constants.h is designed to be modified by the end user to adjust behavior of the ST_Anything library
#include <Device.h>          //Generic Device Class, inherited by Sensor and Executor classes
#include <Sensor.h>          //Generic Sensor Class, typically provides data to ST Cloud (e.g. Temperature, Motion, etc...)
#include <Executor.h>        //Generic Executor Class, typically receives data from ST Cloud (e.g. Switch)
#include <InterruptSensor.h> //Generic Interrupt "Sensor" Class, waits for change of state on digital input 
#include <Everything.h>      //Master Brain of ST_Anything library that ties everything together and performs ST Shield communications

#include <S_UltrasonicPresence.h>

//******************************************************************************************
//Define which Arduino Pins will be used for each device
//******************************************************************************************
#define PIN_ULTRASONIC_TRIG  13  //digital output to trigger ultrasonic
#define PIN_ULTRASONIC_ECHO  12  //digital input to read the echo

//******************************************************************************************
//ESP32 WiFi Information
//******************************************************************************************

void callbackSent(const String &msg)
{
  String strTemp = msg;
  Serial.print(F("Send: "));
  Serial.println(msg);
}

void callbackRcvd(const String &msg)
{
  String strTemp = msg;
  Serial.print(F("Rcvd: "));
  Serial.println(msg);
}

//******************************************************************************************
//Arduino Setup() routine
//******************************************************************************************
void setup()
{
  //******************************************************************************************
  //Declare each Device that is attached to the Arduino
  //******************************************************************************************

  //Sensors
  bool initalState = false; // Start off not-present
  bool longIsPresent = false; // Long distance means not-present
  unsigned long longThreshold = 10656; // 6 feet = 6*12*148 = 10656
  unsigned long shortThreshold = 7104; // 4 feet = 4*12*148 = 7104 
  static st::S_UltrasonicPresence sensor1(F("presence1"),
    PIN_ULTRASONIC_TRIG, PIN_ULTRASONIC_ECHO,
    initalState, longIsPresent, longThreshold, shortThreshold);
  
  //*****************************************************************************
  //  Configure debug print output from each main class 
  //  -Note: Set these to "false" if using Hardware Serial on pins 0 & 1
  //         to prevent communication conflicts with the ST Shield communications
  //*****************************************************************************
  st::Everything::debug=true;
  st::Device::debug=true;

  //*****************************************************************************
  //Initialize the "Everything" Class
  //*****************************************************************************

  //Initialize the optional local callback routine (safe to comment out if not desired)
  st::Everything::callOnMsgSend = callbackSent;
  st::Everything::callOnMsgRcvd = callbackRcvd;
  
  //Create the SmartThings ESP32WiFi Communications Object
  //STATIC IP Assignment - Recommended
  //st::Everything::SmartThing = new st::SmartThingsESP32WiFi(str_ssid, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, st::receiveSmartString);
 
  //DHCP IP Assigment - Must set your router's DHCP server to provice a static IP address for this device's MAC address
  st::Everything::SmartThing = new st::SmartThingsESP32WiFi(str_ssid, str_password, serverPort, hubIp, hubPort, st::receiveSmartString);

  //Run the Everything class' init() routine which establishes WiFi communications with SmartThings Hub
  st::Everything::init();
  
  //*****************************************************************************
  //Add each sensor to the "Everything" Class
  //*****************************************************************************
  st::Everything::addSensor(&sensor1);
      
  //*****************************************************************************
  //Initialize each of the devices which were added to the Everything Class
  //*****************************************************************************
  st::Everything::initDevices();

  //*****************************************************************************
  //Enable serial monitoring. Uncomment to use.
  //*****************************************************************************
  Serial.begin(115200);
}

//******************************************************************************************
//Arduino Loop() routine
//******************************************************************************************
void loop()
{
  //*****************************************************************************
  //Execute the Everything run method which takes care of "Everything"
  //*****************************************************************************
  st::Everything::run();
}
