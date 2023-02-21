#include <Arduino.h>
#include <SD.h>
#include <FS.h>
#include <ESP8266WiFi.h>
#include "ESP-FTP-Server-Lib.h"
#include "FTPFilesystem.h"

#define WIFI_SSID "ssid"
#define WIFI_PASSWORD "password"
#define FTP_USER "ftp"
#define FTP_PASSWORD "ftp"

FTPServer ftp;

void setup()
{
	Serial.begin(115200);

	if (!SD.begin(13)) {
	Serial.println("No SD...");
	}
	else{
		
		SD.begin(13);
	}
SPIFFS.begin();

	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.println(WIFI_SSID);
	Serial.println(WiFi.localIP());
    ftp.addUser(FTP_USER, FTP_PASSWORD);
	ftp.addFilesystem("SPIFFS", &SPIFFS);

	ftp.begin();
}

void loop()
{
	ftp.handle();
}