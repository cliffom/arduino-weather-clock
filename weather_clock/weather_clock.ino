#include "Arduino.h"
#include "DHT11.h"
#include "LiquidCrystal.h"
#include "uRTCLib.h"

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int lcdWidth = 16, lcdHeight = 2;

// initialize RTC library and constants
uRTCLib rtc(0x68);

// initialize DHT11 module on the provided pin (2)
DHT11 dht11(2);

// daysOfTheWeek is an array representation of the days of the week
// in shorthand format, starting with Sun
const String daysOfTheWeek[] = {
  "Sun",
  "Mon",
  "Tue",
  "Wed",
  "Thu",
  "Fri",
  "Sat"
};

// monthsOfTheYear is an array representation of the months of the year
// in shorthand format
const String monthsOfTheYear[] = {
  "Jan",
  "Feb",
  "Mar",
  "Apr",
  "May",
  "Jun",
  "Jul",
  "Aug",
  "Sep",
  "Oct",
  "Nov",
  "Dec"
};

struct DHT11Data {
  int Temperature, Humidity;
};
DHT11Data weather = { 0, 0 };

void setup() {
  Serial.begin(9600);

  // set up the LCD's number of columns and rows:
  lcd.begin(lcdWidth, lcdHeight);

  URTCLIB_WIRE.begin();

  // Comment out below line once you set the date & time.
  // Following line sets the RTC with an explicit date & time
  // for example to set January 13 2022 at 12:56 you would call:
  //rtc.set(0, 2, 9, 5, 2, 2, 24);
  rtc.refresh();
  updateWeather();
  display();
}

// loop runs constantly and does the following:
// - Refreshes data from the rtc module
// - clears the LCD and prints the updated date and time from the rtc module
// - gets the temperature and humidity data, once a minute at 0 seconds
// - delays for 1000ms (1-second)
//
// Please note, getting the temperature and humidy date invokes a small delay
// and when it occurs it will halt the loop for about a second longer than the
// 1000ms delay we already impose
void loop() {
  rtc.refresh();

  // Only update the display every minute
  if (rtc.second() == 0) {
    display();
  }

  // Poll for temp once a minute
  if (rtc.second() == 30) {
    updateWeather();
  }

  //delay(1000);
}

// display writes the date and weather date to the LCD display
void display() {
  lcd.clear();
  lcd.print(dateLine());
  lcd.setCursor(0, 1);
  lcd.print(timeLine());
  lcd.setCursor(6, 1);
  lcd.print(weatherLine());
}

// dateLine returns a string representation of the current day of the week,
// month of the year, day of the month, and year. Ex: Thu Feb 01 22
String dateLine() {
  String day = String(rtc.day(), DEC);
  String year = String(rtc.year(), DEC);

  // Pad with leading zeros
  if (day.length() == 1)
    day = "0" + day;

  // Construct dateLine
  String dateLine = daysOfTheWeek[rtc.dayOfWeek() - 1] + " " + monthsOfTheYear[rtc.month() - 1] + " " + day + " " + year;

  return dateLine;
}

// timeLine returns a string representation of the current time
// in the format hh:mm:ss - Ex: 12:24:01
String timeLine() {
  String hours = String(rtc.hour(), DEC);
  String minutes = String(rtc.minute(), DEC);
  String seconds = String(rtc.second(), DEC);

  // Pad with leading zeros
  if (hours.length() == 1)
    hours = "0" + hours;
  if (minutes.length() == 1)
    minutes = "0" + minutes;
  if (seconds.length() == 1)
    seconds = "0" + seconds;

  // Construct timeLine
  //String timeLine = hours + ":" + minutes + ":" + seconds;
  String timeLine = hours + ":" + minutes;

  return timeLine;
}

// weatherLine returns a string representation of the current
// temperature (in F) and humidity (as a %) - Ex: 75/50
String weatherLine() {
  String temperature = String(weather.Temperature, DEC);
  String humidity = String(weather.Humidity, DEC);

  return temperature + "C " + humidity + "%";
}

// updateWeather updates the temperature and humidity data from
// the DHT11 sensor
void updateWeather() {
  // Attempt to read the temperature and humidity values from the DHT11 sensor.
  int temperature = dht11.readTemperature();
  int humidity = dht11.readHumidity();

  weather = { temperature, humidity };
}