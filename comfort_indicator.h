class ComfortIndicator
{
private:
    int BlueLED;
    int GreenLED;
    int RedLED;
    const int Intensity = 64;

public:
    // Constructor that tells the indicator what pins to use for LEDs.
    ComfortIndicator(int bluePin, int greenPin, int redPin)
    {
        BlueLED = bluePin;
        GreenLED = greenPin;
        RedLED = redPin;

        reset();
    }

    // Sets all pins to LOW/no signal
    void reset()
    {
        analogWrite(BlueLED, 0);
        analogWrite(GreenLED, 0);
        analogWrite(RedLED, 0);
    }

    // Sets which LED to enable based on a rough temperate range of:
    // - Hot when temperature >=30
    // - Normal when temperature < 30 but >= 21
    // - Cold when temperature <21
    void display(int temperature)
    {
        reset();
        if (temperature >= 25)
            analogWrite(RedLED, Intensity);
        else if (temperature >= 21)
            analogWrite(GreenLED, Intensity);
        else
            analogWrite(BlueLED, Intensity);
    }

    // Turns the LED yellow to indicate that there was an issue
    void displayWarning()
    {
        reset();
        analogWrite(RedLED, Intensity);
        analogWrite(GreenLED, Intensity);
    }
};