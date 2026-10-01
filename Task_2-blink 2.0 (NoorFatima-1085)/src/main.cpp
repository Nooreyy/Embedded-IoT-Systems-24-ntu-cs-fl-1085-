// Week2
// Blink LED using Arduino IDE with simulator
// Embedded IoT System Fall-2026

// Name: Noor Fatima                 Reg#: 1085
#include <Arduino.h>

#define LED_BUILTIN 2

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN,HIGH);
  delay(3000);
  digitalWrite(LED_BUILTIN,LOW);
  delay(3000);
}
