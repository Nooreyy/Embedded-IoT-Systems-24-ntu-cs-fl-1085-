#include <Arduino.h>

#define BUTTON_PIN 4
#define LED_PIN 21

hw_timer_t *debounceTimer = NULL;

volatile bool debounceActive = false;
volatile bool buttonPressed = false;


// Button Interrupt
void IRAM_ATTR onButtonISR()
{
    if (!debounceActive)
    {
        debounceActive = true;

        // Start debounce timer
        timerWrite(debounceTimer, 0);
        timerAlarmEnable(debounceTimer);
    }
}


// Timer Interrupt
void IRAM_ATTR onDebounceTimer()
{
    // Stop debounce timer
    timerAlarmDisable(debounceTimer);

    debounceActive = false;

    // Check button after 50 ms
    if (digitalRead(BUTTON_PIN) == LOW)
    {
        buttonPressed = true;
    }
}


void setup()
{
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);

    digitalWrite(LED_PIN, LOW);
    debounceTimer = timerBegin(0, 80, true);


    // Attach timer interrupt
    timerAttachInterrupt(
        debounceTimer,
        &onDebounceTimer,
        true
    );


    // 50 ms debounce time
    // 1 MHz = 1 tick per microsecond
    // 50,000 ticks = 50 ms
    timerAlarmWrite(
        debounceTimer,
        50000,
        false
    );


    // Button interrupt
    attachInterrupt(
        digitalPinToInterrupt(BUTTON_PIN),
        onButtonISR,
        FALLING
    );
}


void loop()
{
    if (buttonPressed)
    {
        buttonPressed = false;

        // Toggle LED
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }
}