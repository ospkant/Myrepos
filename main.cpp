#include <FS.h>
#include <ESP8266WiFi.h>
#include "ESP-FTP-Server-Lib.h"
#include "FTPFilesystem.h"
#include <GyverOLED.h>
//#include <SDfat.h>
#include <SD.h>
#include <LittleFS.h>

#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"

#define FTP_USER "ftp"
#define FTP_PASSWORD "ftp"

const uint8_t csPin = SS; //  SD shield подключен к пину SS (Slave Select). D8 (GPIO15) или D4 (GPIO2)

FTPServer ftp;
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled; // 1306 подключать к пинам D1 (GPIO5,SCL) и D2 (GPIO4,SDA)
//SdFat sd;

const char* fileNew;

 


void homeOLED(){ // очистка, масштаб текста
  oled.clear();       // очистка
  oled.setScale(1);   // масштаб текста (1..4)
  oled.home();        // курсор в 0,0
}

void setup()
{
	Serial.begin(9600);
	LittleFS.begin();
	
	oled.init();        // инициализация OLED
    homeOLED();
    oled.print("GRBL Wifi!");
    delay(1000);
	oled.setCursor(0, 3);
    oled.print("(c)WiFi_OSP v1.0");
	delay(1000);

	homeOLED();
	if(!SD.begin(csPin)) {
		homeOLED();
		oled.setCursor(0, 3);
		oled.print("Ошибка SD карты!");
  		Serial.println("Error initializing SD card");
  	return;
  	}
	
  	SD.begin(csPin);
	homeOLED();
	oled.setCursor(3, 2);
	oled.print("SD карта");
	oled.setCursor(0, 3);
	oled.print("Инициализирована");
  	Serial.println("SD Card is avalible...");


	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
	while (WiFi.status() != WL_CONNECTED)
	{
		homeOLED();
		oled.print(".");
		delay(500);
		Serial.print("░");

	}
	
	homeOLED();
	
    oled.print("Соед.установлено");
	oled.setCursor(0, 2);
	oled.print("IP адрес: ");
	oled.setCursor(0, 3);
	oled.println(WIFI_SSID);
	oled.setCursor(0, 4);
	oled.println(WiFi.localIP());
	
	Serial.println("");
	Serial.print("Connected to ");
	Serial.println(WIFI_SSID);
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	ftp.addUser(FTP_USER, FTP_PASSWORD);
	//ftp.addFilesystem("SPIFFS", &SPIFFS);
	ftp.addFilesystem("LittleFS", &LittleFS);
	ftp.begin();

//////////////////////////////////////////////////
	File file = LittleFS.open("/", "r");
	File sdFile;

  	while (File f = file.openNextFile()) {
    	sdFile = SD.open(f.name(), FILE_WRITE);

    	if (sdFile) {
      	while (f.available()) {
        	sdFile.write(f.read());
      	}
      	sdFile.close();
      	f.close();
      	LittleFS.remove(f.name()); // Удаление файла из LittleFS
      	Serial.print("File ");
      	Serial.print(f.name());
      	Serial.println(" written to SD card and removed from LittleFS");
		homeOLED();
		oled.setCursor(0, 2);
		oled.print("remove SD:");
		oled.setCursor(0, 3);
		oled.print(f.name());

    	}
  	}
  	file.close();

  // Задержка между циклами
  delay(5000);


////////////////////////////////////////////////////
 



}

void loop()
{
//приписать здесь запись на SD

	ftp.handle();


}