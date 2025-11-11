void setup() {
  /* Usamos la constante LED_BUILTIN que es el pin digital donde se 
    conecta el LED interno de la placa, el compilador traduce automaticamente
    al pin correcto dependiendo de la placa.
  */
  /* pinMode(LED_BUILTIN, OUTPUT):
     para ser más explicitos podemos declarar un pin digital como salida/entrada
     esto hace el codigo dependiente del hardware, pero no importa en
     este caso
     Para la placa arduino uno r3 smd, dan una salida de 5V
     */
  pinMode(12, OUTPUT);
}

void loop() {
  digitalWrite(12, HIGH); // Salida de 5V, el led enciende
  delay(500); // espera 0.5 segundos
  digitalWrite(12, LOW); // Salida de 0V, el led se apaga
  delay(500);
  for( int i = 0; i < 5; i++)
  {
    digitalWrite(12, HIGH); // Salida de 5V, el led enciende
    delay(100); // espera 0.5 segundos
    digitalWrite(12, LOW); // Salida de 0V, el led se apaga
    delay(100); // espera 0.5 segundos
  }
}
