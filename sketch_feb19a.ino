#include <ESP8266WiFi.h>
#include <SD.h>
#include <ESP8266FtpServer.h>

const char* ssid = "SERGOV-PC_Network";
const char* password = "80sj-7v9b-sj6n";

WiFiServer server(21);
FtpServer ftpSrv;
File dataFile;
char buffer[128];
uint8_t bufpos = 0;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");

  if(!SD.begin(D8)) {
    Serial.println("Error initializing SD card");
  }

  server.begin();
  ftpSrv.begin("login", "password");
  ftpSrv.setFileCallback(getFile);
}

void loop() {
  ftpSrv.handleFTP();
  
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client");
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (bufpos < 128) {
          buffer[bufpos] = c;
          bufpos++;
        }
        if (c == '\n') {
          dataFile = SD.open("datalog.txt", FILE_WRITE);
          if (dataFile) {
            dataFile.write(buffer, bufpos);
            dataFile.close();
          }
          memset(buffer, 0, bufpos);
          bufpos = 0;
        }
      }
    }
    Serial.println("Client disconnected");
  }
}

void getFile(FtpServer::FileReadCallbackParameter arg) {
  String fileName = arg.fileName;

  File file = SD.open(fileName, FILE_READ);
  if (!file) {
    arg.client.println(F("550 File not found"));
    return;
  }

  arg.client.println(F("150 Opening data channel"));
  while (file.available()) {
    arg.client.write(file.read());
  }
  file.close();
  arg.client.println(F("226 Transfer complete"));
}

