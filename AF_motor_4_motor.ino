#include <AFMotor.h>

// inisialisasi motor
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

void setup() {
  Serial.begin(9600);  // inisialisasi komunikasi serial
}

void loop() {
  if (Serial.available()) { // jika ada data serial masuk
    char command = Serial.read(); // baca perintah

    Serial.print("Perintah diterima: ");
    Serial.println(command);

    switch (command) {
      case 'f': // maju
        maju();
        break;
      case 'b': // mundur
        mundur();
        break;
      case 'l': // belok kiri
        belokKiri();
        break;
      case 'r': // belok kanan
        belokKanan();
        break;
      case 'x': // stop
        stop();
        break;
      default: // jika tidak ada perintah yang cocok, berhenti juga
        stop();
    }
    while(Serial.available() > 0) { // selama masih ada data di buffer serial
      Serial.read(); // baca dan buang data tersebut
    }
  } else { // jika tidak ada data serial masuk, berhenti
    stop();
  }
}

void maju() {
  motor1.setSpeed(255); // kecepatan maksimum
  motor1.run(FORWARD); // arah maju
  motor2.setSpeed(255);
  motor2.run(FORWARD);
  motor3.setSpeed(255);
  motor3.run(FORWARD);
  motor4.setSpeed(255);
  motor4.run(FORWARD);
}

void mundur() {
  motor1.setSpeed(255);
  motor1.run(BACKWARD); // arah mundur
  motor2.setSpeed(255);
  motor2.run(BACKWARD);
  motor3.setSpeed(255);
  motor3.run(BACKWARD);
  motor4.setSpeed(255);
  motor4.run(BACKWARD);
}

void belokKiri() {
  // Motor di satu sisi bergerak maju dan di sisi lain bergerak mundur
  motor1.setSpeed(255);
  motor1.run(FORWARD); 
  motor2.setSpeed(255);
  motor2.run(BACKWARD); 
  motor3.setSpeed(255);
  motor3.run(FORWARD); 
  motor4.setSpeed(255);
  motor4.run(BACKWARD); 
}

void belokKanan() {
  // Motor di satu sisi bergerak mundur dan di sisi lain bergerak maju
  motor1.setSpeed(255);
  motor1.run(BACKWARD); 
  motor2.setSpeed(255);
  motor2.run(FORWARD); 
  motor3.setSpeed(255);
  motor3.run(BACKWARD); 
  motor4.setSpeed(255);
  motor4.run(FORWARD); 
}

void stop() {
  motor1.run(RELEASE); 
  motor2.run(RELEASE); 
  motor3.run(RELEASE); 
  motor4.run(RELEASE); 
}
