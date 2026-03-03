#include <ESP32Servo.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
Servo m1, m2, m3, m4;

// Pins
const int pin1 = 13; const int pin2 = 12;
const int pin3 = 14; const int pin4 = 27;
const int batteryPin = 34; // Connect voltage divider here

int baseThrottle = 1000;
float voltage = 0.0;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("Nitya_F450_Pro");
  
  ESP32PWM::allocateTimer(0);
  m1.attach(pin1, 1000, 2000); m2.attach(pin2, 1000, 2000);
  m3.attach(pin3, 1000, 2000); m4.attach(pin4, 1000, 2000);
  
  analogReadResolution(12); // Sets 0-4095 range
}

void loop() {
  // --- BATTERY CALCULATION ---
  int rawADC = analogRead(batteryPin);
  // Conversion: (3.3V / 4095) * Divider_Ratio (approx 4.03)
  voltage = (rawADC / 4095.0) * 3.3 * 4.03; 

  if (SerialBT.available()) {
    char cmd = SerialBT.read();
    if (cmd == 'U') baseThrottle += 20;
    if (cmd == 'D') baseThrottle -= 20;
    if (cmd == 'H') baseThrottle = 1500;
    if (cmd == '0') baseThrottle = 1000;
    
    baseThrottle = constrain(baseThrottle, 1000, 2000);
    updateMotors(baseThrottle);
  }

  // --- SEND STATUS TO PHONE ---
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 1000) {
    SerialBT.print("Batt: "); SerialBT.print(voltage); SerialBT.print("V | ");
    SerialBT.print("Thr: "); SerialBT.println(baseThrottle);
    
    if (voltage < 10.5 && voltage > 5.0) {
      SerialBT.println("!!! LOW BATTERY - LAND NOW !!!");
    }
    lastUpdate = millis();
  }
}

void updateMotors(int v) {
  m1.writeMicroseconds(v); m2.writeMicroseconds(v);
  m3.writeMicroseconds(v); m4.writeMicroseconds(v4);
}