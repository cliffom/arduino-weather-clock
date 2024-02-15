#include <uRTCLib.h>

class Datetime
{
private:
    // Real-Time Clock (RTC) library instance for accessing time data.
    uRTCLib rtc;

    // Array of day names in shorthand format, used for formatting dates.
    const String daysOfTheWeek[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

    // Array of month names in shorthand format, used for formatting dates.
    const String monthsOfTheYear[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

public:
    // Constructor initializes the RTC with a default address.
    Datetime()
        : rtc(0x68)
    {
        // Starts communication with the RTC module.
        URTCLIB_WIRE.begin();
    }

    // Sets the current date and time on the RTC module.
    void set(int sec, int min, int hour, int dayOfWeek, int dayOfMonth, int month, int year)
    {
        rtc.set(sec, min, hour, dayOfWeek, dayOfMonth, month, year); // Sets the time and date.
    }

    // Refreshes the RTC data to ensure it has the latest date and time.
    void refresh()
    {
        rtc.refresh();
    }

    // Returns a string representation of the current date in a human-readable format.
    String dateToString()
    {
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
    String timeToString()
    {
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
    int seconds()
    {
        return rtc.second();
    }
};