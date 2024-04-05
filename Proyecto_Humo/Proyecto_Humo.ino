// Incluir las bibliotecas necesarias
#include <Arduino.h>

// Definir los pines para el sensor PIR y la válvula solenoide
#define PIR_PIN 14 // Puedes cambiar estos números según la configuración de tu ESP32
#define SOLENOID_PIN 12

// Variables para controlar el estado del sensor PIR y la válvula solenoide
bool movimientoDetectado = false;
bool valvulaActivada = false;
unsigned long tiempoUltimoMovimiento = 0;
const unsigned long tiempoEspera = 10000; // 10 segundos
const unsigned long tiempoActuacionValvula = 30000; // 30 segundos

void setup() {
  // Inicializar los pines
  pinMode(PIR_PIN, INPUT);
  pinMode(SOLENOID_PIN, OUTPUT);
  
  // Iniciar la comunicación serial para la depuración
  Serial.begin(9600);
}

void loop() {
  // Leer el estado del sensor PIR
  int estadoSensorPIR = digitalRead(PIR_PIN);
  
  // Si se detecta movimiento y han pasado al menos 10 segundos desde el último movimiento
  if (estadoSensorPIR == HIGH && !movimientoDetectado && millis() - tiempoUltimoMovimiento >= tiempoEspera) {
    Serial.println("Movimiento detectado, activando la válvula solenoide...");
    
    // Activar la válvula solenoide
    digitalWrite(SOLENOID_PIN, HIGH);
    valvulaActivada = true;
    
    // Marcar que se ha detectado movimiento
    movimientoDetectado = true;
    
    // Guardar el tiempo de detección
    tiempoUltimoMovimiento = millis();
  }
  
  // Si la válvula está activada y han pasado 30 segundos
  if (valvulaActivada && millis() - tiempoUltimoMovimiento >= tiempoActuacionValvula) {
    Serial.println("Desactivando la válvula solenoide...");
    
    // Desactivar la válvula solenoide
    digitalWrite(SOLENOID_PIN, LOW);
    valvulaActivada = false;
  }
  
  // Si no hay movimiento y se había detectado previamente
  if (estadoSensorPIR == LOW && movimientoDetectado) {
    // Reiniciar el estado de detección de movimiento después de 10 segundos
    if (millis() - tiempoUltimoMovimiento >= tiempoEspera) {
      Serial.println("No hay movimiento, reiniciando...");
      movimientoDetectado = false;
    }
  }
}




