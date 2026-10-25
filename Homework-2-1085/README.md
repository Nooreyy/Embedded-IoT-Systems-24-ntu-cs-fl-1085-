# Homework 2 — Task 1: Dual-LED Fading with Different PWM Frequencies

**Student Name:** Noor Fatima  
**Roll Number:** 1085  
**Course:** Embedded IoT Systems  
**Platform:** ESP32 DevKit C V4  
**Simulation:** Wokwi  
**Programming Language:** C++ (Arduino Framework)

## 1. Project Overview

The objective of this task is to control two LEDs independently using Pulse Width Modulation (PWM) on an ESP32 microcontroller.

Both LEDs fade smoothly from minimum to maximum brightness and then back to minimum brightness. Each LED is configured with a different PWM frequency. The program uses an 8-bit duty resolution and maintains separate duty-cycle variables for both LEDs.

## 2. Components Used

- ESP32 DevKit C V4
- One red LED
- One green LED
- Two 220Ω current-limiting resistors
- Wokwi simulator
- PlatformIO with Arduino framework

## 3. Circuit Wiring

The circuit contains two LEDs connected to separate GPIO pins of the ESP32. Each LED has its own 220Ω current-limiting resistor.

| Component | ESP32 / Circuit Connection |
|---|---|
| Red LED | GPIO 18 through a 220Ω resistor |
| Green LED | GPIO 19 through a 220Ω resistor |
| Red LED cathode | GND |
| Green LED cathode | GND |

The anode of each LED is connected to its resistor, and the other side of the resistor is connected to the corresponding ESP32 GPIO pin. Both LED cathodes are connected to the ESP32 ground.

The resistors limit the current flowing through the LEDs and help protect the components.

## 4. PWM Configuration

The ESP32 LEDC peripheral is used to generate PWM signals for controlling LED brightness.

| Parameter | LED 1 (Red) | LED 2 (Green) |
|---|---:|---:|
| GPIO pin | 18 | 19 |
| PWM channel | 0 | 1 |
| PWM frequency | 10,000 Hz (10 kHz) | 5,000 Hz (5 kHz) |
| Duty resolution | 8-bit | 8-bit |
| Duty-cycle range | 0–255 | 0–255 |
| Fade interval | 12 ms | 20 ms |

The program configures the PWM channels using `ledcSetup()` and connects them to their GPIO pins using `ledcAttachPin()`, as supported by the Arduino-ESP32 2.x API.

## 5. Duty-Cycle Control

Duty cycle determines the percentage of time that the PWM signal remains HIGH during each cycle.

With an 8-bit resolution, the duty value ranges from 0 to 255.

The duty-cycle percentage is calculated as:

**Duty Cycle (%) = (Duty Value / 255) × 100**

Examples:

| Duty Value | Approximate Duty Cycle | Expected Brightness |
|---:|---:|---|
| 0 | 0% | Off |
| 64 | 25% | Low brightness |
| 128 | 50% | Medium brightness |
| 192 | 75% | High brightness |
| 255 | 100% | Maximum brightness |

The code uses separate variables, `duty1` and `duty2`, to store the current duty-cycle values for both LEDs.

The `updateFade()` function increases or decreases the duty value by one step. When the value reaches 255, the direction reverses. When it reaches 0, the direction changes again.

The `millis()` function is used to schedule the updates without blocking the main loop with long delays.

## 6. Effect of PWM Frequency

PWM frequency represents the number of PWM cycles generated per second.

- **LED 1:** Configured at 10,000 Hz.
- **LED 2:** Configured at 5,000 Hz.

PWM frequency determines how quickly the signal switches between HIGH and LOW. At sufficiently high frequencies, the switching is generally too fast for the human eye to perceive as individual flashes, so the LED appears to have a steady brightness.

The apparent brightness is primarily controlled by the duty cycle, while the frequency determines the rate of switching.

## 7. OLED Output — Task 2

An SSD1306 I²C OLED display is not included in Task 1. It will be added in Task 2.

The current duty-cycle variables are already maintained in the program and can be used to display the live brightness percentages later.

The planned OLED output will show:

- LED 1 duty-cycle percentage
- LED 1 configured PWM frequency
- LED 2 duty-cycle percentage
- LED 2 configured PWM frequency

The displayed values will be calculated from the actual duty values used by the program and the configured PWM frequencies.

## 8. Testing and Results

The project is designed to be tested in the Wokwi simulator before implementation on physical hardware.

The following checks are required:

1. Verify that both LEDs are connected to their intended GPIO pins.
2. Confirm that both LEDs fade smoothly from minimum to maximum brightness and back.
3. Verify the configured PWM frequencies of 10 kHz and 5 kHz.
4. Confirm that the duty-cycle values remain within the range of 0–255.
5. Check that the fading updates occur at the configured intervals.

**Test status:** The program and circuit are configured for these tests. Final results should be recorded after running the simulation and verifying the actual PWM outputs.

## 9. Technical Note: PWM Timer Resources

The code uses PWM channels 0 and 1 with different frequency settings. However, assigning different channel numbers does not by itself guarantee independent frequencies because LEDC channels can share timer resources.

The actual frequency behavior must be verified for the selected ESP32 variant and Arduino-ESP32 core. If both channels share a timer, the configuration may need adjustment to achieve independent frequencies.

The configured values in the source code are 10,000 Hz and 5,000 Hz; these should not be treated as independently verified hardware output frequencies until tested.

## 10. Conclusion

Task 1 establishes a dual-LED fading system using ESP32 PWM control. It demonstrates how duty-cycle resolution, PWM frequency, GPIO outputs, and timed updates can be used to control LED brightness.

The same duty-cycle variables will be reused in Task 2 to display the live PWM parameters on an SSD1306 OLED.




## Task 2: Dual LED PWM with OLED Monitoring

### Objective
The objective of Task 2 is to extend Task 1 by adding an SSD1306 OLED display to monitor the PWM duty cycle of two LEDs. The display shows the duty-cycle percentage and configured PWM frequency of each LED.

### Components Used
- ESP32 DevKit C V4
- 2 LEDs (Red and Green)
- 2 × 220Ω current-limiting resistors
- SSD1306 OLED display (128 × 64 pixels, I2C)
- Wokwi simulator
- PlatformIO with Arduino framework

### Connections

| Component | ESP32 Pin |
|---|---|
| LED 1 (Red) | GPIO 19 |
| LED 2 (Green) | GPIO 18 |
| OLED SDA | GPIO 21 |
| OLED SCL | GPIO 22 |
| OLED VCC | 3.3V |
| OLED GND | GND |

Each LED is connected in series with a 220Ω resistor to limit current.

### PWM Configuration

| Parameter | LED 1 | LED 2 |
|---|---|---|
| GPIO Pin | GPIO 18 | GPIO 19 |
| PWM Channel | 0 | 1 |
| Configured Frequency | 1000 Hz | 5000 Hz |
| Resolution | 8-bit | 8-bit |
| Duty-Cycle Range | 0–255 | 0–255 |
| Fading Interval | 12 ms | 20 ms |

**Note:** The GPIO assignments in the PWM configuration table follow the code. In the current `diagram.json`, the red LED is connected to GPIO 19 and the green LED is connected to GPIO 18, so the physical LED labels are swapped relative to the code's LED 1 and LED 2 names.

### OLED Display Features

The SSD1306 OLED displays:
- A heading: `DUAL PWM MONITOR`
- LED 1 duty-cycle percentage and configured frequency
- LED 2 duty-cycle percentage and configured frequency
- PWM resolution (8-bit)

The duty-cycle percentage is calculated from the 8-bit duty value using:

`Duty Cycle (%) = (Duty Value × 100) / 255`

The OLED refreshes approximately every 150 milliseconds.

### Implementation Details

1. The ESP32 configures two LEDC PWM channels using the Arduino-ESP32 2.x API.
2. Each LED's duty value increases and decreases to produce a fading effect.
3. The `millis()` function schedules LED updates and OLED refreshes without using long delays in the main loop.
4. The `Wire` library initializes I2C communication using GPIO 21 for SDA and GPIO 22 for SCL.
5. The Adafruit GFX and Adafruit SSD1306 libraries control the OLED display.
6. The duty-cycle percentage is calculated from the current duty value and displayed on the OLED.

### Technical Note: PWM Timer Sharing

On the classic ESP32, LEDC channels share timer resources. Therefore, configuring different frequencies on channels 0 and 1 may not provide two independent PWM frequencies because these channels can share a timer.

The OLED code displays the configured frequency constants (1000 Hz and 5000 Hz), not a measurement of the actual output frequencies. Actual frequency independence must be verified separately.

### Expected Result

- Both LEDs fade in and out continuously.
- The OLED displays the current duty-cycle percentages.
- The OLED displays the configured PWM frequency values.
- The display updates periodically while the LEDs continue fading.

### Libraries Used

- `Wire.h` — I2C communication
- `Adafruit_GFX.h` — graphics and text rendering
- `Adafruit_SSD1306.h` — SSD1306 OLED control

### Conclusion

Task 2 extends the dual-LED PWM project by integrating an I2C OLED display. It demonstrates PWM duty-cycle control, periodic updates using `millis()`, and displaying live duty-cycle calculations on an OLED. The actual independence of the two PWM frequencies requires separate verification due to ESP32 LEDC timer-sharing limitations.