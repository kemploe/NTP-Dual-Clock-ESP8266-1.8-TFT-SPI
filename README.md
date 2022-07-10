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
Upon start up the WiFiManager kicks in. If a saved WiFi configuration found it will configure NodeMCU as a WiFi Station (STA) and connect to the WiFi Access Point that is configured in the saved configuration. Otherwise it will configure NodeMCU as an Access Point (AP) with SSID: "ntpDualClock" and spawn a web page (WiFiManager portal) that you can connect to and configure using your Smartphone. Once the WiFi has been configured it will switch back to Station (STA) mode and start the clock.
Once connected to the internet, it will start to synchronize and fetch the unix epoch from the configured NTP server. You do not need to manually adjust the date and time.

![](https://github.com/kemploe/NTP-Dual-Clock-ESP8266-1.8-TFT-SPI/blob/main/images/2022-07-10_151118-1.png)

NTP sync status is displayed on the lower side of the display next to the date. NTP is displayed in green means it is good and synced in the last 1 hour, NTP is displayed in yellow means it is synced in the last 1 - 24 hour, NTP is displayed in red means it is out of sync.



