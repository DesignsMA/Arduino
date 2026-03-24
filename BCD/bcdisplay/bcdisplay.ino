#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int PIN_D0 = 2;
const int PIN_D1 = 3;
const int PIN_D2 = 4;
const int PIN_D3 = 5;

int ultimoBCD = -1;

int leerPinConDebounce(int pin, int lecturas = 3) {
  int suma = 0;
  for (int i = 0; i < lecturas; i++) {
    suma += digitalRead(pin);
    delay(2);
  }
  return (suma >= lecturas / 2) ? HIGH : LOW;
}

void setup() {
  pinMode(PIN_D0, INPUT_PULLUP);
  pinMode(PIN_D1, INPUT_PULLUP);
  pinMode(PIN_D2, INPUT_PULLUP);
  pinMode(PIN_D3, INPUT_PULLUP);
  
  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("BCD -> Decimal");
  lcd.setCursor(0, 1);
  lcd.print("Esperando...");
  
  delay(2000);
  
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  
  Serial.begin(9600);
}

void loop() {
  int bit0 = !leerPinConDebounce(PIN_D0);
  int bit1 = !leerPinConDebounce(PIN_D1);
  int bit2 = !leerPinConDebounce(PIN_D2);
  int bit3 = !leerPinConDebounce(PIN_D3);
  
  int codigoBCD = (bit3 << 3) | (bit2 << 2) | (bit1 << 1) | bit0;
  int valorDecimal = (bit3 * 8) + (bit2 * 4) + (bit1 * 2) + (bit0 * 1);
  
  if (codigoBCD != ultimoBCD) {
    
    lcd.setCursor(0, 0);
    lcd.print("BCD: ");
    lcd.print(bit3); lcd.print(bit2); 
    lcd.print(bit1); lcd.print(bit0);
    lcd.print("    ");
    
    lcd.setCursor(0, 1);
    if (valorDecimal <= 9) {
      lcd.print("Decimal: ");
      lcd.print(valorDecimal);
      lcd.print("   ");
    } else {
      lcd.print("INVALIDO: ");
      lcd.print(valorDecimal);
      lcd.print(" ");
    }
    
    ultimoBCD = codigoBCD;
    
    Serial.print("BCD: ");
    Serial.print(bit3); Serial.print(bit2);
    Serial.print(bit1); Serial.print(bit0);
    Serial.print(" = ");
    Serial.println(valorDecimal <= 9 ? String(valorDecimal) : "INVALIDO");
  }
  
  delay(100);
}