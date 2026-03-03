#include <ESP32Servo.h>

// Define 4 Servo objects for the ESCs
Servo esc1, esc2, esc3, esc4;

// Pins used (Adjust if your board layout is different)
int pins[] = {13, 12, 14, 27}; 

void setup() {
  Serial.begin(115200);
  
  // Allocate timers for all 4 motors
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  // Attach all ESCs
  esc1.attach(pins[0], 1000, 2000);
  esc2.attach(pins[1], 1000, 2000);
  esc3.attach(pins[2], 1000, 2000);
  esc4.attach(pins[3], 1000, 2000);

  Serial.println("--- 4-MOTOR CALIBRATION SEQUENCE ---");
  Serial.println("1. Type 'H' [Enter] -> THEN plug in LiPo (High Signal)");
  Serial.println("2. Wait for beeps (beep-beep!)");
  Serial.println("3. Type 'L' [Enter] -> (Low Signal / Arm)");
  Serial.println("4. Type 'S' [Enter] -> Test Spin all motors");
  Serial.println("5. Type '0' [Enter] -> STOP ALL");
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    
    if (c == 'H') {
      Serial.println("Sending HIGH to all motors...");
      sendToAll(2000);
    } 
    else if (c == 'L') {
      Serial.println("Sending LOW (Arming) to all motors...");
      sendToAll(1000);
    } 
    else if (c == 'S') {
      Serial.println("Spinning all motors at low speed...");
      sendToAll(1180); // 1180us is a safe 'slow' spin
    } 
    else if (c == '0') {
      Serial.println("EMERGENCY STOP");
      sendToAll(1000);
    }
  }
}

// Helper function to update all motors at once
void sendToAll(int value) {
  esc1.writeMicroseconds(value);
  esc2.writeMicroseconds(value);
  esc3.writeMicroseconds(value);
  esc4.writeMicroseconds(value);
}