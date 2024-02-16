class ComfortIndicator {
private:
  int RedLED;
  int GreenLED;
  int BlueLED;
  const int Intensity = 64;

public:
  // Constructor that tells the indicator what pins to use for LEDs.
  ComfortIndicator(int redPin, int greenPin, int bluePin) {
    RedLED = redPin;
    GreenLED = greenPin;
    BlueLED = bluePin;

    reset();
  }

  // Sets all pins to LOW/no signal
  void reset() {
    analogWrite(RedLED, 0);
    analogWrite(GreenLED, 0);
    analogWrite(BlueLED, 0);
  }

  // Sets which LED to enable based on a rough temperate range of:
  // - Hot when temperature >= 25
  // - Normal when temperature < 25 but >= 21
  // - Cold when temperature < 21
  void display(int temperature) {
    reset();
    if (temperature >= 25)
      analogWrite(RedLED, Intensity);
    else if (temperature >= 21)
      analogWrite(GreenLED, Intensity);
    else
      analogWrite(BlueLED, Intensity);
  }

  // Turns the LED yellow to indicate that there was an issue
  void displayWarning() {
    reset();
    analogWrite(RedLED, Intensity);
    analogWrite(GreenLED, Intensity);
  }
};