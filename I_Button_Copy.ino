#include <SoftwareSerial.h>

byte buffer[8];
SoftwareSerial mySerial(3, 2); // RX, TX
const int buttonPinRead = 4; // Пин, к которому подключена кнопка для считывания
bool buttonPressedRead = false; // Флаг, указывающий, была ли нажата кнопка
const int buttonPinWrite = 5; // Пин, к которому подключена кнопка для записи
bool buttonPressedWrite = false; // Флаг, указывающий, была ли нажата кнопка

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(buttonPinRead, INPUT_PULLUP); // Устанавливаем пин кнопки в режим INPUT_PULLUP
 
}



void loop() {
  if (digitalRead(buttonPinRead) == LOW) { // Если кнопка нажата
    readRFID(); // Выполняем функцию readRFID()
    delay(500); // Задержка, чтобы избежать повторного считывания в течение 500 миллисекунд
  }
}

void readRFID() {
  byte rfidData[8];
  if (mySerial.available() >= sizeof(rfidData)) { // Проверяем, достаточно ли байт доступно для чтения
    mySerial.readBytes(rfidData, sizeof(rfidData)); // Считываем данные в массив байт
  }
}

void ibuttonRead() {
  if (buttonPressedRead) { // Если кнопка была нажата
    byte rfidData[8];
    if (mySerial.available() >= sizeof(rfidData)) { // Проверяем, достаточно ли байт доступно для чтения
      mySerial.readBytes(rfidData, sizeof(rfidData)); // Считываем данные в массив байт
    }
    buttonPressedRead = false; // Сбрасываем флаг, чтобы избежать повторного считывания при следующем вызове функции
  }
}

void ibuttonWrite(){
  if (buttonPressedWrite) { // Если кнопка была нажата
    byte dataToWrite[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08}; // Здесь задайте данные, которые нужно записать
    OneWire ibutton(ONE_WIRE_BUS);
    ibutton.reset();
    ibutton.select(ibuttonAddress);
    ibutton.write_bytes(dataToWrite, sizeof(dataToWrite));
    ibutton.reset();
    buttonPressedWrite = false; // Сбрасываем флаг, чтобы избежать повторной записи при следующем вызове функции
  }
}
