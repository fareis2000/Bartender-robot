#include <SoftwareSerial.h>

/* ================= PINOS ================= */
const int amarelo = 2;
const int vermelho = 3;
const int verde = 4;
const int azul = 5;
const int LedAntena = 6;
const int buzzer = 7;

/* ============== BLUETOOTH ================ */
SoftwareSerial bluetooth(12, 13); // RX, TX

/* ============== NOTAS MUSICAIS ============ */
#define NOTE_REST 0

#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494

#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988

#define NOTE_C6  1047

/* ========================================= */

void setup() {
  pinMode(amarelo, OUTPUT);
  pinMode(vermelho, OUTPUT);
  pinMode(verde, OUTPUT);
  pinMode(azul, OUTPUT);
  pinMode(LedAntena, OUTPUT);
  pinMode(buzzer, OUTPUT);

  digitalWrite(LedAntena, LOW);

  // Bombas desligadas (relé ativo em LOW)
  digitalWrite(amarelo, HIGH);
  digitalWrite(vermelho, HIGH);
  digitalWrite(verde, HIGH);
  digitalWrite(azul, HIGH);

  Serial.begin(9600);
  bluetooth.begin(9600);

  Serial.println("Sistema pronto!");
}

void loop() {

  if (bluetooth.available()) {
    String cmd = bluetooth.readStringUntil('\n');
    cmd.trim();

    Serial.println("Recebido: [" + cmd + "]");

    if (cmd == "RED") {
      RED();
    }
    else if (cmd == "YELLOW") {
      Yellow();
    }
    else if (cmd == "GREEN") {
      Green();
    }
    else if (cmd == "MIX") {
      Mix();
    }
    else {
      bluetooth.println("CMD_ERR");
      Serial.println("Comando inválido");
    }
  }
}

/* ================= DRINKS ================= */

void RED() {
  digitalWrite(vermelho, LOW);
  delay(5000);
  digitalWrite(vermelho, HIGH);

  terminardrink();
}

void Yellow() {
  digitalWrite(amarelo, LOW);
  delay(4000);
  digitalWrite(amarelo, HIGH);
  delay(3000);

  terminardrink();
}

void Green() {
  digitalWrite(verde, LOW);
  delay(3000);
  digitalWrite(verde, HIGH);

  terminardrink();
}

void Mix() {
  digitalWrite(azul, LOW);
  delay(3000);
  digitalWrite(azul, HIGH);

  terminardrink();
}

/* ============== FINALIZAÇÃO =============== */

void terminardrink() {
  digitalWrite(LedAntena, HIGH);
  delay(2000);
  digitalWrite(LedAntena, LOW);

  tocarmusica();

  // Aviso para Flask/Python
  Serial.println("DONE");
  bluetooth.println("DONE");
}

/* ============== MÚSICA ==================== */

void tocarmusica() {

 int melody[] = {
    NOTE_C5, NOTE_C5, NOTE_C5,
    NOTE_C5,
    NOTE_GS4,   // Ab
    NOTE_AS4,   // Bb
    NOTE_C5,
    NOTE_REST,
    NOTE_AS4,
    NOTE_C5
  };

  int durations[] = {
    8, 8, 8,
    3,
    3,
    3,
    8,
    8,
    8,
    1
  };

  int tamanho = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < tamanho; i++) {
    int duracao = 1000 / durations[i];
    tone(buzzer, melody[i], duracao);
    delay(duracao * 1.35);
    noTone(buzzer);
  }
}


