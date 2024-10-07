#include <rack.hpp>

class Clock
{

public:
  void readClock()
  {
    long unixTime = (long)rack::system::getUnixTime();

    // extract the time from the unix time
    struct tm *timeinfo = std::localtime(&unixTime);
    second = timeinfo->tm_sec;
    minute = timeinfo->tm_min;
    hour = timeinfo->tm_hour;
    day = timeinfo->tm_mday;
    weekday = timeinfo->tm_wday;
    month = timeinfo->tm_mon;
    year = timeinfo->tm_year;
  }

  int second = 0;
  int minute = 0;
  int hour = 0;
  int day = 0;
  int weekday = 0;
  int month = 0;
  int year = 0;

  Clock() {};
};
