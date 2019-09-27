
/***************************************************
 * Weather display by James Harris (jim.harris@antikytherallc.com)
 * Based upon the e_Ink display example code from Adafruit with 
 * Logic added to read the OpenWeather API, and graphical
 * bitmaps translated to c-string from Alessio Atzeni's Meteo-Icons
 * (Web: http://www.alessioatzeni.com)
 */
/***************************************************
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/


#include "images.h"
#include "iconstuff.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "Adafruit_EPD.h"

// WIFI Settings
const char* WIFI_SSID = "YOURWIFI";
const char* WIFI_PWD = "YOURPASSWORD";
StaticJsonDocument<5000> doc; 
StaticJsonDocument<10000> doc2; 
// Put in your OpenWeathermap API from https://openweathermap.org/
String apiKey="yourkey";
// Put in your location from https://openweathermap.org/current
String location = "yourlocationID";

String DISPLAYED_CITY_NAME = "YourCity";
int status = WL_IDLE_STATUS; 
char server[] = "api.openweathermap.org"; 
WiFiClient client;    
#define SD_CS       14
#define SRAM_CS     32
#define EPD_CS      15
#define EPD_DC      33  
#define EPD_RESET   -1 // can set to -1 and share with microcontroller Reset!
#define EPD_BUSY    -1 // can set to -1 to not use a pin (will wait a fixed delay)
#define COLOR1 EPD_BLACK
#define COLOR2 EPD_RED

/* Uncomment the following line if you are using 2.13" tricolor EPD */
Adafruit_IL0373 display(212, 104, EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY);

const String WDAY_NAMES[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


uint16_t curcolor;
uint16_t todayhighcol;
uint16_t todaylowcol;

uint16_t tomhighcol;
uint16_t tomlowcol;

uint16_t nexthighcol;
uint16_t nextlowcol;

//given unix time t, returns day of week Sun-Sat as an integer 0-6
uint8_t dow(unsigned long t)
{
    return ((t / 86400) + 4) % 7;
}
unsigned long utime;
uint8_t wkday;
uint8_t wkday2;
void getWeather() { 
 Serial.println("\nStarting connection to server..."); 
 // if you get a connection, report back via serial: 
 if (client.connect(server, 80)) { 
   Serial.println("connected to server"); 
   // Make a HTTP request: 
   client.print("GET /data/2.5/weather?"); 
   client.print("id="+location); 
   client.print("&appid="+apiKey); 
   client.print("&cnt=3"); 
   client.println("&units=imperial"); 
   client.println("Host: api.openweathermap.org"); 
   client.println("Connection: close"); 
   client.println(); 
 } else { 
   Serial.println("unable to connect"); 
 } 
}

void getForecast() { 
 Serial.println("\nStarting connection to server..."); 
 // if you get a connection, report back via serial: 
 if (client.connect(server, 80)) { 
   Serial.println("connected to server"); 
   // Make a HTTP request: 
   client.print("GET /data/2.5/forecast?"); 
   client.print("id="+location); 
   client.print("&appid="+apiKey); 
   client.print("&cnt=6"); 
   client.println("&units=imperial"); 
   client.println("Host: api.openweathermap.org"); 
   client.println("Connection: close"); 
   client.println(); 
 } else { 
   Serial.println("unable to connect"); 
 } 
}

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); }
  Serial.println("Adafruit EPD test");
   // Start WiFi
  WiFi.begin(WIFI_SSID, WIFI_PWD);
  int counter = 0;
  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    ++counter;
    if (counter >= 60){
      ESP.restart();
    }
  }
  
  Serial.println("connected");

  getWeather(); 

  String line = ""; 
 while (client.connected()) { 
   line = client.readStringUntil('\n'); 
   Serial.println(line); 
   //Serial.println("parsingValues"); 
   //create a json buffer where to store the json data 
   
   

  auto error = deserializeJson(doc, line);
  if (error) {
      Serial.print(F("deserializeJson() failed with code "));
      Serial.println(error.c_str());
      return;
  }
 }

   getForecast(); 

  String line2 = ""; 
 while (client.connected()) { 
   line2 = client.readStringUntil('\n'); 
   Serial.println(line2); 
   //Serial.println("parsingValues"); 
   //create a json buffer where to store the json data 
   
   

  auto error = deserializeJson(doc2, line2);
  if (error) {
      Serial.print(F("deserializeJson() failed with code "));
      Serial.println(error.c_str());
      return;
  }
 }
  //JsonObject root = doc.to<JsonObject>(); 
 //get the data from the json tree 
 float nextWeatherTime0 = doc["main"]["temp"]; 
 float nextWeatherTime1 = doc["main"]["temp_min"]; 
 float nextWeatherTime2 = doc["main"]["temp_max"]; 
 float ninehours = doc2["list"][3]["main"]["temp"];
 float twelvehours = doc2["list"][5]["main"]["temp"];
 String ninehdate = doc2["list"][3]["dt_txt"];
 String twelvehdate = doc2["list"][5]["dt_txt"];
 utime = (unsigned long)doc2["list"][3]["dt"];
 wkday = dow(utime);
 utime = (unsigned long)doc2["list"][5]["dt"];
 wkday2 = dow(utime);
 
  nextWeatherTime0+=0.5;
  nextWeatherTime1+=0.5;
  nextWeatherTime2+=0.5;
  ninehours+=0.5;
  twelvehours+=0.5;
  Serial.println(ninehours);
 String nextWeather0 = doc["weather"][0]["icon"];
 int nextWeatherid = doc["weather"][0]["id"];
 String nextWeather1 = doc2["list"][3]["weather"][0]["icon"];
 int nextWeatherid1 = doc2["list"][3]["weather"][0]["id"];
 String nextWeather2 = doc2["list"][5]["weather"][0]["icon"];
 int nextWeatherid2 = doc2["list"][5]["weather"][0]["icon"];
 Serial.println(nextWeather0);
 int todaytemp=(int)nextWeatherTime2;
 int tomorrowtemp=(int)ninehours;
 int thirddaytemp=(int)twelvehours;
 int todaylow=(int)nextWeatherTime1;
 int tomorrowlow=65;
 int thirddaylow=71;
 int current=(int)nextWeatherTime0;
 uint8_t* todayicon=iconselect(nextWeather0, nextWeatherid);
 uint8_t* tomorrowicon=iconselect(nextWeather1, nextWeatherid1);
 uint8_t* thirdicon=iconselect(nextWeather2, nextWeatherid2);
 display.begin();

 display.clearBuffer();
 Serial.println("buffer cleared");
 drawTodayWeather(todayicon,current ,todaytemp,  todaylow);
 threeDay(tomorrowicon,thirdicon,tomorrowtemp, thirddaytemp, ninehdate, twelvehdate);
 Serial.println("display.display now delay");
 delay(1000); 
 Serial.println("going to sleep");
 esp_sleep_enable_timer_wakeup(36e8);
 Serial.flush(); 
 esp_deep_sleep_start();
}



void drawTodayWeather(uint8_t* icon,int current ,int todaytemp,  int todaylow) {
  Serial.println("drawtodayweather");
  char high1[10];
  sprintf(high1, "High: %d",todaytemp);
  char low1[10];
  sprintf(low1, "Low: %d",todaylow);
  char currenttemp[10];
  sprintf(currenttemp, "Curr: %d",current);
  // large block of text
  if (current > 78){
    curcolor=COLOR2;
  } else {
    curcolor=COLOR1;
  }
  if (todaytemp > 78){
    todayhighcol=COLOR2;

  } else {
    todayhighcol=COLOR1;

  }
  
  if (todaylow > 78){
    todaylowcol=COLOR2;
  } else {
    todaylowcol=COLOR1;
  }
  
  testdrawtext(currenttemp,5,5, curcolor);
  testdrawtext(high1,5,20, todayhighcol);
  testdrawtext(low1,5,35, todaylowcol);
  display.drawBitmap(5, 50, icon, 50, 50, todayhighcol); 
  display.drawRect(0, 0, 65, 110, COLOR1);
  //display.display();
}

void threeDay(uint8_t* icon2,uint8_t* icon3,int tomorrowtemp, int thirddaytemp, String ninehdate, String twelvehdate){
  //display.begin();
  //display.clearBuffer();
  Serial.println("threeday");
  char temp2[10];
  char temp3[10];
  sprintf(temp2, "Temp: %d",tomorrowtemp);
  sprintf(temp3, "Temp: %d",thirddaytemp);
  char time2[10];
  char time3[10];
  char nextup[11];
  char lastup[11];
  //String day9 = ninehdate.substring(0,10);
  String day9 = WDAY_NAMES[wkday];
  String time9 = ninehdate.substring(11,16);
  String day12 = WDAY_NAMES[wkday2];
  String time12 = twelvehdate.substring(11,16);
  time9.toCharArray(time2,10);
  time12.toCharArray(time3,10);
  day9.toCharArray(nextup,11);
  day12.toCharArray(lastup,11);
  Serial.println(time2);
  if (tomorrowtemp > 78){
    tomhighcol=COLOR2;

  } else {
    tomhighcol=COLOR1;

  }
  if (thirddaytemp > 78){
    nexthighcol=COLOR2;

  } else {
    nexthighcol=COLOR1;

  }
  
  testdrawtext(nextup,75,5, COLOR1);
  testdrawtext(time2,75,20, COLOR1);
  testdrawtext(temp2,75,35, tomhighcol);

  testdrawtext(lastup,150,5, COLOR1);
  testdrawtext(time3,150,20, COLOR1);
  testdrawtext(temp3,150,35, nexthighcol);
  
  display.drawBitmap(75, 50, icon2, 50, 50, tomhighcol);
  display.drawBitmap(150, 50, icon3, 50, 50, nexthighcol);
  display.drawRect(65, 0, 75, 110, COLOR1);
  display.drawRect(140, 0, 75, 110, COLOR1);
  display.display();
}

void testdrawtext(char *text, int x, int y, uint16_t color) {
  display.setCursor(x, y);
  display.setTextColor(color);
  display.setTextWrap(true);
  display.print(text);
}


void loop() {
  
  
}
