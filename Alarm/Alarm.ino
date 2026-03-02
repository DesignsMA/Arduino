#define enA 11 // Pin de control de velocidad (PWM)
#define in1 13 // Pin de control de dirección 1
#define in2 12 // Pin de control de dirección 2
// --------- Salidas para operadores AND ------
#define logic1 2 
#define logic2 3
#define logic3 4
// --------- Entradas para operadores AND ------
#define input 5


//---------------BOCINA-------------------
#include <avr/pgmspace.h>
#include "rustalarm.h" // Archivo generado por wav2c, contiene la muestra de audio en formato C

extern const int sounddata_length; // Longitud de la muestra de audio
extern const signed char sounddata_data[] PROGMEM; // Muestra de audio almacenada en PROGMEM (memoria flash)
const int sampleRate = 8000; // Frecuencia de muestreo en Hz
#define speakerPin 9 // Pin de salida para el altavoz (PWM)

volatile int sampleIndex = 0;

unsigned long ultimoSample = 0;
unsigned long samplePeriod = 1000000UL / sampleRate; // microsegundos por muestra
//----------------------------------------

int estado = 0;   // 0 = espera, 1 = rampa
volatile bool flag = false;
int pwmValor = 0; // Variable para almacenar el valor PWM
unsigned long tiempoInicio = 0; // Variable para almacenar el tiempo de inicio
unsigned long intervalo = 1000000; // 1 segundo en microsegundos

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(speakerPin, OUTPUT);

  // Configura las salidas lógicas para operadores AND
  pinMode(logic1, OUTPUT);
  pinMode(logic2, OUTPUT);
  pinMode(logic3, OUTPUT);

  // Configura la entrada para operadores AND
  pinMode(input, INPUT);

  // Rotar el motor en una dirección
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  // Salidas and
  digitalWrite(logic1, HIGH);
  digitalWrite(logic2, HIGH);
  digitalWrite(logic3, HIGH);

  // Configura Timer1 para reproducir audio a 8-bit Fast PWM
  TCCR1A = _BV(COM1A1) | _BV(WGM10);
  TCCR1B = _BV(WGM12) | _BV(CS10);  // Sin prescaler (máxima velocidad)
  OCR1A = 128; // Nivel medio (silencio)
  

}

void loop() {

  unsigned long ahora = micros();
  
  estado = digitalRead(input);

  switch (estado) {

    case LOW: // La lectura es cero, motor apagado
      analogWrite(enA, 0); // Motor apagado
      OCR1A = 128; // Silencio en el altavoz
      ultimoSample = ahora; // Reiniciar temporizador de audio
      sampleIndex = 0; // Reiniciar índice de muestra
      break;

    case HIGH:

      // Motor a velocidad fija 50
      analogWrite(enA, 50);

      // si ha pasado el tiempo para la siguiente muestra, reproducir, sino esperar
      if (ahora - ultimoSample >= samplePeriod) {

        ultimoSample += samplePeriod;

        // Leer muestra desde PROGMEM
        int8_t sample = pgm_read_byte(&sounddata_data[sampleIndex]);

        OCR1A = sample;  // enviar al PWM (pin 9)

        sampleIndex++;

        // Si terminó la muestra
        if (sampleIndex >= sounddata_length) {
          sampleIndex = 0; // Reiniciar índice para repetir
        }
      }

      break;
  }
}