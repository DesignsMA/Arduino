

const int speakerPin = 9;
const int sampleRate = 8000; // Frecuencia de muestreo en Hz

void setup() {
  pinMode(speakerPin, OUTPUT);

  // Configura Timer1 para reproducir audio a 8-bit Fast PWM
  TCCR1A = _BV(COM1A1) | _BV(WGM10);
  TCCR1B = _BV(WGM12) | _BV(CS10);  // Sin prescaler (máxima velocidad)
  OCR1A = 128; // Nivel medio (silencio)
}

void loop() {
  unsigned long startMicros = micros();
  unsigned long samplePeriod = 1000000UL / sampleRate; // microsegundos por muestra

  for (int i = 0; i < horn1_length; i++) {
    unsigned char sample = pgm_read_byte(&horn1_data[i]); // Leer desde PROGMEM
    OCR1A = sample; // Actualizar PWM

    // Esperar hasta que haya pasado el tiempo exacto
    while (micros() - startMicros < samplePeriod) {
      // no hacer nada
    }
    startMicros += samplePeriod; // siguiente marca de tiempo
  }

  delay(3000); // Esperar antes de repetir el sonido
}
