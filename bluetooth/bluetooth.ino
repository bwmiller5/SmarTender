/*
 * Bluetooth Test - Arduino Side
 * Arduino to HM10 module to Google Chrome
 * https://www.amazon.com/gp/product/B06WGZB2N4/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&psc=1
 *
 *
 * Example Link: https://www.youtube.com/watch?v=w_mRj5IlVpg
 *
 * Twitter: https://twitter.com/KDcircuits
 * For inquiries or design services:
 * https://www.kdcircuits.com
 *
 * License?  Do whatever you want with this code - it's just a sample
 */

#include <SoftwareSerial.h>
#include <Adafruit_MotorShield.h>

// Pins  ************************
// UART TO HM10 Module
const uint8_t bluRX_ardTXpin = 3;
const uint8_t bluTX_ardRXpin = 2;

// RGB LED
const uint8_t LED_Pin = 8;
// Push Button
const uint8_t pushButtonPin = 6;

const uint8_t limitSwitchXPin = 8; // X direction limit switch
const uint8_t limitSwitchYPin = 9; // Y direction limit switch

SoftwareSerial bluetooth(bluTX_ardRXpin, bluRX_ardTXpin);

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x60);
Adafruit_MotorShield AFMS2 = Adafruit_MotorShield(0x61);
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
//Adafruit_StepperMotor *y = AFMS.getStepper(10000, 2);
//Adafruit_StepperMotor *x = AFMS.getStepper(10000, 1);
//Adafruit_StepperMotor *z = AFMS2.getStepper(10000, 2);

Adafruit_StepperMotor *y = AFMS.getStepper(10000, 2);
Adafruit_StepperMotor *x = AFMS.getStepper(10000, 1);
Adafruit_StepperMotor *y2 = AFMS2.getStepper(10000, 2);
Adafruit_StepperMotor *z = AFMS2.getStepper(10000, 1);

//Adafruit_StepperMotor *myMotor4 = AFMS2.getStepper(10000, 1);

/*const int DISPENSERS = 10;
const int** DISPENSERS_OFFSETS = {
  {1, 2},
  {1, 2},
  {1, 2},
  {1, 2},
  {1, 2},
  {1, 2},
  {1, 2},
  {1, 2},
  {1, 2},
  {1, 2},
};*/

// function prototypes
char* checkBluetooth();

void setup() {
  bluetooth.begin(9600);
  Serial.begin(9600);
  bluetooth.print("AT+NAMEPoojasArduino"); // found this here: ftp://imall.iteadstudio.com/Modules/IM130614001_Serial_Port_BLE_Module_Master_Slave_HM-10/DS_IM130614001_Serial_Port_BLE_Module_Master_Slave_HM-10.pdf
  pinMode(pushButtonPin, INPUT_PULLUP);
  pinMode(limitSwitchXPin, INPUT_PULLUP); // Enable internal pull-up
  pinMode(limitSwitchYPin, INPUT_PULLUP);

  while (!Serial);
  Serial.println("Stepper test!");

  if (!AFMS.begin()) { // create with the default frequency 1.6KHz
    // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  if (!AFMS2.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield 2. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  y->setSpeed(30);
  y2->setSpeed(30);
  x->setSpeed(40);  // 10 rpm
  z->setSpeed(100);

}

void loop() {
  if (!digitalRead(limitSwitchXPin)) { // Check if X limit switch is pressed
    Serial.println("X Limit Reached");
    x->release(); // Stop or reverse motor here
  }

  if (!digitalRead(limitSwitchYPin)) { // Check if Y limit switch is pressed
    Serial.println("Y Limit Reached");
    y->release(); // Stop or reverse motor here
  }

  char* drinkCode = checkBluetooth();

  /*if (drinkCode) {
    int x = 0, y = 0;
    for (int i = 0; i < DISPENSERS; i++) {
      int drinkAmount = drinkCode[i] - '0';
      if (drinkAmount > 0) {
        // move horizontal stepper DISPENSERS_OFFSETS[i][0] - x steps
        // move vertical stepper DISPENSERS_OFFSETS[i][1] - y steps

        // Serial.println("Begin loop");
        // myMotor1->step(300, FORWARD, SINGLE);
        // delay(2000);
        // myMotor1->step(300, BACKWARD, SINGLE);

        for (int j = 0; j < drinkAmount; j++) {
          // push up
          // come back down
        }
      }
    }
  }*/

  if (!digitalRead(pushButtonPin)) { // send out
    bluetooth.print("Push Button");
    while (!digitalRead(pushButtonPin)) {

    }
    delay(20);
    bluetooth.print(" ");
  }
}

char* checkBluetooth() {
  char charBuffer[20];
  if (bluetooth.available() > 0) {
    int numberOfBytesReceived = bluetooth.readBytesUntil('\n', charBuffer, 19);
    charBuffer[numberOfBytesReceived] = '\0';
        Serial.print("Received: ");
        Serial.println(charBuffer);

    if (strstr(charBuffer, "LED OFF") == &charBuffer[0]) {
      Serial.println("Turning LED OFF");
      analogWrite(LED_Pin, 0);
    } else if (strstr(charBuffer, "LED ON") == &charBuffer[0]) {
      analogWrite(LED_Pin, 255);
    } else if (strstr(charBuffer, "Drink1") == &charBuffer[0]) {

      /*Serial.println("Begin motor sequence for y");
      for(int i=0; i<300; i++) {
        y->step(1, FORWARD, SINGLE);
        y2->step(1, BACKWARD, SINGLE);
      }
      delay(1000);
      for(int i=0; i<300; i++) {
        y->step(1, BACKWARD, SINGLE);
        y2->step(1, FORWARD, SINGLE);
      }*/
      
      Serial.println("Begin motor sequence for x");
      x->step(1000, FORWARD, SINGLE);
      delay(1000);
     
      Serial.println("Begin motor sequence for z");
      z->step(1350, FORWARD, SINGLE);
      delay(2000);
      z->step(1350, BACKWARD, SINGLE);

      Serial.println("Begin motor sequence for x");
      x->step(1020, FORWARD, SINGLE);
      delay(1000);

      z->step(1350, FORWARD, SINGLE);
      delay(2000);
      z->step(1350, BACKWARD, SINGLE);

      x->step(2200, FORWARD, SINGLE);
      delay(1000);

      for(int i=0; i<650; i++) {
        y->step(1, BACKWARD, SINGLE);
        y2->step(1, BACKWARD, SINGLE);
      }

      z->step(1350, FORWARD, SINGLE);
      delay(2000);
      z->step(1350, BACKWARD, SINGLE);

      
    } /*else if (strstr(charBuffer, "Drink2") == &charBuffer[0]) {

      Serial.println("Begin motor sequence for y");
      y->step(300, FORWARD, SINGLE);
      delay(1000);
      y->step(300, BACKWARD, SINGLE);

      /*Serial.println("Begin motor sequence for x");
      x->step(1000, FORWARD, SINGLE);
      delay(1000);
      x->step(1000, BACKWARD, SINGLE);
     
      Serial.println("Begin motor sequence for z");
      z->step(2000, FORWARD, SINGLE);
      delay(2000);
      z->step(1700, BACKWARD, SINGLE);

      
    } else if (strstr(charBuffer, "Drink3") == &charBuffer[0]) {

      Serial.println("Begin motor sequence for y");
      y->step(300, FORWARD, SINGLE);
      delay(1000);
      y->step(300, BACKWARD, SINGLE);

      Serial.println("Begin motor sequence for x");
      x->step(1000, FORWARD, SINGLE);
      delay(1000);
      x->step(1000, BACKWARD, SINGLE);
     
      Serial.println("Begin motor sequence for z");
      z->step(2000, FORWARD, SINGLE);
      delay(2000);
      z->step(1700, BACKWARD, SINGLE);

      
    } else if (strstr(charBuffer, "Drink4") == &charBuffer[0]) {

      Serial.println("Begin motor sequence for y");
      y->step(300, FORWARD, SINGLE);
      delay(1000);
      y->step(300, BACKWARD, SINGLE);

      Serial.println("Begin motor sequence for x");
      x->step(1000, FORWARD, SINGLE);
      delay(1000);
      x->step(1000, BACKWARD, SINGLE);
     
      Serial.println("Begin motor sequence for z");
      z->step(2000, FORWARD, SINGLE);
      delay(2000);
      z->step(1700, BACKWARD, SINGLE);

      
    } else if (strstr(charBuffer, "Drink5") == &charBuffer[0]) {

      Serial.println("Begin motor sequence for y");
      y->step(300, FORWARD, SINGLE);
      delay(1000);
      y->step(300, BACKWARD, SINGLE);

      Serial.println("Begin motor sequence for x");
      x->step(1000, FORWARD, SINGLE);
      delay(1000);
      x->step(1000, BACKWARD, SINGLE);
     
      Serial.println("Begin motor sequence for z");
      z->step(2000, FORWARD, SINGLE);
      delay(2000);
      z->step(1700, BACKWARD, SINGLE);

      
    } else if (strstr(charBuffer, "Drink6") == &charBuffer[0]) {

      Serial.println("Begin motor sequence for y");
      y->step(300, FORWARD, SINGLE);
      delay(1000);
      y->step(300, BACKWARD, SINGLE);

      Serial.println("Begin motor sequence for x");
      x->step(1000, FORWARD, SINGLE);
      delay(1000);
      x->step(1000, BACKWARD, SINGLE);
     
      Serial.println("Begin motor sequence for z");
      z->step(2000, FORWARD, SINGLE);
      delay(2000);
      z->step(1700, BACKWARD, SINGLE);

      
    } else if (strstr(charBuffer, "Drink7") == &charBuffer[0]) {

      Serial.println("Begin motor sequence for y");
      y->step(300, FORWARD, SINGLE);
      delay(1000);
      y->step(300, BACKWARD, SINGLE);

      Serial.println("Begin motor sequence for x");
      x->step(1000, FORWARD, SINGLE);
      delay(1000);
      x->step(1000, BACKWARD, SINGLE);
     
      Serial.println("Begin motor sequence for z");
      z->step(2000, FORWARD, SINGLE);
      delay(2000);
      z->step(1700, BACKWARD, SINGLE);

      
    }*/
    Serial.print("Received: ");
    Serial.println(charBuffer);

    return charBuffer;
  }
}