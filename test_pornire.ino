#include <Servo.h>

Servo esc1, esc2, esc3, esc4;

void setup() {
  esc1.attach(3);
  esc2.attach(5);
  esc3.attach(6);
  esc4.attach(9);

  // Armare ESC-uri (semnal minim)
  esc1.writeMicroseconds(1000);
  esc2.writeMicroseconds(1000);
  esc3.writeMicroseconds(1000);
  esc4.writeMicroseconds(1000);
  delay(2000); // Așteaptă armarea

  // Setare turatie hover (ex: 1250)
  esc1.writeMicroseconds(1020);
  esc2.writeMicroseconds(1020);
  esc3.writeMicroseconds(1020);
  esc4.writeMicroseconds(1020); // Ajustare pentru ESC diferit
  delay(4000); // Motoarele merg 4 secunde

  // Oprire motoare
  esc1.writeMicroseconds(1000);
  esc2.writeMicroseconds(1000);
  esc3.writeMicroseconds(1000);
  esc4.writeMicroseconds(1010);
}

void loop() {
  // Nimic de făcut după
}
