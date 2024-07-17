/* 
	Fundación Kinal
    Centro educativo Técncico Laboral Kinal
    Quinto Perito
    Quinto Electrónica 
    Codigo Técnico: EB5AM
    Curso: Taller de Electrónica Digital y reparación de computadoras
    Proyecto: Comunicación serial y visualización
    Dev: Alexander González 2020414
    Fecha: 16 de julio de 2024
*/

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// Definición de pines
#define LED_BAR_START 2
#define LED_BAR_END 11

// Configuración del DFPlayer Mini
SoftwareSerial mySerial(7, 8); // RX, TX
DFRobotDFPlayerMini player;

// Objeto para el acelerómetro ADXL345
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

void setup() {
  // Inicialización de comunicación serial para el DFPlayer Mini
  mySerial.begin(9600);
  if (!player.begin(mySerial)) {
    Serial.println(F("No se pudo encontrar el módulo DFPlayer Mini"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini listo"));

  // Inicialización del acelerómetro ADXL345
  if(!accel.begin()) {
    Serial.println(F("No se pudo encontrar el acelerómetro ADXL345"));
    while(true);
  }
  Serial.println(F("ADXL345 listo"));

  // Configuración de pines de salida para la barra de LEDs
  for (int i = LED_BAR_START; i <= LED_BAR_END; i++) {
    pinMode(i, OUTPUT);
  }
}

void loop() {
  sensors_event_t event;
  accel.getEvent(&event);

  // Obtener la aceleración en los ejes X, Y, Z
  float ax = event.acceleration.x;
  float ay = event.acceleration.y;
  float az = event.acceleration.z;

  // Calcular la magnitud de la aceleración
  float magnitude = sqrt(ax * ax + ay * ay + az * az);

  // Determinar la intensidad del sismo según la magnitud
  int intensity = map((int)magnitude, 0, 200, 0, 10); 

  // Mostrar la intensidad en la barra de LEDs
  showIntensity(intensity);

  // Activar alarma sonora si la intensidad es mayor a 6 (nivel VI en escala de Mercalli)
  if (intensity > 6) {
    player.play(1); // Número de archivo de sonido en la tarjeta SD
  }

  delay(1000); // Intervalo de lectura y procesamiento
}

// Función para mostrar la intensidad en la barra de LEDs
void showIntensity(int intensity) {
  // Apaga todos los LEDs primero
  for (int i = LED_BAR_START; i <= LED_BAR_END; i++) {
    digitalWrite(i, LOW);
  }

  // Enciende LEDs según la intensidad
  for (int i = LED_BAR_START; i <= LED_BAR_START + intensity - 1; i++) {
    digitalWrite(i, HIGH);
  }
}
