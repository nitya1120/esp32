#include <ESP32Servo.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// ESC Objects
Servo m1, m2, m3, m4;

// Your Specific Pin Mapping
const int pin1 = 13; 
const int pin2 = 12; 
const int pin3 = 14; // The one that was cutting off at 1700
const int pin4 = 27; 

int throttle = 1000;
const int stepValue = 10; // Updated to 10 as requested

void setup() {
  Serial.begin(115200);
  SerialBT.begin("Nitya_F450_Final_Sync"); 

  // Allocate all 4 timers for smooth PWM
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  // Attach all ESCs with the standard 1000-2000ms range
  m1.attach(pin1, 1000, 2000);
  m2.attach(pin2, 1000, 2000);
  m3.attach(pin3, 1000, 2000);
  m4.attach(pin4, 1000, 2000);

  // Safety Initialization
  updateAll(1000);
  
  Serial.println("Bluetooth Ready. Use 'H' and 'L' to SYNC ALL MOTORS.");
}

void loop() {
  if (SerialBT.available()) {
    char cmd = SerialBT.read();

    // --- CALIBRATION (Run this once to fix Pin 14 and Pin 12 RPM) ---
    if (cmd == 'H') {
      updateAll(2000);
      SerialBT.println(">> MAX SIGNAL (2000). Plug in LiPo now.");
    }
    else if (cmd == 'L') {
      updateAll(1000);
      throttle = 1000;
      SerialBT.println(">> MIN SIGNAL (1000). All motors synced!");
    }
    
    // --- FLIGHT TEST ---
    else if (cmd == 'U') {
      throttle += stepValue; // Increased by 10
      throttle = constrain(throttle, 1000, 2000);
      updateAll(throttle);
      SerialBT.print("Throttle: "); SerialBT.println(throttle);
    }
    else if (cmd == 'D') {
      throttle -= stepValue; // Decreased by 10
      throttle = constrain(throttle, 1000, 2000);
      updateAll(throttle);
      SerialBT.print("Throttle: "); SerialBT.println(throttle);
    }
    else if (cmd == '0') {
      throttle = 1000;
      updateAll(1000);
      SerialBT.println("!! EMERGENCY STOP !!");
    }
  }
}

// Forces all 4 motors to the exact same microsecond value
void updateAll(int val) {
  m1.writeMicroseconds(val);
  m2.writeMicroseconds(val);
  m3.writeMicroseconds(val);
  m4.writeMicroseconds(val);
}