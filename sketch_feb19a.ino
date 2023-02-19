#include <ESP8266WiFi.h>
#include <SD.h>
#include <ESP8266FtpServer.h>

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

FtpServer ftpSrv;
File writeFile;

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.println("WiFi connected");

  if (!SD.begin(SS)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized");

  ftpSrv.begin("login", "password");
}

void loop() {
  ftpSrv.handleFTP();
}

void onFileReceived(FtpServer::DataChannel channel, const char* filename, size_t size) {
  Serial.println("File received");

  // Open the file for writing
  writeFile = SD.open(filename, FILE_WRITE);

  if (!writeFile) {
    Serial.println("Failed to open file for writing");
    return;
  }

  // Create a buffer to store the data
  const int bufferSize = 1024;
  uint8_t buffer[bufferSize];

  // Read the data from the FTP channel and write it to the SD card
  size_t bytesRead = 0;
  while ((bytesRead = channel.read(buffer, bufferSize)) > 0) {
    writeFile.write(buffer, bytesRead);
  }

  // Close the file
  writeFile.close();

  Serial.println("File saved to SD card");
}

void onFileUpload(FtpServer::DataChannel channel, const char* filename, bool append) {
  Serial.print("File upload: ");
  Serial.println(filename);

  if (!append) {
    // Create a new file if one doesn't exist with the same name
    writeFile = SD.open(filename, FILE_WRITE);
  } else {
    // Append to an existing file
    writeFile = SD.open(filename, FILE_APPEND);
  }

  if (!writeFile) {
    Serial.println("Failed to open file for writing");
    return;
  }

  // Create a buffer to store the data
  const int bufferSize = 1024;
  uint8_t buffer[bufferSize];

  // Read the data from the FTP channel and write it to the SD card
  size_t bytesRead = 0;
  while ((bytesRead = channel.read(buffer, bufferSize)) > 0) {
    writeFile.write(buffer, bytesRead);
  }

  // Close the file
  writeFile.close();

  Serial.println("File saved to SD card");
}

