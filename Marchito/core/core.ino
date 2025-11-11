#include <SoftwareSerial.h>
#include <avr/pgmspace.h>

SoftwareSerial BT(2, 3);  // RX, TX

// === Pines ===
int a = 13;
int b = 12;
int c = 11;
int d = 10;
int y = 9;   // tail lamp (relé)
int z = 7;   // front lamp
const int speakerPin = 8; // bocina PWM

// === Variables ===
char r = 0;
bool tailState = false;
bool frontState = false;


void setup() 
{ 
  Serial.begin(9600);
  BT.begin(9600);

  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(y, OUTPUT);
  pinMode(z, OUTPUT);
  pinMode(speakerPin, OUTPUT);

  // === Apagar todo al inicio ===
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(y, LOW); // relé apagado
  digitalWrite(z, LOW);
}

void loop()
{
  // === Procesar comandos Bluetooth SOLO cuando llegan ===
  if (BT.available()) {
    char cmd = BT.read();

    // Ignorar saltos de línea
    if (cmd == '\r' || cmd == '\n') return;

    Serial.print("Comando: ");
    Serial.println(cmd);

    // === Movimiento ===
    if (cmd == 'S') { digitalWrite(a, LOW); digitalWrite(b, LOW); digitalWrite(c, LOW); digitalWrite(d, LOW); }
    if (cmd == 'F') { digitalWrite(a, HIGH); digitalWrite(b, LOW); digitalWrite(c, HIGH); digitalWrite(d, LOW); }
    if (cmd == 'B') { digitalWrite(a, LOW); digitalWrite(b, HIGH); digitalWrite(c, LOW); digitalWrite(d, HIGH); }
    if (cmd == 'R') { digitalWrite(a, HIGH); digitalWrite(b, LOW); digitalWrite(c, LOW); digitalWrite(d, HIGH); }
    if (cmd == 'L') { digitalWrite(a, LOW); digitalWrite(b, HIGH); digitalWrite(c, HIGH); digitalWrite(d, LOW); }

    // === Bocina ===
    if (cmd == 'V');
    if (cmd == 'v');

    // === Luces traseras: Toggle (relé activo en HIGH) ===
    if (cmd == 'U') {
      tailState = !tailState;
      digitalWrite(y, tailState ? HIGH : LOW);
    }

    // === Luces frontales: Toggle ===
    if (cmd == 'W') {
      frontState = !frontState;
      digitalWrite(z, frontState ? HIGH : LOW);
    }
  }

  delay(1);
}
