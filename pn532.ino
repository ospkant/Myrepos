#include <Wire.h>
#include <SPI.h>
#include <PN532_SPI.h>
#include <PN532.h>

PN532_SPI pn532spi(SPI, 10);
PN532 nfc(pn532spi);

void setup(void) {
  Serial.begin(115200);
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("PN532 not found");
    while (1); // Приостановить выполнение программы
  }
  Serial.print("Found chip PN532");
  Serial.println((versiondata>>16) & 0xFF, HEX);
  Serial.println((versiondata>>8) & 0xFF, HEX);
  Serial.println((versiondata>>0) & 0xFF, HEX);
  nfc.SAMConfig();
}

void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 }; // Массив для хранения UID тега
  uint8_t uidLength; // Размер UID тега

  // Ждем пока тег будет обнаружен
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  if (success) {
    Serial.println("Tag detected!");

    // Пример чтения данных из блока 4, используя ключ A, смещение в битах 0, и чтение 4 битов
    uint8_t blockNumber = 4;
    uint8_t key[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    uint8_t offsetBits = 0;
    uint8_t lengthBits = 4;
    uint8_t data[16];
    success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, blockNumber, 0, key);
    if (success) {
      success = nfc.mifareclassic_ReadDataBlock(blockNumber, data);
      if (success) {
        Serial.print("Block ");
        Serial.print(blockNumber);
        Serial.print(" Data: ");
        for (uint8_t i = 0; i < 16; i++) {
          Serial.print(data[i], HEX);
          Serial.print(" ");
        }
        Serial.println();
      } else {
        Serial.println("Read failed");
      }
    } else {
      Serial.println("Authentication failed");
    }

    // Пример записи данных в блок 4, используя ключ A, смещение в битах 0, и запись 4 битов
    uint8_t blockNumber = 4;
    uint8_t key[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    uint8_t offsetBits = 0;
    uint8_t lengthBits = 4;
    uint8_t data[16];
    success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, blockNumber, 0, key);
if (success) {
  // Изменяем нужные биты в буфере данных
  data[0] &= 0xF0; // Сбросить младшие 4 бита
  data[0] |= 0x0A; // Установить младшие 4 бита на 0x0A

  // Записываем данные в блок 4
  success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, blockNumber, 0, key);
  if (success) {
    success = nfc.mifareclassic_WriteDataBlock(blockNumber, data);
    if (success) {
      Serial.println("Write success");
    } else {
      Serial.println("Write failed");
    }
  } else {
    Serial.println("Authentication failed");
  }
}

// Пример чтения данных из блока 5, используя ключ B, смещение в байтах 1, и чтение 4 байтов
blockNumber = 5;
key[0] = 0x11; key[1] = 0x22; key[2] = 0x33; key[3] = 0x44; key[4] = 0x55; key[5] = 0x66;
uint8_t offsetBytes = 1;
uint8_t lengthBytes = 4;
uint8_t dataBytes[16];
success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, blockNumber, 1, key);
if (success) {
  success = nfc.mifareclassic_ReadDataBlock(blockNumber, dataBytes);
  if (success) {
    Serial.print("Block ");
    Serial.print(blockNumber);
    Serial.print(" Data: ");
    for (uint8_t i = 0; i < 16; i++) {
      Serial.print(dataBytes[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  } else {
    Serial.println("Read failed");
  }
} else {
  Serial.println("Authentication failed");
  }
 }
}
