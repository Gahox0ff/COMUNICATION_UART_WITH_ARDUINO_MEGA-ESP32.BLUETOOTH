#include <DHT.h>
#define DHTPIN A15
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define LDR_PIN A13
#define POT_PIN A14

#define LED1 22
#define LED2 23
#define LED3 24

#define Bot_frecuencia 53
#define Bot_temperatura 52
#define Bot_ldr 51
#define Bot_4 50

volatile unsigned long cuenta = 0;
unsigned long t_ini;

int estadoAnteriorFreq = HIGH;
int estadoAnteriorTemp = HIGH;
int estadoAnteriorLDR = HIGH;
int estadoAnterior4 = HIGH;

void setup() {

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  pinMode(Bot_frecuencia, INPUT_PULLUP);
  pinMode(Bot_temperatura, INPUT_PULLUP);
  pinMode(Bot_ldr, INPUT_PULLUP);
  pinMode(Bot_4, INPUT_PULLUP);

  Serial.begin(9600);    
  Serial1.begin(9600);    

  attachInterrupt(digitalPinToInterrupt(2), medirFrecuencia, RISING);

  dht.begin();
}
void medirFrecuencia() {
  cuenta++;
}
void loop() {
  int estadoActualFreq = digitalRead(Bot_frecuencia);
  int estadoActualTemp = digitalRead(Bot_temperatura);
  int estadoActualLDR  = digitalRead(Bot_ldr);
  int estadoActualBot_4 = digitalRead(Bot_4);

  float voltaje = analogRead(POT_PIN) * (5.0 / 1023.0);
  float ldrValor = analogRead(LDR_PIN);
  float Vcc = 5.0;
float Rc = 10000; // resistencia fija de 10k ohmios
float Rmin = 470;   // resistencia de la LDR con mucha luz
float Rmax = 10000; // resistencia de la LDR con poca luz

  float Vin = (ldrValor / 1023.0) * Vcc;

  float Rldr = (Vin * Rc) / (Vcc - Vin);

  float luz = (Rmax - Rldr) / (Rmax - Rmin) * 100.0;

  luz = constrain(luz, 0.0, 100.0);

  float temperatura = dht.readTemperature();

  if (estadoAnteriorFreq == HIGH && estadoActualFreq == LOW) {
    cuenta = 0;
    t_ini = millis();
    while (millis() - t_ini < 1000) {
      delay(1);
    }

    Serial1.print("Frecuencia: ");
    Serial1.print(cuenta);
    Serial1.println(" Hz");
    Serial.print("Frecuencia: ");
    Serial.println(cuenta);
  }

  if (estadoAnteriorTemp == HIGH && estadoActualTemp == LOW) {
      Serial1.print("Temperatura: ");
      Serial1.print(temperatura);
      Serial1.println(" C");
      Serial.print("Temperatura: ");
      Serial.println(temperatura);
  }

  if (estadoAnteriorLDR == HIGH && estadoActualLDR == LOW) {
    Serial1.print("Luz: ");
    Serial1.print(luz);
    Serial1.println(" %");
  }

  if (estadoAnterior4 == HIGH && estadoActualBot_4 == LOW) {
    Serial1.print("Frecuencia: ");
    Serial1.print(cuenta);
    Serial1.println(" Hz");

    Serial1.print("Temperatura: ");
    Serial1.print(temperatura);
    Serial1.println(" C");

    Serial1.print("Luz: ");
    Serial1.print(luz);
    Serial1.println(" %");

    if (voltaje >= 2.0 && voltaje <= 4.0) {
      Serial1.print("Voltaje: ");
      Serial1.println(voltaje);
    }
  }

  estadoAnteriorFreq = estadoActualFreq;
  estadoAnteriorTemp = estadoActualTemp;
  estadoAnteriorLDR = estadoActualLDR;
  estadoAnterior4 = estadoActualBot_4;

  if (Serial1.available()) {
    String comando = Serial1.readStringUntil('\n');
    comando.trim();
    Serial.println(comando);
    if (comando == "OK") {
    return;
  }
    if (comando == "F") {
      Serial1.print("Frecuencia: ");
      Serial1.print(cuenta);
      Serial1.println(" Hz");
    }
     else if (comando == "T") {
        Serial1.print("Temperatura: ");
        Serial1.print(temperatura);
        Serial1.println(" C");
      }
     else if (comando == "L") {
      Serial1.print("Luz: ");
      Serial1.print(luz);
      Serial1.println(" %");
    } else if (comando == "A") 
      {
      Serial1.print("Frecuencia: ");
      Serial1.print(cuenta);
      Serial1.println(" Hz");

      Serial1.print("Temperatura: ");
      Serial1.print(temperatura);
      Serial1.println(" C");

      Serial1.print("Luz: ");
      Serial1.print(luz);
      Serial1.println(" %");

      if (voltaje >= 2.0 && voltaje <= 4.0) 
      {
      Serial1.print("Voltaje: ");
      Serial1.println(voltaje);
      }
    } 
    else if (comando == "LED1") digitalWrite(LED1, HIGH);
    else if (comando == "LED1OFF") digitalWrite(LED1, LOW);
    else if (comando == "LED2") digitalWrite(LED2, HIGH);
    else if (comando == "LED2OFF") digitalWrite(LED2, LOW);
    else if (comando == "LED3") digitalWrite(LED3, HIGH);
    else if (comando == "LED3OFF") digitalWrite(LED3, LOW);
  }
}
