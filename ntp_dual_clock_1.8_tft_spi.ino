// NodeMCU ESP8266 - NTP Dual Clock 1.8" TFT SPI

#include <Adafruit_ST7735.h>
#include <ESP8266WiFi.h>
#include <ezTime.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>

// Adjustable parameters
const char   * t_zone = "Asia/Jakarta";     // See https://en.wikipedia.org/wiki/List_of_tz_database_time_zones
const char * ntp_pool = "id.pool.ntp.org";  // NTP Server pool address
const int  duty_cycle = 72;                 // TFT brightness using PWM duty cycle (0-255)
const int display_ori = 1;                  // TFT display orientation
String   new_hostname = "ntpDualClock";

// day of the week (dow) and it's absolute x position array
char loc_dow_array[7][10] = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
byte x_pos[7] = {50, 50, 45, 35, 40, 55, 40};
static byte prev_loc_dow = 0;

// day of the week (dow) - short
char utc_dow_array[7][10] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
static byte prev_utc_dow = 0;

Timezone my_tz;                              // local timezone variable
time_t utc_time;                             // current & displayed UTC
time_t loc_time;                             // current & displayed local time


// Pin assignment for PWM output to set TFT backlight brightness
uint8_t led_pin = 5;           // TFT LED/BL    pin is connected to NodeMCU GPIO 5

// Pin assignment for 1.8" TFT display with ST7735
#define TFT_A0    4            // TFT DC/A0     pin is connected to NodeMCU GPIO 4
#define TFT_CS    0            // TFT CS        pin is connected to NodeMCU GPIO 0
#define TFT_RST   2            // TFT RST/RESET pin is connected to NodeMCU GPIO 2
#define TFT_SCK   14           // TFT SCK/SCLK  pin is connected to NodeMCU GPIO 14
#define TFT_SDA   13           // TFT SDA/MOSI  pin is connected to NodeMCU GPIO 13

// 1.8" TFT display with ST7735
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_A0, TFT_RST);

// Convert RGB colors
#define RGB(r, g, b) ((( r & 0xF8 ) << 8)|(( g & 0xFC ) << 3 )|( b >> 3 ))

// TFT display colors
#define BLACK       RGB(  0,   0,   0)
#define AQUAMARINE  RGB(127, 255, 212)
#define GREY        RGB(128, 128, 128)
#define DARKGREY    RGB(169, 169, 169)
#define LIGHTGREY   RGB(211, 211, 211)
#define TURQUOISE   RGB( 64, 224, 208)
#define PINK        RGB(255, 128, 192)
#define OLIVE       RGB(128, 128,   0)
#define PURPLE      RGB(128,   0, 128)
#define AZURE       RGB(  0, 128, 255)
#define ORANGE      RGB(255, 128,  64)
#define CYAN        RGB(  0, 255, 255)
#define DARKCYAN    RGB(  0, 139, 139)
#define RED         RGB(255,   0,   0)
#define YELLOW      RGB(255, 255,   0)
#define WHITE       RGB(255, 255, 255)
#define BLUE        RGB(  0,   0, 255)
#define GREEN       RGB(  0, 255,   0)

#define SYNC_MARGINAL 3600                           // yellow status if no sync for 1 hour
#define SYNC_LOST     86400                          // red status if no sync for 1 day

// LOCAL CLOCK FUNCTION
void LC()
{
  loc_time = my_tz.now();
// print time (HH:MM)
  tft.setTextSize(3);                                // text size = 3
  tft.setCursor(26, 49);                             // move cursor to position (26, 49) pixel
  tft.setTextColor(LIGHTGREY, BLACK);                // set text color to lightgrey and black background
  tft.printf( "%02u:%02u", hour(loc_time), minute(loc_time) );

// print time (:SS)
  tft.setTextSize(2);                                // text size = 2
  tft.setCursor(114, 56);                            // move cursor to position (114, 56) pixel
  tft.printf( ":%02u", second(loc_time) );

// print day of the week
  if( prev_loc_dow != weekday(loc_time) )
  {
    prev_loc_dow = weekday(loc_time);
    tft.fillRect(17, 3, 142, 19, BLACK);             // fill rectangle (x,y,w,h,color)
    tft.setTextSize(2);                              // text size = 2
    tft.setTextColor(CYAN, BLACK);                   // set text color to cyan and black background
    tft.setCursor(x_pos[prev_loc_dow-1], 5);         // move cursor to position (dow, 5) pixel
    tft.print( loc_dow_array[prev_loc_dow-1] );
  }

// print date (DD-MM-YYYY)
  tft.setTextSize(2);                                // text size = 2
  tft.setCursor(28, 28);                             // move cursor to position (28, 28) pixel
  tft.setTextColor(YELLOW, BLACK);                   // set text color to yellow and black background
  tft.printf( "%02u/%02u/%04u", day(loc_time), month(loc_time), year(loc_time) );
}

// UTC CLOCK FUNCTION
void UC()
{
  utc_time = now();                                  // requesting actual time (now)
// print time (HH:MM)
  tft.setTextSize(3);                                // text size = 3
  tft.setCursor(26, 102);                            // move cursor to position (26, 102) pixel
  tft.setTextColor(LIGHTGREY, BLACK);                // set text color to lightgrey and black background
  tft.printf( "%02u:%02u", hour(utc_time), minute(utc_time) );

// print time (:SS)
  tft.setTextSize(2);                                // text size = 2
  tft.setCursor(114, 109);                           // move cursor to position (114, 109) pixel
  tft.printf( ":%02u", second(utc_time) );

// print day of the week
  if( prev_utc_dow != weekday(utc_time) )
  {
    prev_utc_dow = weekday(utc_time);
    tft.setTextSize(1);                              // text size = 1
    tft.setTextColor(CYAN, BLACK);                   // set text color to cyan and black background
    tft.setCursor(27, 87);                           // move cursor to position (27, 87) pixel
    tft.print( utc_dow_array[prev_utc_dow-1] );
  }

// print date (MM-DD-YYYY)
  tft.setTextSize(1);                                // text size = 1
  tft.setCursor(53, 87);                             // move cursor to position (53, 87) pixel
  tft.setTextColor(YELLOW, BLACK);                   // set text color to yellow and black background
  tft.printf( "%02u-%02u-%04u", month(utc_time), day(utc_time), year(utc_time) );
}

// NTP Clock Status Function - inspired by W8BH - Bruce E. Hall - https://github.com/bhall66/NTP-clock
void NCS()
{
  int sync_result, sync_age;

  if ( second()%30 ) return;                         // update every 30 seconds 
  sync_age = now() - lastNtpUpdateTime();            // how long since last sync?
  if ( sync_age < SYNC_MARGINAL )                    // time is good & in sync
    sync_result = GREEN;
  else 
  if ( sync_age < SYNC_LOST )                        // sync is 1-24 hours old
    sync_result = YELLOW;
  else 
    sync_result = RED;                               // time is stale!

  // display sync_result
  tft.setTextSize(1);                                // text size = 1
  tft.setCursor(120, 87);                            // move cursor to position (120, 87) pixel
  tft.setTextColor(sync_result, BLACK);              // set text color to 'sync_result' and black background
  tft.print( "NTP" );
  tft.fillRoundRect(142, 87, 7, 7, 0, sync_result);  // show clock status color as a 'sync_result'
}

// SETUP
void setup()
{
  // initializing 1.8" TFT display
  analogWrite(led_pin, duty_cycle);                  // set display brightness
  tft.initR(INITR_BLACKTAB);                         // initialize TFT display with ST7735 chip
  tft.setRotation(display_ori);                      // set display orientation
  tft.fillScreen(BLACK);                             // blanking display

  // initializing Serial Port
  Serial.begin(115200);                              // set serial port speed
  delay (3000);                                      // 3 seconds delay

  // WiFiManager, Local initialization. 
  // Once its business is done, there is no need to keep it around
  WiFiManager wfm;

    // Supress Debug information
    wfm.setDebugOutput(true);

    // reset settings - wipe stored credentials for testing
    // these are stored by the esp library
    wfm.resetSettings();

    Serial.println("WiFi connecting");
    tft.setCursor(38, 20);                         // move cursor to position (38, 20) pixel
    tft.print("WiFi connecting");

    if (!wfm.autoConnect( "ntpDualClock" )) 
    {
      // Did not connect, print error message
      Serial.println("failed to connect and hit timeout");
      tft.setCursor(40, 30);                       // move cursor to position (40, 30) pixel
      tft.print("failed to connect");
   
      // Reset and try again
      ESP.restart();
      delay(1000);
    }
 
    // Connected!
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    tft.setCursor(40, 40);                         // move cursor to position (40, 40) pixel
    tft.print("WiFi connected");
    tft.setCursor(40, 60);                         // move cursor to position (40, 60) pixel
    tft.print(WiFi.localIP());
    delay(3000);

  // set hostname
  WiFi.setHostname(new_hostname.c_str());

  // priming eztime library
  setServer(ntp_pool);                             // set NTP server
  while (timeStatus()!=timeSet)                    // wait until time synced
  {
    events();                                      // allow ezTime to sync
    delay(1000);
  }
  my_tz.setLocation(t_zone);

  // static frame for local time
  tft.fillScreen(BLACK);                           // blanking display
  tft.drawRect(0, 0, tft.width(), 76, WHITE);      // draw rectangle (x, y, w, h, color)
  tft.fillRect(0, 0, 16, 76, WHITE);
  tft.fillRect(3, 12, 10, 52, BLACK);
  tft.setTextSize(1);                              // text size = 1
  tft.setTextColor(CYAN, BLACK);                   // set text color to cyan and black background
  tft.setCursor(5, 14);                            // move cursor to position (5, 14) pixel
  tft.print( "L" );
  tft.setCursor(5, 24);                            // move cursor to position (5, 24) pixel
  tft.print( "O" );
  tft.setCursor(5, 34);                            // move cursor to position (5, 34) pixel
  tft.print( "C" );
  tft.setCursor(5, 44);                            // move cursor to position (5, 44) pixel
  tft.print( "A" );
  tft.setCursor(5, 54);                            // move cursor to position (5, 54) pixel
  tft.print( "L" );
  
  // static frame for UTC
  tft.drawRect(0, 79, tft.width(), 49, BLUE);      // draw rectangle (x, y, w, h, color)
  tft.setTextSize(1);                              // text size = 1
  tft.setTextColor(WHITE, BLACK);                  // set text color to cyan and black background
  tft.fillRect(0, 79, 16, 48, BLUE);
  tft.fillRect(3, 86, 10, 32, BLACK);
  tft.setCursor(5, 89);                            // move cursor to position (5, 89) pixel
  tft.print( "U" );
  tft.setCursor(5, 99);                            // move cursor to position (5, 99) pixel
  tft.print( "T" );
  tft.setCursor(5, 109);                           // move cursor to position (5, 109) pixel
  tft.print( "C" );
}

// MAIN LOOP
void loop()
{
  LC();                                            // requesting Local Clock
  UC();                                            // requesting UTC Clock
  NCS();                                           // requesting NTP Clock Status
  events();                                        // update NTP
}

// PROGRAM END
