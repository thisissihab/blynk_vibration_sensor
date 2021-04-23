#define BLYNK_PRINT DebugSerial

#include <SoftwareSerial.h>
SoftwareSerial DebugSerial(2, 3); 

#include <BlynkSimpleStream.h>
BlynkTimer timer;
#include <SimpleTimer.h>

//SimpleTimer timer;

char auth[] = "2qsKRZPCv9g1IRia-D2jLdf-7Jta-rTc";  // You will get this on your email used to sign up on Blynk
                                                   // this key is required to keep your project safe so that others can not access it.

const int sensorPin = 9;  // the DO pin of the SW420 would be this pin
const int LEDPin = V0;   // pin number in Blynk and in this code need to be the same
const int terminalPin = V1;
const int gaugePin = V2;
const int ledYellow = 13; // this is the physical led you want that to light up when the vibration is higher than usual

String dataString = "";

WidgetLED led1(LEDPin); // the LED is under the Widget class on Blynk Library this is why we need to create a instance of 
                        // widget library to use the LED

void setup() {
  pinMode(sensorPin, INPUT);
  pinMode(ledYellow, OUTPUT);
  DebugSerial.begin(9600);
  Serial.begin(9600);
  Blynk.begin(Serial, auth);
  timer.setInterval(100,readSensor);    //we are using arduino timer. It calls readSensor() dunction on every 100 mili sec interval. 
                                        // which means the arduino reads the vibration sensor 10 times within a second.
}                                       // if you change it to 1000 then you will get data every 1 sec interval

void loop() {
  Blynk.run();
  timer.run();         // we need to use timer when we want to multitask. we have to read sensor and execute Blynk.run() at a time
                       // this is why we are using arduino internal timer
}

void readSensor(){
  long sensorValue = pulseIn(sensorPin,HIGH)/80;  //Change the divission factor according to your output
  //Serial.println(sensorValue);              // If you want bigger output then decrease the division factor
  dataString += "Sensor Reading :" + (String)sensorValue +"\n";
  Blynk.virtualWrite(gaugePin, sensorValue);     // this line writes sensor value to the gauge meter
  Blynk.virtualWrite(terminalPin, dataString);   // this line writes the data to the terminal on the app
  
  if(sensorValue > 900){          // Change the value 900 according to your need
    led1.on();        // this line turns the virtual led on the app on when unnatural/higher movement detected
    digitalWrite(ledYellow, HIGH);  // the physical yellow led lights up
    delay(1000);
    Blynk.notify("Alert!! High Movement Detected"); // this line shows the popup message when higher movement detected
    delay(1000);
  }
  
  else{
    digitalWrite(ledYellow, LOW);       // if movement is natural it gets turned off
    led1.off();  // if there is no unnatural/higher movement then the LED on the app goes down
  }
  dataString = "";
  //Serial.println(sensorValue);
 }
