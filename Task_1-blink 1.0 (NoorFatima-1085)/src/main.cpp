// Week1-Lecture2
// Blink LED using Arduino IDE
// Embedded IoT System Fall-2026

// Name: Noor Fatima                 Reg#: 1085


#include <Arduino.h>

#define LED_BUILTIN 13

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off (LOW is the voltage level)
  delay(1000);                       // wait for a second
}
