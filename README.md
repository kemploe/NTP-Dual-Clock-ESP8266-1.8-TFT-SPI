# NTP-Dual-Clock-ESP8266-1.8-TFT-SPI

![dual clock](https://github.com/kemploe/NTP-Dual-Clock-ESP8266-1.8-TFT-SPI/blob/main/images/2022-07-10_151118.png)

## 1. Introduction

This is basically a NTP clock running on NodeMCU ESP8266 with Adafruit compatible 1.8" TFT SPI display.

### THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY.

## 2. Bill of Material (BOM)

   - NodeMCU ESP8266 - https://bit.ly/3zTizP3 or equivalent.
   - 1.8" TFT SPI Display - https://bit.ly/3QDjysy or equivalent.
   - Breadboard - https://bit.ly/3xL2dFn or equivalent.
   - Jumpers - https://bit.ly/39KrPdJ or equivalent.

## 3. Cabling/Wiring the 1.8" TFT SPI display to NodeMCU
      Display <---> NodeMCU
          VCC <---> 5V or 3.3V
          GND <---> GND
           CS <---> GPIO 0
    RESET/RST <---> GPIO 2
        A0/DC <---> GPIO 4
     SDA/MOSI <---> GPIO 13
     SCK/SCLK <---> GPIO 14
       LED/BL <---> GPIO 5

![1.8" TFT SPI Display](https://github.com/kemploe/NTP-Dual-Clock-ESP8266-1.8-TFT-SPI/blob/main/images/2022-07-01_082841.png)

### Note: 
On some Adafruit compatible 1.8" TFT SPI display, RESET is RST, A0 is DC, SDA is MOSI, SCK is SCLK and LED is BL. So pay attention to your wiring, double check before powering up the device.

## 4. Operation

