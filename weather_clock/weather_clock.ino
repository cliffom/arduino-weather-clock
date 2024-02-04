#include "Arduino.h"
#include "DHT11.h"
#include "LiquidCrystal.h"
#include "uRTCLib.h"

class Datetime {
private:
  // Real-Time Clock (RTC) library instance for accessing time data.
  uRTCLib rtc;

  // Array of day names in shorthand format, used for formatting dates.
  const String daysOfTheWeek[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

  // Array of month names in shorthand format, used for formatting dates.
  const String monthsOfTheYear[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

public:
  // Constructor initializes the RTC with a default address.
  Datetime()
    : rtc(0x68) {
    // Starts communication with the RTC module.
    URTCLIB_WIRE.begin();
  }

  // Sets the current date and time on the RTC module.
  void set(int sec, int min, int hour, int dayOfWeek, int dayOfMonth, int month, int year) {
    rtc.set(sec, min, hour, dayOfWeek, dayOfMonth, month, year);  // Sets the time and date.
  }

  // Refreshes the RTC data to ensure it has the latest date and time.
  void refresh() {
    rtc.refresh();
  }

  // Returns a string representation of the current date in a human-readable format.
  String dateToString() {
    String day = String(rtc.day(), DEC);
    String year = String(rtc.year(), DEC);

    // Pad single-digit days with a leading zero.
    if (day.length() == 1)
      day = "0" + day;

    // Constructs the date string using day names and month names arrays.
    String date = daysOfTheWeek[rtc.dayOfWeek() - 1] + " " + monthsOfTheYear[rtc.month() - 1] + " " + day + " " + year;

    return date;
  }

  // Returns a string representation of the current time in HH:MM format.
  String timeToString() {
    String hours = String(rtc.hour(), DEC);
    String minutes = String(rtc.minute(), DEC);

    // Pad single-digit hours and minutes with leading zeros.
    if (hours.length() == 1)
      hours = "0" + hours;
    if (minutes.length() == 1)
      minutes = "0" + minutes;

    // Constructs the time string in HH:MM format.
    String time = hours + ":" + minutes;

    return time;
  }

  // Returns the current seconds from the RTC.
  int seconds() {
    return rtc.second();
  }
};

class Weather {
private:
  DHT11 dht11;      // DHT11 sensor instance for reading temperature and humidity.
  int Temperature;  // Variable to store the last read temperature value.
  int Humidity;     // Variable to store the last read humidity value.

public:
  // Constructor initializes the DHT11 sensor with the given pin.
  Weather(int pin)
    : dht11(pin) {}

  // Reads and updates the current temperature and humidity from the DHT11 sensor.
  void update() {
    Temperature = dht11.readTemperature();
    Humidity = dht11.readHumidity();
  }

  // Returns the last read temperature value.
  int getTemperature() {
    return Temperature;
  }

  // Returns the last read humidity value.
  int getHumidity() {
    return Humidity;
  }

  // Returns a string representation of the current weather data in "temperatureC humidity%" format.
  String toString() {
    String temperature = String(Temperature, DEC);
    String humidity = String(Humidity, DEC);

    return temperature + "C " + humidity + "%";
  }
};

class Display {
private:
  // Instance of LiquidCrystal for LCD control.
  LiquidCrystal lcd;

  // Width of the LCD, in characters.
  const int width = 16;

  // Height of the LCD, in lines.
  const int height = 2;

public:
  // Constructor initializes the LCD display with specified pins and dimensions.
  Display(int rs, int en, int d4, int d5, int d6, int d7)
    : lcd(rs, en, d4, d5, d6, d7) {
    lcd.begin(width, height);
  }

  // Sets the text displayed on the first line of the LCD.
  void setLine1(String text) {
    lcd.setCursor(0, 0);
    lcd.print(text);
  }

  // Sets the text displayed on the second line of the LCD.
  void setLine2(String text) {
    lcd.setCursor(0, 1);
    lcd.print(text);
  }

  // Sets the text displayed on the LCD.
  void updateDisplay(String line1, String line2) {
    lcd.clear();
    setLine1(line1);
    setLine2(line2);
  }
};

// Initialize a new display with the following Arduino wiring:
// - rs=12
// - en=11
// - d4=10
// - d5=9
// - d3=8
// - d2=7
Display lcd(12, 11, 10, 9, 8, 7);

Weather weather(2);
Datetime datetime;

void setup() {
  Serial.begin(9600);
  Serial.println("Serial communication initialized");

  // uncomment below to set the time
  //datetime.set(0, 45, 11, 7, 3, 2, 24);

  weather.update();
  datetime.refresh();

  lcd.updateDisplay(
    datetime.dateToString(),
    datetime.timeToString() + " " + weather.toString());
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
    Serial.println(
      datetime.dateToString() + " " + datetime.timeToString() + ": Weather data updated");
  }

  if (datetime.seconds() == 0) {
    lcd.updateDisplay(
      datetime.dateToString(),
      datetime.timeToString() + " " + weather.toString());
  }

  delay(loopInterval);
}
