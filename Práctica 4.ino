/* 
  Fundación Kinal
    Centro educativo Técncico Laboral Kinal
    Quinto Perito
    Quinto Electrónica 
    Codigo Técnico: EB5AM
    Curso: Taller de Electrónica Digital y reparación de computadoras
    Proyecto: RFID
    Dev: Alexander González 2020414
    Fecha: 29 de julio de 2024
*/

#include <MFRC522.h>
#include <SPI.h>

#define RST_PIN 9    // Reset del MFRC522 en el pin 9
#define SS_PIN 10    // Selección de chip en el pin 10
#define ledVerde 4   // LED verde en el pin 4
#define ledRojo 5    // LED rojo en el pin 5
#define buzzer 6     // Buzzer en el pin 6

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

// UID de las tarjetas o llaveros autorizados y no autorizados
byte tagAutorizado[4] = {0xE3, 0xDD, 0xEE, 0x24}; // UID autorizado
byte tagNoAutorizado[4] = {0x70, 0x78, 0x70, 0xA4}; // UID no autorizado

byte tagActual[4];

void setup() {
  Serial.begin(9600);
  while (!Serial); // Espera a que se abra el puerto serie

  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(ledVerde, OUTPUT);
  pinMode(ledRojo, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent())
    return;

  if (!mfrc522.PICC_ReadCardSerial())
    return;

  Serial.print("Card UID: ");
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);

  // Apagar LEDs y buzzer para asegurarse de que solo uno está encendido en cualquier momento
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledRojo, LOW);
  noTone(buzzer);

  // Comparar el UID leído con el autorizado y no autorizado
  if (compararArray(tagActual, tagAutorizado)) {
    Serial.println("Acceso Permitido...");
    digitalWrite(ledVerde, HIGH);
    tone(buzzer, 200); // Emite tono de 200Hz
    delay(4000); // Mantener el LED verde encendido durante 4 segundos
    noTone(buzzer); // Detener el tono
    digitalWrite(ledVerde, LOW);
  } else if (compararArray(tagActual, tagNoAutorizado)) {
    Serial.println("Acceso Denegado");
    digitalWrite(ledRojo, HIGH);
    tone(buzzer, 400); // Emite tono de 400Hz
    delay(1000); // Mantener el LED rojo encendido durante 1 segundo
    noTone(buzzer); // Detener el tono
    digitalWrite(ledRojo, LOW);
  } else {
    Serial.println("Tag no reconocido");
  }
}

void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? "0" : "");
    Serial.print(buffer[i], HEX);
    tagActual[i] = buffer[i]; // Guardar el UID leído en tagActual
  }
  Serial.println();
}

boolean compararArray(byte Array1[], byte Array2[]) {
  for (byte i = 0; i < 4; i++) { // Comparar los primeros 4 bytes
    if (Array1[i] != Array2[i]) {
      return false;
    }
  }
  return true;
}
