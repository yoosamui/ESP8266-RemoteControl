#define WIFI_SSID "CORAL_NET"
#define WIFI_PASSWORD "920Pt_OnBQ"
#define IPADDRESS IPAddress(192, 168, 78, 160)
#define GATEWAY IPAddress(192, 168, 78, 1)
#define NETMASK IPAddress(255, 255, 255, 0)
#define HTTPPORT  80

//---- NTP ------
#define NTP_TIMEZONE +07
#define UDP_LOCALPORT 2390 
#define NTPSERVERNAME  "time.nist.gov"
#define NTP_PACKET_SIZE 48            // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE];  //buffer to hold incoming and outgoing packets
IPAddress timeServerIP;               // time.nist.gov NTP server address

#define PIN_RF 14      // RF 430.92Mghz. sender PIN  ESP-03

