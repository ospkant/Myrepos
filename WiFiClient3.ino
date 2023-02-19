#include <ESP8266WiFi.h>
#include <FS.h>
#include <ESP8266FtpServer.h>
#include <SD.h>

const char* ssid = ".....";
const char* password = ".....";

FtpServer ftpSrv;
File uploadFile;
bool fileOpen = false;

void onFileReceived(FtpServer::FileMeta_t file_meta) {
  Serial.println("File received");
  if (fileOpen) {
    uploadFile.close();
  }
  uploadFile = SD.open(file_meta.name, FILE_WRITE);
  fileOpen = true;
}

void setup() {
  Serial.begin(9600);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)  
    delay(500);                           

  Serial.print("\n\nLocal IP: ");  
  Serial.println(WiFi.localIP());  

  if (!SD.begin(4)) {
    Serial.println("SD card initialization failed");
    return;
  }
  Serial.println("SD card initialized");

  ftpSrv.begin("login","password");       // Инициализируем FTP-сервер (на 21-й порт)
  ftpSrv.onFileReceived(onFileReceived);
}

void loop() {
  ftpSrv.handleFTP();
}

