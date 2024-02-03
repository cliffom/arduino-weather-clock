#include "Arduino.h"
#include "DHT11.h"
#include "LiquidCrystal.h"
#include "uRTCLib.h"

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

class Datetime {
private:
  uRTCLib rtc;
public:
  Datetime()
    : rtc(0x68) {
    URTCLIB_WIRE.begin();
  }
  String dateToString();
  String timeToString();
  void refresh();
};

String Datetime::dateToString() {
  String day = String(rtc.day(), DEC);
  String year = String(rtc.year(), DEC);

  // Pad with leading zeros
  if (day.length() == 1)
    day = "0" + day;

  // Construct date
  String date = daysOfTheWeek[rtc.dayOfWeek() - 1] + " " + monthsOfTheYear[rtc.month() - 1] + " " + day + " " + year;

  return date;
}

String Datetime::timeToString() {
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
  String time = hours + ":" + minutes;

  return time;
}

void Datetime::refresh() {
  rtc.refresh();
}

class Weather {
private:
  DHT11 dht11;
  int Temperature;
  int Humidity;

public:
  Weather(int pin)
    : dht11(pin) {}
  void update();
  int getTemperature();
  int getHumidity();
  String toString();
};

void Weather::update() {
  Temperature = dht11.readTemperature();
  Humidity = dht11.readHumidity();
}

Weather::getTemperature() {
  return Temperature;
}

Weather::getHumidity() {
  return Humidity;
}

String Weather::toString() {
  String temperature = String(Temperature, DEC);
  String humidity = String(Humidity, DEC);

  return temperature + "C " + humidity + "%";
}

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int lcdWidth = 16, lcdHeight = 2;

Weather* weather;
Datetime* datetime;

void setup() {
  Serial.begin(9600);

  weather = new Weather(2);
  datetime = new Datetime();

  weather->update();
}

void loop() {
  static unsigned long lastWeatherUpdate = 0;
  unsigned long currentMillis = millis();

  // Check if 60 seconds have passed
  if (currentMillis - lastWeatherUpdate >= 60000) {
    weather->update();
    lastWeatherUpdate = currentMillis;
    Serial.println("Weather data updated.");
  }

  datetime->refresh();
  display(datetime->dateToString(), datetime->timeToString(), weather->toString());

  // Print the weather and datetime information
  Serial.println(weather->toString());
  Serial.println(datetime->dateToString());
  Serial.println(datetime->timeToString());

  delay(1000);
}

// display writes the date and weather date to the LCD display
void display(String date, String time, String weather) {
  lcd.clear();
  lcd.print(date);
  lcd.setCursor(0, 1);
  lcd.print(time);
  lcd.setCursor(6, 1);
  lcd.print(weather);
}