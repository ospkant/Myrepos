#include <ESP8266WiFi.h>                  // Библиотека для создания Wi-Fi подключения (клиент или точка доступа)
#include <FS.h>                           // Библиотека для работы с файловой системой
#include <ESP8266FtpServer.h>             // Библиотека для работы с SPIFFS по FTP

const char* ssid = "SERGOV-PC_Network";            // Указываем имя WiFi-сети, к которой будет подключаться ESP8266 для доступа в Интернет
const char* password = "80sj-7v9b-sj6n";        // Указываем пароль для подключения к WiFi-сети

FtpServer ftpSrv;                         // Создаём объект для работы с FTP

void setup() {
  Serial.begin(9600);                     // Инициализируем вывод данных на серийный порт со скоростью 9600 бод
  
  WiFi.begin(ssid, password);             // Соединяемся с WiFi-сетью
  while (WiFi.status() != WL_CONNECTED)   // Пока соединение не установлено
    delay(500);                           //  делаем задержку в пол секунды, пока соединение не установится

  Serial.print("\n\nLocal IP: ");         // Выводим на монитор серийного порта сообщение о том, что сейчас будем выводить локальный IP
  Serial.println(WiFi.localIP());         // Выводим локальный IP-адрес ESP8266

  SPIFFS.begin();                         // Инициализируем работу с файловой системой


  if (SPIFFS.format())                    // Если удалось отформатировать файловую систему
    Serial.println("File System Formated"); // Выводим уведомление об этом
  else                                    // иначе
    Serial.println("File System Formating error");  // Выводим сообщение об ошибке


  ftpSrv.begin("login","password");       // Инициализируем FTP-сервер (на 21-й порт)
}

void loop() {
  ftpSrv.handleFTP();                     // Отслеживаем обращения по FTP 
}
