#include <Servo.h>
#include <SoftwareSerial.h>

// Pini pentru SoftwareSerial (Bluetooth)
#define BT_RX 11
#define BT_TX 10
SoftwareSerial BT(BT_RX, BT_TX); // RX, TX

Servo esc1; // ESC roșu
Servo esc2; // ESC roșu
Servo esc3; // ESC roșu
Servo esc4; // ESC verde

int pwm_common = 1020;
int pwm_step = 10;
int pwm_min = 1000;
int pwm_max = 2000;
int corectie_esc4 = 10;

bool motors_started = false;

void setup() {
  Serial.begin(9600);  // Pentru monitor serial (USB)
  BT.begin(9600);      // Pentru Bluetooth HC-05

  esc1.attach(3);
  esc2.attach(5);
  esc3.attach(6);
  esc4.attach(9);

  esc1.writeMicroseconds(pwm_min);
  esc2.writeMicroseconds(pwm_min);
  esc3.writeMicroseconds(pwm_min);
  esc4.writeMicroseconds(pwm_min);

  Serial.println("Sistem gata. Comenzi: s = start | + = add pwm | - = minus pwm | x = stop | c = compensatie");
  BT.println("Sistem gata. Comenzi: s = start | + = add pwm | - = minus pwm | x = stop | c = compensatie");
}

void loop() {
  if (BT.available()) {
    char c = BT.read();
    Serial.print("Tasta primită: "); Serial.println(c);

    switch (c) {
      case 's': // start
        pwm_common = 1050;
        esc1.writeMicroseconds(pwm_common);
        esc2.writeMicroseconds(pwm_common);
        esc3.writeMicroseconds(pwm_common);
        esc4.writeMicroseconds(pwm_common + corectie_esc4);
        motors_started = true;
        Serial.println("Motoare armate și pornite.");
        BT.println("Motoare armate și pornite.");
        break;

      case '+': // add pwm
        if (motors_started) {
          pwm_common += pwm_step;
          if (pwm_common > pwm_max) pwm_common = pwm_max;
          updateMotors();
          Serial.println("PWM crescut.");
          BT.println("PWM crescut.");
        }
        break;

      case '-': // minus pwm
        if (motors_started) {
          pwm_common -= pwm_step;
          if (pwm_common < pwm_min) pwm_common = pwm_min;
          updateMotors();
          Serial.println("PWM scăzut.");
          BT.println("PWM scăzut.");
        }
        break;

      case 'x': // stop
        if (motors_started) {
          Serial.println("Oprire motoare graduală...");
          BT.println("Oprire motoare graduală...");
          for (int val = pwm_common; val >= pwm_min; val -= pwm_step) {
            esc1.writeMicroseconds(val);
            esc2.writeMicroseconds(val);
            esc3.writeMicroseconds(val);
            esc4.writeMicroseconds(val + corectie_esc4);
            delay(100);
          }
          motors_started = false;
          Serial.println("Motoare oprite.");
          BT.println("Motoare oprite.");
        }
        break;

      case 'c': // compensatie
        corectie_esc4 += pwm_step;
        if ((pwm_common + corectie_esc4) > pwm_max) {
          corectie_esc4 = pwm_max - pwm_common;
        }
        updateMotors();
        Serial.println("Compensare ESC 4 crescută.");
        BT.println("Compensare ESC 4 crescută.");
        break;

      default:
        Serial.print("Comandă necunoscută: "); Serial.println(c);
        BT.print("Comandă necunoscută: "); BT.println(c);
        break;
    }

    printPWMValues();
  }
}

void updateMotors() {
  esc1.writeMicroseconds(pwm_common);
  esc2.writeMicroseconds(pwm_common);
  esc3.writeMicroseconds(pwm_common);
  esc4.writeMicroseconds(pwm_common + corectie_esc4);
}

void printPWMValues() {
  Serial.print("ESC1: "); Serial.print(pwm_common);
  Serial.print(" | ESC2: "); Serial.print(pwm_common);
  Serial.print(" | ESC3: "); Serial.print(pwm_common);
  Serial.print(" | ESC4: "); Serial.print(pwm_common + corectie_esc4);
  Serial.print(" | Compensatie ESC4: "); Serial.println(corectie_esc4);

  BT.print("ESC1: "); BT.print(pwm_common);
  BT.print(" | ESC2: "); BT.print(pwm_common);
  BT.print(" | ESC3: "); BT.print(pwm_common);
  BT.print(" | ESC4: "); BT.print(pwm_common + corectie_esc4);
  BT.print(" | Compensatie ESC4: "); BT.println(corectie_esc4);
}