#define enA 12 // Pin de control de velocidad (PWM)
#define in1 11 // Pin de control de dirección 1
#define in2 10 // Pin de control de dirección 2

//---------------BOCINA-------------------
#include <avr/pgmspace.h>

extern const int horn1_length; // Longitud de la muestra de audio
extern const signed char horn1_data[] PROGMEM; // Muestra de audio almacenada en PROGMEM (memoria flash)
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
}

void loop() {
  unsigned long ahora = micros();
  switch (estado) {
// Estado 0: esperar 1 segundo
    case 0:
      if (ahora - tiempoInicio >= intervalo) {
        tiempoInicio = ahora;
        estado = 1;
        pwmValor = 0;
      }
      break;

    // Estado 1: subir PWM de 0 a 255 en 1 segundo
    case 1:
      if (ahora - tiempoInicio <= intervalo) {

        // Calculamos progreso (0.0 a 1.0)
        float progreso = (float)(ahora - tiempoInicio) / intervalo;
        pwmValor = progreso * 255;

        analogWrite(enA, pwmValor);

      } else {
        analogWrite(enA, 0); // apagar
        estado = 0;               // volver a empezar
        tiempoInicio = ahora;
      }
      break;
    }
}