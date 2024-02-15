#include <LiquidCrystal.h>

class Display
{
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
        : lcd(rs, en, d4, d5, d6, d7)
    {
        lcd.begin(width, height);
    }

    // Sets the text displayed on the first line of the LCD.
    void setLine1(String text)
    {
        lcd.setCursor(0, 0);
        lcd.print(text);
    }

    // Sets the text displayed on the second line of the LCD.
    void setLine2(String text)
    {
        lcd.setCursor(0, 1);
        lcd.print(text);
    }

    // Sets the text displayed on the LCD.
    void updateDisplay(String line1, String line2)
    {
        lcd.clear();
        setLine1(line1);
        setLine2(line2);
    }
};