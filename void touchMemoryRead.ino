void touchMemoryRead() {
  // ��������� ������ � ����������� iButton
  byte ibuttonAddress[IBUTTON_ADDRESS_SIZE];
  if (oneWire.search(ibuttonAddress)) { // ���� ������� ����� ����������
    if (OneWire::crc8(ibuttonAddress, 7) == ibuttonAddress[7]) { // ���� ����� ���������� ������ �������� CRC
      Serial.print("iButton: ");
      for (int i = 0; i < IBUTTON_ADDRESS_SIZE; i++) {
        Serial.print(ibuttonAddress[i], HEX); // ������� ����� ����������
      }
      Serial.println();
    }
  }
}