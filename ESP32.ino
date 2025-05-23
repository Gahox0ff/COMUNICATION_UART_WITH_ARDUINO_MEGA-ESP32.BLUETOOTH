#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_MOSI   23
#define OLED_CLK    18
#define OLED_DC     2
#define OLED_CS     5
#define OLED_RESET  4

#define RXD2 16
#define TXD2 17

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_DC, OLED_RESET, OLED_CS);
String comando = "";
String mega = "";

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  SerialBT.begin("ESP32_BT");

  // INICIALIZACIÓN MANUAL DEL BUS SPI
  SPI.begin(OLED_CLK, -1, OLED_MOSI, OLED_CS);

  // INICIALIZACIÓN DE LA PANTALLA
  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("No se pudo inicializar la pantalla OLED"));
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE); // IMPORTANTE: asegúrate de tener color
  display.setCursor(0, 0);
  display.println("Esperando datos");
  display.display();
}

void loop() {

  if (Serial2.available()) {
    String linea1 = Serial2.readStringUntil('\n');
    String linea2 = Serial2.readStringUntil('\n');
    String linea3 = Serial2.readStringUntil('\n');
    String linea4 = Serial2.readStringUntil('\n');

    Serial.println("Recibido:");
    Serial.println(linea1);
    Serial.println(linea2);
    Serial.println(linea3);
    Serial.println(linea4);

    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.println(linea1);
    display.println(linea2);
    display.println(linea3);
    if (linea4.length() > 0) display.println(linea4);
    display.display();

    SerialBT.println(linea1);
    SerialBT.println(linea2);
    SerialBT.println(linea3);
    if (linea4.length() > 0) SerialBT.println(linea4);

    Serial2.println("OK");
  }

  if (SerialBT.available() > 0) {
    comando = SerialBT.readStringUntil('\n');
    comando.trim();
    Serial.println("Comando: " + comando);
    
    SerialBT.println("Comando: " + comando);

    if (comando == "F" || comando == "T" || comando == "L" || comando == "A" ||
        comando == "LED1" || comando == "LED1OFF" || comando == "LED2" || comando == "LED2OFF" ||
        comando == "LED3" || comando == "LED3OFF") 
    {
      Serial2.println(comando);
      Serial.println("Enviado a Mega: " + comando);
      delay(1500);
      if (Serial2.available()) {
        mega = Serial2.readStringUntil('\n');
        SerialBT.println(mega);
        Serial.println(mega);
      }
    }
  }
}
