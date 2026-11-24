#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Pin Definitions based on your Wokwi diagram
#define BTN_START_PIN 19
#define BTN_RESET_PIN 18
#define LED_PIN       4

// OLED Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Timer & State Variables
int totalSeconds = 10; // 25 minutes default Pomodoro
int remainingSeconds = totalSeconds;
bool isRunning = false;

// Debounce Variables
unsigned long lastDebounceTime1 = 0;
unsigned long lastDebounceTime2 = 0;
const unsigned long debounceDelay = 50;
int lastBtn1State = HIGH;
int lastBtn2State = HIGH;
int btn1State = HIGH;
int btn2State = HIGH;

// ESP32 Hardware Timer (v2.x Syntax)
hw_timer_t *timer = NULL;
volatile bool tickFlag = false;

// Hardware Timer Interrupt Service Routine
void IRAM_ATTR onTimer() {
  tickFlag = true;
}

// Function to Update OLED Display
void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(15, 0);
  display.print(F("POMODORO TIMER"));

  // Format Time MM:SS
  int mins = remainingSeconds / 60;
  int secs = remainingSeconds % 60;

  display.setTextSize(2);
  display.setCursor(20, 25);
  if (mins < 10) display.print("0");
  display.print(mins);
  display.print(":");
  if (secs < 10) display.print("0");
  display.print(secs);

  display.setTextSize(1);
  display.setCursor(35, 50);
  if (isRunning) {
    display.print(F("[ RUNNING ]"));
  } else {
    display.print(F("[ PAUSED / READY ]"));
  }
  display.display();
}

void setup() {
  Serial.begin(115200);

  // Digital I/O with Internal Pull-ups
  pinMode(BTN_START_PIN, INPUT_PULLUP);
  pinMode(BTN_RESET_PIN, INPUT_PULLUP);

  // PWM Setup for LED (ESP32 v2.x API)
  ledcSetup(0, 5000, 8); // Channel 0, 5kHz frequency, 8-bit resolution
  ledcAttachPin(LED_PIN, 0); // Attach LED pin to Channel 0

  // I2C OLED Initialization
  Wire.begin(21, 22);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  updateDisplay();

  // Hardware Timer Setup (v2.x syntax: timerBegin(timer_id, prescaler, count_up))
  timer = timerBegin(0, 80, true); // 80 prescaler -> 1 tick = 1 microsecond (80MHz / 80)
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000, true); // 1,000,000 microseconds = 1 second, auto-reload
  timerAlarmEnable(timer);
}

void loop() {
  // --- 1. Button 1 Logic (Start / Pause with Software Debounce) ---
  int reading1 = digitalRead(BTN_START_PIN);
  if (reading1 != lastBtn1State) {
    lastDebounceTime1 = millis();
  }
  if ((millis() - lastDebounceTime1) > debounceDelay) {
    if (reading1 != btn1State) {
      btn1State = reading1;
      if (btn1State == LOW) { // Button pressed
        isRunning = !isRunning; // Toggle state
      }
    }
  }
  lastBtn1State = reading1;

  // --- 2. Button 2 Logic (Reset with Software Debounce) ---
  int reading2 = digitalRead(BTN_RESET_PIN);
  if (reading2 != lastBtn2State) {
    lastDebounceTime2 = millis();
  }
  if ((millis() - lastDebounceTime2) > debounceDelay) {
    if (reading2 != btn2State) {
      btn2State = reading2;
      if (btn2State == LOW) { // Button pressed
        isRunning = false;
        remainingSeconds = totalSeconds;
        updateDisplay();
      }
    }
  }
  lastBtn2State = reading2;

  // --- 3. Hardware Timer Countdown Tick ---
  if (tickFlag) {
    tickFlag = false;
    if (isRunning && remainingSeconds > 0) {
      remainingSeconds--;
      updateDisplay();
      if (remainingSeconds == 0) {
        isRunning = false;
      }
    }
  }

  // --- 4. PWM Brightness Control for LED when Running ---
  if (isRunning) {
    // Simple fading effect using millis
    int brightness = (sin(millis() / 200.0) + 1.0) * 127.5; 
    ledcWrite(0, brightness); // Write to Channel 0
  } else {
    ledcWrite(0, 15); // Dim light when paused/stopped
  }
}