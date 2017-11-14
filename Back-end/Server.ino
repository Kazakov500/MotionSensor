/*
 * Copyright (c) 2015, Majenko Technologies
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char *ssid = "KV";
const char *password = "hksi6088";

ESP8266WebServer server ( 80 );

const int led = 13;

void handleRoot() {
	digitalWrite ( led, 1 );
	char temp[400];
	int sec = millis() / 1000;
	int min = sec / 60;
	int hr = min / 60;

	snprintf ( temp, 400,
//<meta http-equiv='refresh' content='1'/>
"<html>\
  <head>\
    <title>ESP8266 Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
    <script src=\"//ajax.googleapis.com/ajax/libs/jquery/3.1.0/jquery.min.js\"></script>\
    <script>\
    var img = $(\"<img />\").attr('src', '/test.svg')\
    .on('load', function() {\
            $(\"#something\").append(img);\
    });\
    </script>\
  </head>\
  <body>\
    <h1>Hello from ESP8266!</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <img id = \"something\" src=\"/test.svg\" />\
  </body>\
</html>",

		hr, min % 60, sec % 60
	);
	server.send ( 200, "text/html", temp );
	//digitalWrite ( led, 0 );
}

void handleNotFound() {
	//digitalWrite ( led, 1 );
	String message = "File Not Found\n\n";
	message += "URI: ";
	message += server.uri();
	message += "\nMethod: ";
	message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server.args();
	message += "\n";

	for ( uint8_t i = 0; i < server.args(); i++ ) {
		message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
	}

	server.send ( 404, "text/plain", message );
	//digitalWrite ( led, 0 );
}

void setup ( void ) {
	pinMode ( led, OUTPUT );
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
	digitalWrite ( led, 0 );
	Serial.begin ( 115200 );
	WiFi.begin ( ssid, password );
	Serial.println ( "" );

	// Wait for connection
	while ( WiFi.status() != WL_CONNECTED ) {
		delay ( 500 );
		Serial.print ( "." );
	}

	Serial.println ( "" );
	Serial.print ( "Connected to " );
	Serial.println ( ssid );
	Serial.print ( "IP address: " );
	Serial.println ( WiFi.localIP() );

	if ( MDNS.begin ( "esp8266" ) ) {
		Serial.println ( "MDNS responder started" );
	}

	server.on ( "/", handleRoot );
	server.on ( "/test.svg", drawGraph );//Get graph SVG
  server.on ( "/getValue.html", GetCurrentValue );
  server.on ( "/control.html", ControlPage );
	server.on ( "/inline", []() {
		server.send ( 200, "text/plain", "this works as well" );
	} );
	server.onNotFound ( handleNotFound );
	server.begin();
	Serial.println ( "HTTP server started" );
}

int Sig[380] = {0};
int curr_pos = 0;
int deley_iter = 0;
int A0_a;
void loop ( void ) 
{
	  server.handleClient();

    /*
    deley_iter = 0;
    A0_a = analogRead(A0); 

    for(int i = 0; i <= 378; i++)
    {
      Sig[i] = Sig[i+1];
    }
    Sig[379] = A0_a*0.2;

    delay(100);
  
    //digitalWrite ( led, 0 );
    */
}

void GetCurrentValue()
{

  int Curr_A0 = analogRead(A0);
  String out = String(Curr_A0, DEC);
  
  out += Curr_A0;
  server.send ( 200, "text/html", out);    
  
}

void ControlPage()
{
  String out = "";
  char temp[100];
  out += 
"<html>\
  <head>\
    <title>ESP8266 Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <a href=\"/control.html?led=1\">Включить диод</a><br>\
    <a href=\"/control.html?led=0\">Выключить сраный диод</a>\
  </body>\
</html>";
  server.send ( 200, "text/html", out);    

  if(server.arg("led") == "1" ) 
  {
    Serial.println ( "Led ON" );
    Serial.println ( analogRead(0) );//Read data from A0 pin end send to Serial port
  }
  else 
  {

    Serial.println ( "Led OFF" );
  }

  
}

void drawGraph() {
  //digitalWrite ( led, 0 );
	String out = "";
	char temp[100];
	out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
 	out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
 	out += "<g stroke=\"black\">\n";
   /*
 	int y = analogRead(0);
 	for (int x = 10; x < 390; x+= 10) {
 		int y2 = analogRead(0);
 		sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
 		*/

  int y = Sig[0];
  for (int x = 0; x < 380; x++) {
    int y2 = Sig[x];
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x+10, 140 - y, x + 1+10, 140 - y2);
 		
 		out += temp;
 		y = y2;
 	}
	out += "</g>\n</svg>\n";
	server.send ( 200, "image/svg+xml", out);
}
