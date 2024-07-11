/* 
	Fundación Kinal
    Centro educativo Técncico Laboral Kinal
    Quinto Perito
    Quinto Electrónica 
    Codigo Técnico: EB5AM
    Curso: Taller de Electrónica Digital y reparación de computadoras
    Proyecto: Comunicación serial y visualización
    Dev: Alexander González 2020414
    Fecha: 11 de julio de 2024
*/

// Pines para el sensor ultrasónico
const int trigPin = 9;
const int echoPin = 10;

// Pines para el visualizador de 7 segmentos
const int segmentPins[7] = {2, 3, 4, 5, 6, 7, 8};

// Pin para el LED NeoPixel
#include <Adafruit_NeoPixel.h>
const int neoPixelPin = 11;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, neoPixelPin, NEO_GRB + NEO_KHZ800);

int minimopersonas = 0;
const int maximopersonas = 9;

// Funcion que utilizo para obtener la distancia leida por el sensor ultrasonico
long readDistance(void);

void setup() {
  // Inicializar los pines del sensor ultrasónico
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Inicializar los pines del visualizador de 7 segmentos
  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }

  // Inicializar el LED NeoPixel
  strip.begin();
  strip.show(); // Inicializar todos los LEDs apagados

  // Inicializar la comunicación serial
  Serial.begin(9600);
}

void loop() {
  // Leer la distancia
  long distance = readDistance();

  // Comprobar la distancia para aumentar el conteo de personas
  if (distance >= 6 && distance <= 10) {
    if (minimopersonas < maximopersonas) {
      minimopersonas++;
    }
  }

  // Actualizar el visualizador de 7 segmentos y el LED NeoPixel
  updateDisplay(minimopersonas);
  updateNeoPixel(minimopersonas);

  // Esperar un tiempo antes de la siguiente medición
  delay(1000);
}

long readDistance(void) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distance = duration / 59; // Calcular la distancia en cm

  return distance;
}

void updateDisplay(int number) {
  // Definir los patrones para los números del 0 al 9
  const bool digitPatterns[10][7] = {
    {1, 1, 1, 1, 1, 1, 0},  // 0
    {0, 1, 1, 0, 0, 0, 0},  // 1
    {1, 1, 0, 1, 1, 0, 1},  // 2
    {1, 1, 1, 1, 0, 0, 1},  // 3
    {0, 1, 1, 0, 0, 1, 1},  // 4
    {1, 0, 1, 1, 0, 1, 1},  // 5
    {1, 0, 1, 1, 1, 1, 1},  // 6
    {1, 1, 1, 0, 0, 0, 0},  // 7
    {1, 1, 1, 1, 1, 1, 1},  // 8
    {1, 1, 1, 1, 0, 1, 1}   // 9
  };

  // Encender o apagar los segmentos según el número
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], digitPatterns[number][i]);
  }
}

void updateNeoPixel(int count) {
  if (count < maximopersonas) {
    strip.setPixelColor(0, strip.Color(0, 255, 0)); // Verde
  } else {
    strip.setPixelColor(0, strip.Color(255, 0, 0)); // Rojo
  }
  strip.show();
}
