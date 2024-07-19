/* 
  Fundación Kinal
    Centro educativo Técncico Laboral Kinal
    Quinto Perito
    Quinto Electrónica 
    Codigo Técnico: EB5AM
    Curso: Taller de Electrónica Digital y reparación de computadoras
    Proyecto: Alarma
    Dev: Alexander González 2020414
    Fecha: 16 de julio de 2024
*/

// Librerías
#include <Wire.h>                   // Librería para usar el protocolo I2C
#include <SparkFun_ADXL345.h>       // Librería para usar el acelerómetro ADXL345
#include <SoftwareSerial.h>         // Librería para emular otro puerto serie
#include <DFRobotDFPlayerMini.h>    // Librería para usar el reproductor MP3

// Constructores
ADXL345 acc_sismo = ADXL345();          // Constructor para el acelerómetro
SoftwareSerial mp3_sismo(2, 3);         // RX en 2 y TX en 3
DFRobotDFPlayerMini reproductor_MP3;    // Constructor para el reproductor MP3

// Variables
int acc_X;    // Aceleración en el eje X
int acc_Y;    // Aceleración en el eje Y
int acc_Z;    // Aceleración en el eje Z
float magnitud_acc;    // Magnitud del vector aceleración

// Pines de la barra de LEDs
const int ledPins[] = {4, 5, 6, 7, 8, 9, 10, 11, 12}; // Pines a los que están conectados los LEDs
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]); // Número total de LEDs en la barra

// Configuraciones
void setup() {
  Serial.begin(9600);             // Inicio la comunicación serial del Arduino hacia la computadora
  mp3_sismo.begin(9600);          // Inicio la comunicación serial emulada para el MP3
  acc_sismo.powerOn();            // Enciendo el acelerómetro
  acc_sismo.setRangeSetting(8);   // Configuro la sensibilidad del acelerómetro

  // Configuro los pines de la barra de LEDs como salidas
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW); // Apaga todos los LEDs al inicio
  }

  // Compruebo si hay conexión con el módulo MP3
  if (!reproductor_MP3.begin(mp3_sismo)) { 
    Serial.println("No he podido comunicarme con el módulo MP3");
    while (1);
  }

  // Si la comunicación ha sido exitosa, avanzo
  reproductor_MP3.volume(10);      // Volumen al máximo
  Serial.println("Comunicación exitosa con el reproductor MP3");
  Serial.println("Práctica 2 - Ayuda profe Alejandro");
  Serial.println("Reproduzco la primera canción");
  reproductor_MP3.play(1);        // Reproduzco la primera canción (001.mp3)
  delay(100);
}

void loop() {
  // Leo las variables medidas por el acelerómetro
  acc_sismo.readAccel(&acc_X, &acc_Y, &acc_Z);

  // Obtengo la magnitud del vector aceleración
  magnitud_acc = sqrtf((acc_X * acc_X) + (acc_Y * acc_Y) + (acc_Z * acc_Z));

  // Mapea la magnitud de la aceleración a un rango de 0 a 9 (para 9 LEDs)
  int mapeo_intensidad = map(magnitud_acc, 47, 75, 0, numLeds);

  // Actualiza la barra de LEDs según la intensidad del sismo
  for (int i = 0; i < numLeds; i++) {
    if (i < mapeo_intensidad) {
      digitalWrite(ledPins[i], HIGH);  // Enciende el LED
    } else {
      digitalWrite(ledPins[i], LOW);   // Apaga el LED
    }
  }

  // Activa la alarma sonora si el nivel es mayor o igual a 6
  if (mapeo_intensidad >= 6) {
    digitalWrite(13, HIGH);  // Enciende la bocina
    delay(500);              // Espera por 500 ms
    digitalWrite(13, LOW);   // Apaga la bocina
    delay(500);              // Espera por 500 ms
  }

  delay(1000);  // Espera 1 segundo antes de leer nuevamente
}

