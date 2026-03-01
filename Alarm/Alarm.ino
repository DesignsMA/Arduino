#define enA 12 // Pin de control de velocidad (PWM)
#define in1 11 // Pin de control de dirección 1
#define in2 10 // Pin de control de dirección 2

//---------------BOCINA-------------------
#include <avr/pgmspace.h>
#include "rustalarm.h" // Archivo generado por wav2c, contiene la muestra de audio en formato C

extern const int sounddata_length; // Longitud de la muestra de audio
extern const signed char sounddata_data[] PROGMEM; // Muestra de audio almacenada en PROGMEM (memoria flash)
const int sampleRate = 8000; // Frecuencia de muestreo en Hz
#define speakerPin 9 // Pin de salida para el altavoz (PWM)

volatile int sampleIndex = 0;
int repeticiones = 3;      // cuántas veces quieres reproducir
int conteoReps = 0;

unsigned long ultimoSample = 0;
unsigned long samplePeriod = 1000000UL / sampleRate; // microsegundos por muestra (duracion de sonido)
//----------------------------------------

int estado = 0;   // 0 = espera, 1 = rampa
int pwmValor = 0; // Variable para almacenar el valor PWM
unsigned long tiempoInicio = 0; // Variable para almacenar el tiempo de inicio
unsigned long intervalo = 1000000; // 1 segundo en microsegundos

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  // Rotar el motor en una dirección
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  // Configura Timer1 para reproducir audio a 8-bit Fast PWM
  TCCR1A = _BV(COM1A1) | _BV(WGM10);
  TCCR1B = _BV(WGM12) | _BV(CS10);  // Sin prescaler (máxima velocidad)
  OCR1A = 128; // Nivel medio (silencio)
}

void loop() {

  unsigned long ahora = micros();

  switch (estado) {

    case 0:
      // Espera 1 segundo antes de iniciar
      if (ahora - tiempoInicio >= intervalo) {
        tiempoInicio = ahora;
        estado = 1;
        sampleIndex = 0;
        conteoReps = 0;
      }
      break;

    case 1:

      // Motor a velocidad fija 50
      analogWrite(enA, 50);

      // Control de muestreo a 8000 Hz
      if (ahora - ultimoSample >= samplePeriod) {

        ultimoSample += samplePeriod;

        // Leer muestra desde PROGMEM
        int8_t sample = pgm_read_byte(&sounddata_data[sampleIndex]);

        OCR1A = sample;  // enviar al PWM (pin 9)

        sampleIndex++;

        // Si terminó la muestra
        if (sampleIndex >= sounddata_length) {
          sampleIndex = 0;
          conteoReps++;

          if (conteoReps >= repeticiones) {
            // Apagar todo
            OCR1A = 128;       // silencio
            analogWrite(enA, 0);
            estado = 0;
            tiempoInicio = ahora;
          }
        }
      }

      break;
  }
}