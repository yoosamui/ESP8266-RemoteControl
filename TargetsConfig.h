
#ifndef TargetsConfig_h
#define TargetsConfig_h
//http://www.androidhive.info/2012/02/android-custom-listview-with-image-and-text/
class Targets;
class WiFiClient;
class TimerOnOff;


void processDaiking( Targets* t,const WiFiClient client);
//void processMitsubishiMrSlim( Targets* t,const WiFiClient client);
void processLamp1( Targets* t,const WiFiClient client);
void processLamp2( Targets* t,const WiFiClient client);
void processLs( Targets* t,const WiFiClient client);
void processXml( Targets* t, WiFiClient client);
void processTime( Targets* t,  WiFiClient client);

void processState( Targets* t,const WiFiClient client);
void processRedFan( Targets* t, const WiFiClient client);
void processRF10( Targets* t, const WiFiClient client);
void processRF11( Targets* t, const WiFiClient client);
void processRF12( Targets* t, const WiFiClient client);
void processWallLamps( Targets* t,  WiFiClient client);
void processBlinkTest( Targets* t,  WiFiClient client);



typedef std::function<void(Targets*,WiFiClient client)> tdCallback;

class Targets
{
  public:
//  new Targets("shows the current time stamp ","time stamp",
//	"/time","0",0,0, processTime),

    Targets(String caption, String subtitle, String request,String Id,
	 const unsigned int onCode,  const unsigned int offCode, const tdCallback& process)
    {
	this->_caption = caption;
	this->_request =  request;

	this->_onCode =  onCode;
	this->_offCode =  offCode;

	this->_process = process;

	this->_state = 0;
	this->_mode = 0;
	this->_id = Id;
	this->_subtitle = subtitle;


    };
    unsigned int _onCode;
    unsigned int _offCode;
    String _caption;
    String _subtitle;
    String _request;
    String _id;
		
    int _mode; // 0 = off, 1 = on	
    int _state; // 0 = off, 1 = on	 
    tdCallback _process;


    String toString()
    {
        String smode = _mode == 1 ?"/on":"/off";
	return _caption+"="+_request+smode;
    }
	
    



	
};

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

class TimerOnOff
{

 public:
    int _hour;
    int _minute;
    int _mode; // 0 = off = 1 on
    String _targetIds;
    int _isSet = 0;	


	TimerOnOff(int hour, int minute, int mode,String targetIds)
	{
		_hour = hour;
		_minute = minute;
		_mode = mode;
    		_targetIds = targetIds;

	}
	

	
};

Targets* tlist[] =
{
  new Targets("get xml output","n/a","/xml","0",0,0, processXml),
  new Targets("Show request list","n/a","/ls","0",0,0, processLs),
  new Targets("Show request status","n/a","/state","0",0,0, processState),

  new Targets("Daiking aircondition","House lamai upstairs aircondition",
	"/ac","1",0,0, processDaiking),

  new Targets("shows the current time stamp ","time stamp",
	"/time","0",0,0, processTime),





  new Targets("Workroom Lamp 1","Juan workroom Lamp1",
	"/rf13","2",10000013,13000000,NULL),
  new Targets("Bedroom Red Lamp","Sup Bedroom Lamp",
	"/rf14","3",10000014,14000000,NULL),
  new Targets("Table Lamp 4","downstairs living room near sofa",
	"/rf10","4", 10000010,10000000, NULL),
  new Targets("Table Lamp 5","downstairs living room near WC",
	"/rf11","5",10000011,11000000, NULL),
  new Targets("Table Lamp 6","Upstairs living room near WC",
	"/rf12","6",10000012,12000000, NULL),

  new Targets("Empty","Empty",
	"/rf15","7",10000015,15000000, NULL),

  // Wall Lamps turn on/off all wall lamps
  new Targets("Wall lamps","Wall",
  	"/rf16","8",0,0, processWallLamps),

  new Targets("Wall lamp17","Wall",
	"/rf17","10",10000025,12100000, NULL),
  new Targets("Wall lamp18","Wall",
	"/rf18","11",10000125,12200000, NULL),
  new Targets("Wall lamp19","Wall",
	"/rf19","12",10000027,12300000, NULL),
  new Targets("Wall lamp20","Wall",
	"/rf20","13",10000028,12400000, NULL),
  new Targets("Wall lamp21","Wall",
	"/rf21","14",10000029,12500000, NULL),
  new Targets("Wall lamp22","Wall",
	"/rf22","15",10000030,12600000, NULL),

 //New models for the wall
  new Targets("Wall lamp24","Wall",
	//"/rf24","16",10000031,14100000, NULL),
	"/rf24","16",10000124,14000000, NULL),
  new Targets("Wall lamp25","Wall",
	"/rf25","17",10000126,14200000, NULL),
  new Targets("Wall lamp26","Wall",
	"/rf26","18",10000127,14300000, NULL),


  // Free

  new Targets("Free lamp27","Free",
	"/rf27","19",10000142,14400000, NULL),
  new Targets("Free lamp28","Free",
	"/rf28","20",10000143,14500000, NULL),
  new Targets("Free lamp29","Free",
	"/rf29","29",10000155,14600000, NULL),
  new Targets("Free lamp30","Free",
	"/rf30","30",10000156,14700000, NULL),
  new Targets("Free lamp31","Free",
	"/rf31","31",10000157,14800000, NULL),
  new Targets("Free lamp32","Free",
	"/rf32","32",10000158,14900000, NULL),
  new Targets("Free lamp33","Free",
	"/rf33","33",10000159,15000000, NULL),



 	


 //Dummy blink
  new Targets("Dummy blink work room lamp","Dummy test",
	"/rf23","9",0,0, processBlinkTest)




  //new Targets("Red Fan Nathon","/rfan", processRedFan),
//  new Targets("Mitsubishi lamai juan workroom","/ac2", processMitsubishiMrSlim),
};

int _timerOnOff_testHour = 21;
int _timerOnOff_testMinute = 30;

TimerOnOff* timerOnOffList[] = 
{
	// Dummy blink work room lamp
	new TimerOnOff(23,0,1,"9"),


	new TimerOnOff(_timerOnOff_testHour,_timerOnOff_testMinute,1,"2"),
	new TimerOnOff(_timerOnOff_testHour,_timerOnOff_testMinute,0,"2"),
	new TimerOnOff(_timerOnOff_testHour,_timerOnOff_testMinute,1,"2"),
	new TimerOnOff(_timerOnOff_testHour,_timerOnOff_testMinute,0,"2"),
	new TimerOnOff(_timerOnOff_testHour,_timerOnOff_testMinute,1,"2"),

	// wall lamps
	new TimerOnOff(18,31,1,"8"),

	// Day Night
	new TimerOnOff(18,30,1,"4,5,6,3"),
	new TimerOnOff(5,0,0,"4,5,6,3")



};





#endif
