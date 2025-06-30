#include <Servo.h>

#define IR_SENSOR_PIN 2  // IR sensor connected to D2
#define SERVO_PIN 3      // Servo connected to D3

Servo myServo;

void setup() {
    pinMode(IR_SENSOR_PIN, INPUT);
    myServo.attach(SERVO_PIN);
    myServo.write(0);  // Start at 0 degrees
}

void loop() {
    int irState = digitalRead(IR_SENSOR_PIN);

    if (irState == LOW) {  // Object detected (IR sensor output LOW)
        myServo.write(90);  // Rotate servo to 90 degrees
        delay(1000);        // Wait for 1 second
        myServo.write(0);   // Return to 0 degrees
    }

    delay(100); // Small delay to prevent fast triggering
}
