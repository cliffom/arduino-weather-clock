#include <DHT11.h>

class Climate {
private:
  DHT11 dht11;
  int Temperature;
  int Humidity;
  int SensorStatus = 0;
  String Location = "default";

public:
  // Constructor initializes the DHT11 sensor with the given pin.
  Climate(int pin, String location)
    : dht11(pin) {
    Location = location;
  }

  // Reads and updates the current temperature and humidity from the DHT11 sensor.
  void update() {
    SensorStatus = dht11.readTemperatureHumidity(Temperature, Humidity);
  }

  // Checks to see if either sensor value was an error value
  bool error() {
    return !(SensorStatus == 0);
  }

  // Gets a string representation of the sensor status
  String status() {
    if (!error())
      return "ok";
    else
      return DHT11::getErrorString(SensorStatus);
  }

  // Returns the temperature as of the last sensor reading
  int getTemperature() {
    if (error())
      return -1;

    return Temperature;
  }

  // Returns the humidity as of the last sensor reading
  int getHumidity() {
    if (error())
      return -1;

    return Humidity;
  }

  // Returns the location of the sensor
  String getLocation() {
    return Location;
  }

  // Returns a string representation of the current climate data in "temperatureC humidity%" format.
  String toString() {
    String temperature;
    String humidity;

    if (error()) {
      temperature = "--";
      humidity = "--";
    } else {
      temperature = String(Temperature, DEC);
      humidity = String(Humidity, DEC);
    }

    return temperature + "C " + humidity + "%";
  }
};