void touchMemoryRead() {
  // Считываем данные с считывателя iButton
  byte ibuttonAddress[IBUTTON_ADDRESS_SIZE];
  if (oneWire.search(ibuttonAddress)) { // Если удалось найти устройство
    if (OneWire::crc8(ibuttonAddress, 7) == ibuttonAddress[7]) { // Если адрес устройства прошел проверку CRC
      Serial.print("iButton: ");
      for (int i = 0; i < IBUTTON_ADDRESS_SIZE; i++) {
        Serial.print(ibuttonAddress[i], HEX); // Выводим адрес устройства
      }
      Serial.println();
    }
  }
}