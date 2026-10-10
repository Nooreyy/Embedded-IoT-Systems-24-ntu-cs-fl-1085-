
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

// ESP32 pins
#define BUTTON_PIN 18
#define SDA_PIN 21
#define SCL_PIN 22

// Debounce interval in milliseconds
const unsigned long DEBOUNCE_INTERVAL = 50;

// Create OLED display object
Adafruit_SSD1306 display(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    OLED_RESET
);

// Button states
int lastButtonReading = HIGH;
int stableButtonState = HIGH;

unsigned long lastDebounceTime = 0;
unsigned long pressCount = 0;

// Function to update OLED screen
void updateDisplay() {
    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    // Project title
    display.setCursor(0, 0);
    display.println("ESP32 BUTTON STATUS");

    display.drawLine(0, 12, 127, 12, SSD1306_WHITE);

    // Current button status
    display.setCursor(0, 22);
    display.print("Button: ");

    if (stableButtonState == LOW) {
        display.println("PRESSED");
    } else {
        display.println("RELEASED");
    }

    // Accepted press count
    display.setCursor(0, 40);
    display.print("Press Count: ");
    display.println(pressCount);

    // Send buffer contents to the OLED
    display.display();
}

void setup() {
    Serial.begin(115200);

    // Configure button with internal pull-up resistor
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    // Initialize I2C using the selected ESP32 pins
    Wire.begin(SDA_PIN, SCL_PIN);

    // Initialize OLED
    if (!display.begin(
            SSD1306_SWITCHCAPVCC,
            OLED_ADDRESS
        )) {
        Serial.println("OLED initialization failed!");
        while (true) {
            // Stop here if the OLED cannot initialize
        }
    }

    // Initialize button states
    lastButtonReading = digitalRead(BUTTON_PIN);
    stableButtonState = lastButtonReading;
    lastDebounceTime = millis();

    // Show initial status and count
    updateDisplay();

    Serial.println("ESP32 OLED Button Project Started");
    Serial.println("Press count: 0");
}

void loop() {
    // Read current button input
    int currentReading = digitalRead(BUTTON_PIN);

    // Detect a raw input change
    if (currentReading != lastButtonReading) {
        lastDebounceTime = millis();
        lastButtonReading = currentReading;
    }

    // Accept a state change only after it remains stable
    if (millis() - lastDebounceTime >= DEBOUNCE_INTERVAL) {

        if (currentReading != stableButtonState) {
            stableButtonState = currentReading;

            // INPUT_PULLUP means LOW = pressed
            if (stableButtonState == LOW) {
                pressCount++;

                Serial.print("Accepted press count: ");
                Serial.println(pressCount);
            } else {
                Serial.println("Button released");
            }

            // Refresh display after an accepted state change
            updateDisplay();
        }
    }

    // Other tasks can run here without a blocking debounce delay
}
