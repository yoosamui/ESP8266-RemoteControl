class TimerOnOff
{

  public:
    int _hour;
    int _minute;
    int _mode; // 0 = off = 1 on
    String _targetIds;
    int _isSet = 0;


    TimerOnOff(int hour, int minute, int mode, String targetIds)
    {
      _hour = hour;
      _minute = minute;
      _mode = mode;
      _targetIds = targetIds;

    }



};


int _timerOnOff_testHour = 21;
int _timerOnOff_testMinute = 0;
TimerOnOff* timerOnOffList[] =
{
  // Dummy blink work room lamp
  new TimerOnOff(23, 0, 1, "9"),


  new TimerOnOff(_timerOnOff_testHour, _timerOnOff_testMinute, 1, "2"),
  new TimerOnOff(_timerOnOff_testHour, _timerOnOff_testMinute, 0, "2"),
  new TimerOnOff(_timerOnOff_testHour, _timerOnOff_testMinute, 1, "2"),
  new TimerOnOff(_timerOnOff_testHour, _timerOnOff_testMinute, 0, "2"),
  new TimerOnOff(_timerOnOff_testHour, _timerOnOff_testMinute, 1, "2"),

  // wall lamps
  new TimerOnOff(18, 30, 1, "8"),

  // standard downstairs Day Night
  new TimerOnOff(18, 31, 1, "4,5,6,3"),
  new TimerOnOff(5, 0, 0, "4,5,6,3")

};

