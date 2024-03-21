
#include "SleepTimer.h"

SleepTimer::SleepTimer()
{
    if(DEBUGLEVEL >= DEBUG_OPERATIONS)
    {
        Serial.print(F("Creating SleepTimer at adress "));
        Serial.println((int)this, DEC);
    }

    turn            = TURN_NEUTRAL;
    turnOnEnabled   = false;
    turnOnFlag      = false;
    turnOffEnabled  = false;
    turnOffFlag     = false;
    time            = new struct tm;
    turnOnTime      = new struct tm;
    turnOffTime     = new struct tm;

    setTime(2023,3,2, 9,58);

    Serial.println(F("...SleepTimer Started"));
}

void SleepTimer::setTime(uint16_t year, uint8_t month, uint8_t day,   uint8_t hour, uint8_t minute)
{ 
    //Set own time
    time->tm_year  = year - 1900; //Year
    time->tm_mon   = month - 1; //Month (Do -1 or it will be incorrect)
    time->tm_mday  = day; //Day

    time->tm_hour  = hour;//Hour
    time->tm_min   = minute; //Minute

    time->tm_sec   = 0; //Second

    //Setting DayMonthYear for turnOnTime (Doesn't do anything but makes me feel safe against leap seconds)
    turnOnTime->tm_year     = time->tm_year;
    turnOnTime->tm_mon      = time->tm_mon;
    turnOnTime->tm_mday     = time->tm_mday;
    turnOnTime->tm_sec      = time->tm_sec;

    //Setting DayMonthYear for turnOffTime (Doesn't do anything but makes me feel safe against leap seconds)
    turnOffTime->tm_year    = time->tm_year;
    turnOffTime->tm_mon     = time->tm_mon;
    turnOffTime->tm_mday    = time->tm_mday;
    turnOffTime->tm_sec     = time->tm_sec;

    //struct timeval now = { .tv_sec = mktime(time) };
    //settimeofday(&now, NULL);
}

void SleepTimer::printTime()
{
    Serial.print(F("Current Time: "));
    //Serial.println(time, "%A, %B %d %Y %H:%M:%S");

    
    Serial.print(F("Turn On  Time: "));
    //Serial.println(turnOnTime, "%B %d %Y %H:%M:%S");

    Serial.print(F("Turn Off Time: "));
    //Serial.println(turnOffTime, "%B %d %Y %H:%M:%S");
}

void SleepTimer::tick()
{
    //getLocalTime(time);

    if (turnOnEnabled)
    {
        if(time->tm_hour == turnOnTime->tm_hour && time->tm_min == turnOnTime->tm_min)
        {
            if(!turnOnFlag)
            {
                turn = TURN_ON;
                turnOnFlag = true;
            }
        }
        else
        {
            turnOnFlag = false;
        }
    }

    if (turnOffEnabled)
    {
        if(time->tm_hour == turnOffTime->tm_hour && time->tm_min == turnOffTime->tm_min)
        {
            if(!turnOffFlag)
            {
                turn = TURN_OFF;
                turnOffFlag = true;
            }
        }
        else
        {
            turnOffFlag = false;
        }   
    }
}
uint8_t SleepTimer::getTurn()
{
    if (turn != TURN_NEUTRAL)
    {
        uint8_t turnToSend = turn;
        turn = TURN_NEUTRAL;
        return turnToSend;
    }
    return turn;
}
void SleepTimer::setTurnOnTime(uint8_t hour, uint8_t minute)
{
    turnOnTime->tm_hour = hour;
    turnOnTime->tm_min = minute;
}
void SleepTimer::setTurnOnEnabled(bool enabled)
{
    turnOnEnabled = enabled;
}
void SleepTimer::setTurnOffTime(uint8_t hour, uint8_t minute)
{
    turnOffTime->tm_hour = hour;
    turnOffTime->tm_min = minute;
}
void SleepTimer::setTurnOffEnabled(bool enabled)
{
    turnOffEnabled = enabled;
}