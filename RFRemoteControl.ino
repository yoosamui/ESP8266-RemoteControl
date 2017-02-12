
//*****************************************************************
//
//  Copyright (C) 2016 Juan R. Gonzalez
//  Created on November 26, 2016,
//  j-gonzalez@email.de
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//****************************************************************
//work with ESP-03

/*
  if can't connect to USB
  #sudo chmod a+rw /dev/ttyUSB0


  Power Cycling

  If you can’t upload, try flipping the switch to OFF,
  press the upload button, and flip the switch to ON.
  It’s tricky but as you you reset the board, it will be in bootloader mode
  so that you can upload code before it starts running the code that was uploaded.
*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <RCSwitch.h>

#include "Defines.h"
#include "SoftClock.h"
#include "Target.h"
#include "TargetList.h"
#include "TimerOnOff.h"

RCSwitch mySwitch = RCSwitch();
WiFiServer server(HTTPPORT);

WiFiUDP udp;    // A UDP instance to let us send and receive packets over UDP
SoftClock softclock;


uint8_t MAC_array[6];
char MAC_char[18];

//---------------------------------------------------------------------------
void setup()
{
  pinMode(PIN_RF, OUTPUT);
  digitalWrite(PIN_RF, LOW);

  Serial.begin(115200);
  Serial.print("Start Setup...");
  delay(100);

  // RF Switch
  // Transmitter is connected to Arduino Pin #10
  mySwitch.enableTransmit(PIN_RF);

  // Optional set pulse length.
  mySwitch.setPulseLength(220);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // static ip, gateway, netmask
  WiFi.config(IPADDRESS, GATEWAY, NETMASK);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");


  // Start the server
  server.begin();
  WiFiClient wifiClient = WiFiClient();
  wifiClient.connect(IPADDRESS, 80);

  IPAddress localip  = WiFi.localIP() ;
  IPAddress remoteip  = wifiClient.remoteIP() ;

  Serial.print("Wifi ESP IP ");
  Serial.println(localip);
  Serial.print("Remote IP ");
  Serial.println(remoteip);

  Serial.println("Starting UDP ");
  udp.begin(UDP_LOCALPORT);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());



  WiFi.macAddress(MAC_array);
  for (int i = 0; i < sizeof(MAC_array); ++i) {
    sprintf(MAC_char, "%s%02x:", MAC_char, MAC_array[i]);
  }

  Serial.println(MAC_char);

  digitalWrite(PIN_RF, LOW);
  Serial.println("Server started! wait for request...");

}

unsigned long previousMillis = 0;         // will store last time clock was updated
const long interval = 1000;               // interval at which the clock is updated (milliseconds)
//int lastTimeMinute = -1;                  // Last Time minute an alarm was set.
//---------------------------------------------------------------------------
void loop()
{

  // check to see if it's time to update the clock.
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    if ( !softclock.isSet )
    {
      GetTime();


  
  Serial.print("UPDATE FROM NTP ");
  
      
    }
    softclock.update();
    Serial.print(softclock.Hour);

  Serial.print(":");
  Serial.print(softclock.Minute);
  


  Serial.print(":");
  Serial.print(softclock.Second);
 Serial.println(" ");

    // save the last time we was here
    previousMillis = currentMillis;

    // Check for alarms
    for ( auto timer :  timerOnOffList )
    {
      if ( softclock.Hour == timer->_hour && softclock.Minute == timer->_minute && /*lastTimeMinute != timer->_minute*/ softclock.Second == 0)
      {

        Serial.println("in time...............");
        
        //lastTimeMinute = timer->_minute;
        timer->_isSet = 1;
        String token = "";
        int Ints[500] = {0};
        int index = 0;
        int maxlength = timer->_targetIds.length();

        // split _targetIds
        for (int i = 0 ; i < maxlength ; i++)
        {
          char c = timer->_targetIds[i];
          if ( c != ',' ) {
            token += c;
          }

          if ( c == ',' || i + 1 == maxlength ) {
            Ints[ index ] = atoi(token.c_str());
            token = "";
            index++;
          }

        } // split _targetIds

        for (int i = 0 ; i < index  ; i++)
        {
          String id = String(Ints[i]);

          //  Serial.println(id);

          Target* t = getTargetById( id );

          if ( t == NULL )
            continue;

          t->_mode = timer->_mode;

          // check if function pointer available
          if ( t->_process != NULL )
          {
            t->_process(t, server.available() );
          }
          else
          {
            executeRf(t);
           // printTargetsDebug(t);
          }
        }


      } // end of if time alarm
    } // end of for next
  } // end of time iterval


  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }


  // Wait until the client sends some data or time out
  //Serial.println("wait for available request...");

  long int time = millis();
  while (  (time + 10000) > millis())
  {
    if (client.available() )
      break;

    delay(10);
  }

  if (!client.available() )
    return;



  Serial.println("new client request");
  auto aclientip = client.remoteIP();
  Serial.println(aclientip);

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  bool found = false;
  int pos1 =  req.indexOf('/');

  if (pos1 == -1 || req.indexOf("GET") == -1 ||
      req.indexOf("/favicon.ico") != -1 ||
      req.indexOf("/apple-touch-icon.png") != -1 ||
      req.indexOf("/apple-touch-icon-precomposed.png") != -1)
  {
    ;
    //bad request swallow!
  }
  else
  {
    IPAddress allowed1 = IPAddress(192, 168, 78, 2);
    IPAddress allowed2 = IPAddress(192, 168, 78, 3);
    IPAddress allowed3 = IPAddress(192, 168, 78, 5);
    IPAddress allowed4 = IPAddress(192, 168, 78, 111);
    if ( aclientip != allowed1 && aclientip != allowed2 && aclientip != allowed3 && aclientip != allowed4)
    {
      serveResponse(client, "ACCESS DENIED!");
      client.stop();
      return;
    }

    // process the request
    char *p = (char*)req.c_str();//getrequest;
    int i = 0;
    int c = 0;
    p++;

    while ( p != NULL)
    {
      if ( *p == '/' )
        c++;

      i++;
      p++;

      if ( c == 2 )
        break;
    }

    String Xtarget = "";
    if ( pos1 > 1 && i > pos1 )
    {
      Xtarget = req.substring(pos1, i) ;
    }

    for ( auto t :  tlist )
    {
      if (Xtarget ==  t->_request )
      {

        t->_mode = req.lastIndexOf( t->_request + "/on") != -1 ? 1 : 0 ;
        Serial.print("command found: " + t->_request);
        Serial.print(t->_mode == 1 ? "/on" : "/off");
        Serial.println(" ");

        if ( t->_process != NULL )
        {
          t->_process(t, client);

        }
        else
        {

          if ( t->_mode == 1 )
          {
            executeRf(t);
          }
          else
          {
            executeRf(t);
          }
          serveResponse(client, t->toString());
        }

        found = true;
        break;
      }

    }

  } // for end list


  if ( !found)
    serveResponse(client, "unknown command!");

  client.stop();

}

Target* getTargetById( String id )
{

  for ( auto t :  tlist ) {
    if ( t->_id == id )
      return t;
  }

  return NULL;
}

/**
  executeRf
*/
void executeRf( Target * t )
{

  if (t->_process != NULL )
  {
    WiFiClient xclient;
    t->_process(t, xclient);
    return;
  }

  if ( t->_mode == 1 )
  {
    t->_mode = 1;
    t->_state = 1;
    digitalWrite(PIN_RF, HIGH);
    mySwitch.send(t->_onCode , 24);


  }
  else
  {
    t->_mode = 0;
    t->_state = 0;
    digitalWrite(PIN_RF, LOW);
    mySwitch.send(t->_offCode, 24);

  }

  printTargetsDebug(t);
  delay(50);
}

void printTargetsDebug(Target * t)
{
  Serial.println( t->toString() );
}

void processBlinkTest( Target * target,  WiFiClient client)
{

  Target* result  = NULL;
  for ( auto t :  tlist )
  {
    if (t->_id == "2" ) // juan work room lamp
    {
      result = t;
      break;
    }
  }

  if ( result == NULL )
    return;

  int cmode = result->_mode;
  for (int i = 0 ; i < 24 ; i++)
  {
    result->_mode = i % 2;
    executeRf(result);


  }

  result->_mode = cmode ;
  executeRf(result);


  if ( client ) {
    serveResponse(client, result->toString());
  }


}
void processXml( Target * t, WiFiClient client)
{
  // The max you can send at a time is about 1400 characters, but most Arduinos do not have enough SRAM for a buffer that big.
  // If you try that with an Uno, you would run out of SRAM.
  //Serial.print( t->_request );
  Serial.println("processXml");

  // TODO: Fix this in the android app
  String message = "HTTP/1.1 200 OK\n"; // REMOVE THIS THEN
  message += "<?xml version='1.0' encoding='UTF-8'?>\n";
  message += "<targets>\n";
  client.print(message);
  message = "";
  for ( auto t : tlist )
  {
    if (t->_id == "0")
      continue;

    String cstate = t->_state == 1 ? "On" : "Off";

    message += "\n<target>\n";
    message += "<id>" + t->_id;
    message += "</id>\n<title>" + t->_caption;
    message += "</title>\n<command>" + t->_request;
    message += "</command>\n<state>" + cstate;
    message += "</state>\n";
    message += "<subtitle>" + t->_subtitle;
    message += "</subtitle>\n";
    message += "</target>\n";

    client.print(message);
    message = "";
  }

  message = "</targets>\n";
  client.println(message);
}

/*********************************************************

   processWallLamps
 ********************************************************/
void processWallLamps( Target* target,  WiFiClient client)
{


  for ( auto t :  tlist )
  {

    // front right
    if (t->_request == "/rf17" ) {
      t->_mode = target->_mode;
      executeRf(t);
      printTargetsDebug(t);
      continue;
    }
    if (t->_request == "/rf18" ) {
      t->_mode = target->_mode;
      executeRf(t);
      printTargetsDebug(t);
      continue;
    }
    if (t->_request == "/rf19" ) {
      t->_mode = target->_mode;
      executeRf(t);
      printTargetsDebug(t);
      continue;
    }

    // Right side
    if (t->_request == "/rf20" ) {
      t->_mode = target->_mode;
      executeRf(t);
      printTargetsDebug(t);
      continue;
    }
    if (t->_request == "/rf21" ) {
      t->_mode = target->_mode;
      executeRf(t);
      printTargetsDebug(t);
      continue;
    }
    if (t->_request == "/rf22" ) {
      t->_mode = target->_mode;
      executeRf(t);
      printTargetsDebug(t);
      continue;
    }
    if (t->_request == "/rf24" ) {
      t->_mode = target->_mode;
      executeRf(t);
      printTargetsDebug(t);

      continue;
    }
    if (t->_request == "/rf25" ) {
      t->_mode = target->_mode;
      executeRf(t);
      printTargetsDebug(t);

      continue;
    }


  }

  if ( client  ) {
    serveResponse(client, target->toString());
  }


}
/*********************************************************

   serveResponse
 ********************************************************/
void serveResponse(WiFiClient client, const String message)
{
  if (!client)
    return;

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");  // the connection will be closed after completion of the response
  // client.println("Refresh: 10");  // refresh the page automatically every 5 sec
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<html><body><h1>");

  String content = message;
  client.println(content);
  client.println("</h1></body></html>");

}


void GetTime()
{
  //get a random server from the pool
  WiFi.hostByName(NTPSERVERNAME, timeServerIP);

  sendNTPpacket(timeServerIP); // send an NTP packet to a time server
  // wait to see if a reply is available
  delay(1000);

  int cb = udp.parsePacket();
  if (!cb) {
    Serial.println("NTP Time Fail");
    return;
  }

  // We've received a packet, read the data from it
  udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

  // the timestamp starts at byte 40 of the received packet and is four bytes,
  // or two words, long. First, esxtract the two words:
  unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
  unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);

  // combine the four bytes (two words) into a long integer
  // this is NTP time (seconds since Jan 1 1900):
  unsigned long secsSince1900 = highWord << 16 | lowWord;

  // now convert NTP time into everyday time:
  // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
  const unsigned long seventyYears = 2208988800UL;
  unsigned long epoch = secsSince1900 - seventyYears;

  //-- Send Epoch to ESP --
  byte hh = (epoch  % 86400L) / 3600;
  byte mm = (epoch  % 3600) / 60;
  byte ss = epoch % 60;

  int thehour =  (epoch  % 86400L) / 3600 + NTP_TIMEZONE;
  if ( thehour > 24 )
    thehour -= 24;

  int theMinutes = mm;
  int theSeconds = ss;


  Serial.print("Time from NTP updated");
  //Serial.println(thehour );

  softclock.setTime(thehour, mm, ss + 1);

  Serial.print(" ");
  Serial.print(thehour);
  Serial.print(":");
  Serial.print(theMinutes);
  Serial.print(":");
  Serial.println(theSeconds);


}


// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(IPAddress & address)
{
  ///Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}



