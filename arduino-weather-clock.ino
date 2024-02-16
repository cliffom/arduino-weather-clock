#include <Arduino.h>
#include <arduino-timer.h>

#include "datetime.h"
#include "climate.h"
#include "comfort_indicator.h"
#include "display.h"

// Initialize a new display with the following Arduino wiring:
// - rs=12
// - en=11
// - d4=10
// - d5=9
// - d3=8
// - d2=7
Display lcd(12, 11, 10, 9, 8, 7);

// Initialize our climate sensor on pin 2
Climate climate(2, "Office");
Datetime datetime;

// Initialize our comfort indicator on the provided pins (R,G,B)
ComfortIndicator comfortIndicator(6, 5, 4);

auto timer = timer_create_default();

void setup() {
  Serial.begin(9600);

  // uncomment below to set the time
  // datetime.set(0, 19, 12, 5, 15, 2, 24);

  datetime.refresh();
  climate.update();

  updateVisuals();
  sendDataToSerial();

  timer.every(1000, updateDateTimeAndDisplay);
  timer.every(60000, updateClimateAndSendToSerial);
}

void loop() {
  timer.tick();
}

// Refreshes the visual outputs, including:
// - LCD
// - Comfort Indicator
void updateVisuals() {
  if (climate.error())
    comfortIndicator.displayWarning();
  else
    comfortIndicator.display(climate.getTemperature());

  lcd.updateDisplay(
    datetime.dateToString(),
    datetime.timeToString() + " " + climate.toString());
}

// Sends data to the serial console in the format
// error:0,status:ok,location:office,temperature:25,humidity:16
void sendDataToSerial() {
  String temperature = String(climate.getTemperature(), DEC);
  String humidity = String(climate.getHumidity(), DEC);
  String error = String(climate.error(), DEC);

  Serial.println(
    "error:" + error + ",status:" + climate.status() + ",location:" + climate.getLocation() + ",temperature:" + temperature + ",humidity:" + humidity);
}

// Helper function to update the datetime and refresh the display
bool updateDateTimeAndDisplay(void *) {
  datetime.refresh();
  if (datetime.seconds() == 0)
    updateVisuals();

  return true;
}

// Helper function to update the climate and send the data to serial
bool updateClimateAndSendToSerial(void *) {
  climate.update();
  sendDataToSerial();

  return true;
}
