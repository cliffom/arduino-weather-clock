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

void setup() {
  Serial.begin(9600);

  // set up the LCD's number of columns and rows:
  lcd.begin(lcdWidth, lcdHeight);

  URTCLIB_WIRE.begin();

  // Comment out below line once you set the date & time.
  // Following line sets the RTC with an explicit date & time
  // for example to set January 13 2022 at 12:56 you would call:
  // rtc.set(0, 19, 17, 5, 1, 2, 24);
}

void loop() {
  rtc.refresh();
  lcd.clear();
  lcd.print(dateLine());
  lcd.setCursor(0, 1);
  lcd.print(timeLine());
  //dht11Test();
  delay(1000);
}

// dateLine returns a string representation of the current day of the week,
// month of the year, day of the month, and year. Ex: Thu Feb 01 22
String dateLine() {
  String day = String(rtc.day(), DEC);
  String year = String(rtc.year(), DEC);

  // Pad with leading zeros
  if (day.length() == 1) day = "0" + day;

  // Construct dateLine
  String dateLine = daysOfTheWeek[rtc.dayOfWeek() - 1] + " " +
                    monthsOfTheYear[rtc.month() - 1] + " " +
                    day + " " + year;

  return dateLine;
}

// timeLine returns a string representation of the current time
// in the format hh:mm:ss - Ex: 12:24:01
String timeLine() {
  String hours = String(rtc.hour(), DEC);
  String minutes = String(rtc.minute(), DEC);
  String seconds = String(rtc.second(), DEC);

  // Pad with leading zeros
  if (hours.length() == 1) hours = "0" + hours;
  if (minutes.length() == 1) minutes = "0" + minutes;
  if (seconds.length() == 1) seconds = "0" + seconds;

  // Construct timeLine
  String timeLine = hours + ":" + minutes + ":" + seconds;

  return timeLine;
}

void dht11Test()
{
    // Attempt to read the temperature and humidity values from the DHT11 sensor.
    int temperature = dht11.readTemperature();

    // If using ESP32 or ESP8266 (xtensa architecture), uncomment the delay below.
    // This ensures stable readings when calling methods consecutively.
    // delay(50);

    int humidity = dht11.readHumidity();

    // Check the results of the readings.
    // If there are no errors, print the temperature and humidity values.
    // If there are errors, print the appropriate error messages.
    if (temperature != DHT11::ERROR_CHECKSUM && temperature != DHT11::ERROR_TIMEOUT &&
        humidity != DHT11::ERROR_CHECKSUM && humidity != DHT11::ERROR_TIMEOUT)
    {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.println(" °C");

        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.println(" %");
    }
    else
    {
        if (temperature == DHT11::ERROR_TIMEOUT || temperature == DHT11::ERROR_CHECKSUM)
        {
            Serial.print("Temperature Reading Error: ");
            Serial.println(DHT11::getErrorString(temperature));
        }
        if (humidity == DHT11::ERROR_TIMEOUT || humidity == DHT11::ERROR_CHECKSUM)
        {
            Serial.print("Humidity Reading Error: ");
            Serial.println(DHT11::getErrorString(humidity));
        }
    }
}