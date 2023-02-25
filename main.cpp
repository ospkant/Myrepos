//#include <WiFi.h>
#include <ESP8266WiFi.h>
#include <ESP8266FtpServer.h>
#include <GyverOLED.h>
#include <SD.h>


const char* ssid = "SERGOV-PC_Network";
const char* password = "80sj-7v9b-sj6n";
const uint8_t csPin = SS; // pin SD shield

FtpServer ftpSrv;   //set #define FTP_DEBUG in ESP8266FtpServer.h to see ftp verbose on serial
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled; // 1306 подключать к пинам D1 (GPIO5,SCL) и D2 (GPIO4,SDA)
 
void homeOLED() { // очистка, масштаб текста
  oled.clear();       // очистка
  oled.setScale(1);   // масштаб текста (1..4)
  oled.home();        // курсор в 0,0
}


void setup() {
  Serial.begin(9600);
  SPIFFS.begin();
  oled.init();
  homeOLED();
  oled.print("Wifi GRBL");
  WiFi.begin(ssid, password);
  Serial.println("");

  // SD initialization------------------------
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
  delay(2000);
  homeOLED();
 

 /* //-------Просмотр всех файлов на SD карте---

  File dirSD = SD.open("/"); //  Открытие корневой директории
  // Проверка, удалось ли открыть директорию
  if (dirSD) {
    // Перебор всех файлов в директории
    while (true) {
      File entry = dirSD.openNextFile();
      
      // Если файл не найден, выходим из цикла
      if (!entry) {
        break;
      }
      // Если это файл, выводим его имя
      if (!entry.isDirectory()) {
        Serial.println(entry.name());
      }
      // Закрываем файл
      entry.close();
    }
    // Закрываем директорию
    dirSD.close();
  }
  //------------------------------------------*/


 Serial.println("Список файлов в SPIFFS:");
  Dir dirFM = SPIFFS.openDir("/");
  int fileCount = 0;
  String thirdFileName;
  while (dirFM.next()) {

	fileCount++;
    if (fileCount == 3) {
      Serial.println(dirFM.fileName());

	  thirdFileName = (dirFM.fileName());

      break;
	}

    Serial.print(dirFM.fileName());
	oled.println(dirFM.fileName());
    File flash_file = dirFM.openFile("r");
    Serial.print("  Размер: ");
    Serial.print(flash_file.size());
    Serial.println(" байт");
    flash_file.close();
  }
homeOLED();
  Serial.print("Имя третьего файла: ");
    oled.println(thirdFileName);
	delay(5000);

  Serial.println();
  delay(5000); 




  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
	homeOLED();
	oled.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  homeOLED();
  oled.print("Соединение: ОК");
  oled.setCursor(0, 3);
  oled.print(ssid);
  oled.setCursor(0, 6);

  oled.print(WiFi.localIP());

  if (SPIFFS.begin()) {
	  //bool fileExists = SD.exists("/");
      Serial.println("SPIFFS opened!");
      ftpSrv.begin("esp8266","esp8266");    //username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)
  }   
}



void loop() {

  ftpSrv.handleFTP();        //make sure in loop you call handleFTP()!!  
 // server.handleClient();   //example if running a webserver you still need to call .handleClient();
 
}