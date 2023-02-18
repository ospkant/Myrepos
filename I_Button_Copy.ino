#include <OneWire.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 4); // RX, TX

const int buttonPin = 2; // pin для кнопки для чтения
const int buttonPinWrite = 5; // pin для кнопки для записи
bool buttonPressedRead = false; // Флаг нажатия кнопки для чтения
bool buttonPressedWrite = false; // Флаг нажатия кнопки для записи

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  readRFIDData();

  if (digitalRead(buttonPin) == LOW) {
    buttonPressedRead = true;
  }

  if (buttonPressedRead) {
    if (mySerial.available() >= 8) { // Проверяем, достаточно ли байт доступно для чтения
      byte rfidData[8];
      mySerial.readBytes(rfidData, sizeof(rfidData)); // Считываем данные в массив байт
      Serial.print("Read RFID data: ");
      for (int i = 0; i < sizeof(rfidData); i++) {
        Serial.print(rfidData[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
    }
    buttonPressedRead = false;
  }

  if (digitalRead(buttonPin) == LOW) {
    buttonPressedWrite = true;
  }

  if (buttonPressedWrite) {
    byte rfidData[8] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED, 0xFA, 0xCE}; // Записываемые данные
    mySerial.write(rfidData, sizeof(rfidData)); // Записываем данные в порт
    Serial.print("Wrote RFID data: ");
    for (int i = 0; i < sizeof(rfidData); i++) {
      Serial.print(rfidData[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
    buttonPressedWrite = false;
  }
}

void readRFIDData() {
  if (mySerial.available() >= 8) { // Проверяем, достаточно ли байт доступно для чтения
    byte rfidData[8];
    mySerial.readBytes(rfidData, sizeof(rfidData)); // Считываем данные в массив байт
    Serial.print("Read RFID data: ");
    for (int i = 0; i < sizeof(rfidData); i++) {
      Serial.print(rfidData[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
}
