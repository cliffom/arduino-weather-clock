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
  void set(int sec, int min, int hour, int dayOfWeek, int dayOfMonth, int month, int year);
  String dateToString();
  String timeToString();
  int seconds();
  void refresh();
};

void Datetime::set(int sec, int min, int hour, int dayOfWeek, int dayOfMonth, int month, int year) {
  rtc.set(sec, min, hour, dayOfWeek, dayOfMonth, month, year);
}

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

int Datetime::seconds() {
  return rtc.second();
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

int Weather::getTemperature() {
  return Temperature;
}

int Weather::getHumidity() {
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

Weather weather(2);
Datetime datetime;

void setup() {
  Serial.begin(9600);
  Serial.println("Serial communication initialized");

  // set up the LCD's number of columns and rows:
  lcd.begin(lcdWidth, lcdHeight);

  // uncomment below to set the time
  //datetime.set(0, 45, 11, 7, 3, 2, 24);

  weather.update();
  datetime.refresh();
  display(
    datetime.dateToString(),
    datetime.timeToString(),
    weather.toString()
  );
}

void loop() {
  const unsigned long weatherUpdateInterval = 60000;
  const unsigned long loopInterval = 1000;

  static unsigned long lastWeatherUpdate = 0;
  unsigned long currentMillis = millis();
  
  datetime.refresh();

  // Check if weatherUpdateInterval/1000 seconds have passed
  if (currentMillis - lastWeatherUpdate >= weatherUpdateInterval) {
    weather.update();
    lastWeatherUpdate = currentMillis;
    Serial.print(
      datetime.dateToString() + " " + 
      datetime.timeToString() + 
      ": Weather data updated"
    );
  }

  if (datetime.seconds() == 0) {
    display(
      datetime.dateToString(),
      datetime.timeToString(),
      weather.toString()
    );
  }

  delay(loopInterval);
}

// display writes the date and weather date to the LCD display
void display(const String& date, const String& time, const String& weather) {
  lcd.clear();
  lcd.print(date);
  lcd.setCursor(0, 1);
  lcd.print(time);
  lcd.setCursor(6, 1);
  lcd.print(weather);
}