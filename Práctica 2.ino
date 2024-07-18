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

// Librerías
#include <Wire.h>                   // Librería para usar el protocolo i2c
#include <SparkFun_ADXL345.h>       // Librería para usar el acelerómetro ADXL345
#include <SoftwareSerial.h>         // Librería para emular otro puerto serie
#include <DFRobotDFPlayerMini.h>    // Librería para usar el reproductor MP3

// Constructores
ADXL345 acc_sismo = ADXL345();          // Constructor para usar el acelerómetro
SoftwareSerial mp3_sismo(A0, A1);       // RX A0 y TX A1
DFRobotDFPlayerMini reproductor_MP3;    // Constructor para el reproductor MP3

// Definición de pines para LEDs
#define LED_BAR_START 2
#define LED_BAR_END 11

// Variables
unsigned int acc_X;         // Aceleración en el eje x
unsigned int acc_Y;         // Aceleración en el eje y
unsigned int acc_Z;         // Aceleración en el eje z

// Variable donde guardo la magnitud del vector aceleración
float magnitud_acc;

// Configuraciones
void setup() {
  Serial.begin(9600);              // Inicio la comunicación serial del Arduino hacia la computadora
  mp3_sismo.begin(9600);           // Inicio la comunicación serial emulada para el mp3
  acc_sismo.powerOn();             // Enciendo el acelerómetro
  acc_sismo.setRangeSetting(8);    // Configuro la sensibilidad del acelerómetro

  // Compruebo si hay conexión con el módulo mp3
  if (!reproductor_MP3.begin(mp3_sismo)) { 
    // Si no existe, me quedo aquí siempre. Ya no avanzo en el código
    Serial.println("No he podido comunicarme con el modulo MP3");
    while (1);
  }
  
  // Si la comunicación ha sido exitosa, avanzo
  reproductor_MP3.volume(10);       // Volumen al máximo
  Serial.println("Comunicacion exitosa con el reproductor MP3");
  Serial.println("Practica 2");
  Serial.println("Reproduzco la primera cancion");
  reproductor_MP3.play(1);          // Reproduzco la primera canción
  delay(100);

  // Configuración de pines de salida para la barra de LEDs
  for (int i = LED_BAR_START; i <= LED_BAR_END; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);           
  }
}

void loop() {
  // Leo las variables medidas por el acelerómetro
  acc_sismo.readAccel(&acc_X, &acc_Y, &acc_Z);

  // Obtengo la magnitud del vector aceleración
  magnitud_acc = sqrtf((acc_X * acc_X) + (acc_Y * acc_Y) + (acc_Z * acc_Z));
  
  int mapeo_intensidad = map(magnitud_acc, 47, 75, 0, 10);

  // Mostrar la intensidad en la barra de LEDs
  showIntensity(mapeo_intensidad);

  // Activar alarma sonora si la intensidad es mayor a 6 (nivel VI en escala de Mercalli)
  if (mapeo_intensidad > 6) {
    reproductor_MP3.play(1); // Número de archivo de sonido en tu tarjeta SD
  }

  delay(1000); // Intervalo de lectura y procesamiento
}

// Función para mostrar la intensidad en la barra de LEDs
void showIntensity(int intensity) {
  // Apaga todos los LEDs primero
  for (int i = LED_BAR_START; i <= LED_BAR_END; i++) {
    digitalWrite(i, LOW);
  }
  
  // Enciende los LEDs según la intensidad
  for (int i = LED_BAR_START; i < LED_BAR_START + intensity; i++) {
    digitalWrite(i, HIGH);
  }
}
