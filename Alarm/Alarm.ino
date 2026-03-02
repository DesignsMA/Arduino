#define in1 12 // Pin de control de dirección 1
#define in2 11 // Pin de control de dirección 2
#define enA 3 // Pin de control de velocidad (PWM)

// --------- Salidas para operadores AND ------
#define logic1 A0 
#define logic2 A1
#define logic3 A2
// --------- Entradas para operadores AND ------
#define input 2


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
volatile bool motorActivo = false;
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

  // Inicializa el motor apagado
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  TCCR2A = 0;
  TCCR2B = 0;
  digitalWrite(enA, LOW);
  motorActivo = false;

  // Salidas and
  digitalWrite(logic1, HIGH);
  digitalWrite(logic2, HIGH);
  digitalWrite(logic3, HIGH);

  // Configura Timer1 para reproducir audio a 8-bit Fast PWM
  TCCR1A = _BV(COM1A1) | _BV(WGM10);
  TCCR1B = _BV(WGM12) | _BV(CS10);
  OCR1A = 128;
}

void loop() {

  unsigned long ahora = micros();
  
  estado = digitalRead(input);


  switch (estado) {

    case LOW: // La lectura es cero, motor apagado
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      
      TCCR2A = 0;  // ← Detener Timer2
      TCCR2B = 0;
      digitalWrite(enA, LOW);

      OCR1A = 128; // Silencio en el altavoz
      ultimoSample = ahora; // Reiniciar temporizador de audio
      sampleIndex = 0; // Reiniciar índice de muestra
      motorActivo = false; // Marcar motor como inactivo
      break;

case HIGH:
      // configurando pin3
      if (!motorActivo) {
        digitalWrite(in2, HIGH); 
        digitalWrite(in1, LOW);
        
        // Fast PWM 8-bit, COM2B1 para pin 3, prescaler 8
        TCCR2A = _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);  // COM2B1
        TCCR2B = _BV(CS21);  // Prescaler 8
        OCR2B = 50;  // ← PWM inicial (usar OCR2B directo, NO analogWrite)
        
        motorActivo = true;
      }
      
      int pwmMotor = 50 + ((long)sampleIndex * (255 - 50)) / sounddata_length;
      OCR2B = pwmMotor;  // OCR2B directo para pin 3

      if (ahora - ultimoSample >= samplePeriod) {
        ultimoSample = ahora;

        int8_t sample = pgm_read_byte(&sounddata_data[sampleIndex]);
        OCR1A = sample;

        sampleIndex++;
        if (sampleIndex >= sounddata_length) {
          sampleIndex = 0;
        }
      }
      break;
  }
}