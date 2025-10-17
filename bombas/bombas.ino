#include <SoftwareSerial.h>

const int bomba1Pin = 7;
const int bomba2Pin = 8;

SoftwareSerial bluetooth(13, 12); // RX, TX (HC-05 TX -> 13 | HC-05 RX <- 12)

void setup() {
  pinMode(bomba1Pin, OUTPUT);
  pinMode(bomba2Pin, OUTPUT);

  digitalWrite(bomba1Pin, HIGH);
  digitalWrite(bomba2Pin, HIGH);

  Serial.begin(9600);
  bluetooth.begin(9600);
  
  // Aguarda a inicialização da serial
  delay(1000);
  
  Serial.println("Iniciando...");
  
  // Testa comunicação bluetooth
  bluetooth.println("AT");
  delay(1000);
  
  if (bluetooth.available()) {
    Serial.println("Módulo bluetooth respondeu!");
  } else {
    Serial.println("AVISO: Módulo bluetooth não respondeu!");
  }
  
  Serial.println("Sistema pronto para comandos");
}

void loop() {
  // Debug para mostrar se há dados disponíveis
  if (Serial.available()) {
    String serialCmd = Serial.readStringUntil('\n');
    Serial.println("Serial recebido: " + serialCmd);
  }

  if (bluetooth.available()) {
    String cmd = bluetooth.readStringUntil('\n');
    cmd.trim();
    
    Serial.println("Bluetooth recebido: [" + cmd + "]");

    // Envia confirmação de recebimento
    bluetooth.print("Recebido: ");
    bluetooth.println(cmd);

    if (cmd == "SODA_ITALIANA") {
      sodaitaliana();
    } 
    else if (cmd == "B1_OFF") {
      digitalWrite(bomba1Pin, HIGH);
      bluetooth.println("B1_OK");
      Serial.println("Bomba 1 desligada");
    } 
    else if (cmd == "B2_ON") {
      digitalWrite(bomba2Pin, LOW);
      bluetooth.println("B2_OK");
      Serial.println("Bomba 2 ligada");
    } 
    else if (cmd == "B2_OFF") {
      digitalWrite(bomba2Pin, HIGH);
      bluetooth.println("B2_OK");
      Serial.println("Bomba 2 desligada");
    } 
    else {
      bluetooth.println("CMD_ERR");
    }
  }
}


void sodaitaliana(){
  digitalWrite(bomba2Pin, LOW);
  delay(1000);
  digitalWrite(bomba2Pin, HIGH);
  delay(1000);
  digitalWrite(bomba1Pin, LOW);
  delay(5000);
  digitalWrite(bomba1Pin, HIGH);
  
  
}
