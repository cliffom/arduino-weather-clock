#include "Arduino.h"
#include "uRTCLib.h"
#include "LiquidCrystal.h"

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int lcdWidth = 16;
const int lcdHeight = 2;

// initialize RTC library and constants
uRTCLib rtc(0x68);

const String daysOfTheWeek[] = {
  "Sun",
  "Mon",
  "Tue",
  "Wed",
  "Thu",
  "Fri",
  "Sat"
};

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
  // set up the LCD's number of columns and rows:
  lcd.begin(lcdWidth, lcdHeight);

  URTCLIB_WIRE.begin();

  // Comment out below line once you set the date & time.
  // Following line sets the RTC with an explicit date & time
  // for example to set January 13 2022 at 12:56 you would call:
  //rtc.set(0, 37, 12, 5, 13, 2, 22);
}

void loop() {
  rtc.refresh();
  lcd.clear();
  lcd.print(generateDateLine());
  lcd.setCursor(0, 1);
  lcd.print(generateTimeLine());
  delay(1000);
}

String generateDateLine() {
  String day = String(rtc.day(), DEC);
  String year = String(rtc.year(), DEC);

  // Construct dateLine
  String dateLine = daysOfTheWeek[rtc.dayOfWeek() - 1] + " " +
                    monthsOfTheYear[rtc.month() - 1] + " " +
                    day + " " + year;

  return dateLine;
}


String generateTimeLine() {
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

