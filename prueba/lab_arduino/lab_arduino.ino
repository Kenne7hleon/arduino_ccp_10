//Pines de entrada
#define BTN1 12
#define BTN2 11
#define BTN3 10
#define BTN_RESET 13
// Pines de salida
#define LED1 2
#define LED2 3
#define LED3 4

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);
  pinMode(BTN_RESET, INPUT_PULLUP);
  Serial.begin(9600);
}

int posicion = 0;
bool desbloqueado = false;

void loop() {
  // Si el candado está desbloqueado, solo se espera el botón de reinicio
  if (desbloqueado) {
    if (digitalRead(BTN_RESET) == LOW) {   // presionado con pull-up
      posicion = 0;
      desbloqueado = false;
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      Serial.println("Candado reiniciado. Esperando secuencia...");
      delay(250);
    }
    return;
  }

  // Revisar primero si se presiona el botón de reinicio
  if (digitalRead(BTN_RESET) == LOW) {   // presionado
    posicion = 0;
    desbloqueado = false;
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    Serial.println("Candado reiniciado manualmente. Esperando secuencia...");
    delay(250);
    return;
  }

  // Detectar si se presionó alguno de los tres botones de la secuencia
  int botonPresionado = 0;

  if (digitalRead(BTN1) == LOW) {
    botonPresionado = 1;
  } else if (digitalRead(BTN2) == LOW) {
    botonPresionado = 2;
  } else if (digitalRead(BTN3) == LOW) {
    botonPresionado = 3;
  }

  if (botonPresionado == 0) {
    return;
  }

  Serial.print("Boton presionado: ");
  Serial.println(botonPresionado);

  bool correcto = false;

  if (posicion == 0) {
    if (botonPresionado == 1) correcto = true;
  } else if (posicion == 1) {
    if (botonPresionado == 3) correcto = true;
  } else if (posicion == 2) {
    if (botonPresionado == 2) correcto = true;
  }

  if (correcto) {
    posicion++;
    Serial.print("Posicion correcta, ahora posicion = ");
    Serial.println(posicion);

    if (posicion == 3) {
      desbloqueado = true;
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      Serial.println("Candado desbloqueado. LEDs encendidos.");
    }
   else {
    Serial.println("Error en la secuencia, se reinicia el candado.");
    posicion = 0;
    desbloqueado = false;
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
   }
  }

  delay(250);
}