
#define LOADFROM_NTP_INTERVAL 60 * 5
class SoftClock
{
private: 
  
  int reloadinsecs = 0;
public:
  unsigned int Hour;
  unsigned int Minute;
  unsigned int Second;
  bool isSet = false;

  void setTime(unsigned int h, unsigned int m, unsigned int s)
  {
    this->Hour = h;
    this->Minute = m;
    this->Second = s;

    isSet = true;
    
    reloadinsecs = LOADFROM_NTP_INTERVAL;
  }

  void update()
  {
    if(this->Second++ > 59 ) { this->Minute++; this->Second = 0; }
    if(this->Minute > 59 ) { this->Hour++; this->Minute = 0; }
    if(this->Hour > 23 ) { this->Hour = 0; this->Minute = 0; this->Second = 0; }
  
    if(reloadinsecs-- <= 0 && this->Minute > 0 ) 
      isSet = false;

  }


};
