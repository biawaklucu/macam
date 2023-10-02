#include <AFMotor.h>
#define led A5
#define NOTE_B0 31
#include <RCSwitch.h>

 RCSwitch rf433 = RCSwitch(); 
    


int melody[] = {
    // Nada "bib" sederhana:
    NOTE_B0,
};

// inisialisasi motor
AF_DCMotor motor1(1); 
AF_DCMotor motor2(2);
AF_DCMotor motor3(3); 
AF_DCMotor motor4(4);

int kecepatan = 9; // Nilai default kecepatan

void setup() 
{        
  Serial.begin(9600);  //Set Baud Rate
   pinMode(led,OUTPUT);
    rf433.enableReceive(0);//interupt 0 atau pin digital 2
}

void loop() 
    {  
  if((Serial.available() > 0)||(rf433.available()) )
  {
    char data = Serial.read();
     char dataRF =rf433.getReceivedValue();
        
     if(data=='U')
        {
            lampuOn();
            }
        else if(data=='u')
        {
            digitalWrite(led,LOW);
            }
        
    //Kontrol Motor
    if(data == 'F') //Maju
    {
      maju();
    }
    else if(data == 'B') //Mundur
    {
      mundur();
    }
    else if(data == 'L') //Belok Kiri
    {
      belokKiri();
    }
    else if(data == 'R') //Belok Kanan
    {
      belokKanan();
    }
    else if(data == 'I') //Maju Kiri
    {
      majuKiri();
    }
    else if(data == 'G') //Maju Kanan
    {
      majuKanan();
    }
    else if(data == 'J') //Mundur Kiri
    {
      mundurKiri();
    }
    else if(data == 'H') //Mundur Kanan
    {
      mundurKanan();
    }
    else if(data == 'S') //Berhenti
    {
      berhenti();
    }
        else if(data == 'V') //MUSIK
    {
      musik();
    }
     
    else if(data >= '0' && data <= '9') //Skala Kecepatan
    {
      kecepatan = data - '0'; // Simpan skala kecepatan dalam variabel kecepatan
      setKecepatan(kecepatan * 28); // Skala kecepatan dari 0 (0*25.5) hingga 255 (9*25.5)
     
    }
  }
    
      
}

void setKecepatan(int kecepatan)
{
  motor1.setSpeed(kecepatan);
  motor2.setSpeed(kecepatan);
  motor3.setSpeed(kecepatan);
  motor4.setSpeed(kecepatan);
}

void maju()
{
  setKecepatan(kecepatan * 28);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void mundur()
{
  setKecepatan(kecepatan * 28);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

void belokKiri()
{
  //setKecepatan(kecepatan * 28);
     motor2.setSpeed(kecepatan*28);
  motor2.run(BACKWARD);
     motor3.setSpeed(kecepatan*28);
  motor3.run(BACKWARD);
    
     motor1.setSpeed(kecepatan/2*28);  motor1.run(FORWARD);
     motor4.setSpeed(kecepatan/2*28);
  motor4.run(FORWARD);
}

void belokKanan()
{
 // setKecepatan(kecepatan * 28);
   motor2.setSpeed(kecepatan*28); 
  motor2.run(FORWARD);
    motor3.setSpeed(kecepatan*28);
  motor3.run(FORWARD);
    
    motor1.setSpeed(kecepatan/2*28);
  motor1.run(BACKWARD);
     motor4.setSpeed(kecepatan/2*28);
  motor4.run(BACKWARD);
}

void majuKanan()
{
  setKecepatan(kecepatan * 28);
  motor3.run(RELEASE);
  motor2.run(RELEASE);
  motor1.run(FORWARD);
  motor4.run(FORWARD);
}

void majuKiri()
{
  setKecepatan(kecepatan * 28);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor1.run(RELEASE);
  motor4.run(RELEASE);
}

void mundurKiri()
{
  setKecepatan(kecepatan * 28);
  motor1.run(RELEASE);
  motor4.run(RELEASE);
  motor3.run(BACKWARD);
  motor2.run(BACKWARD);
}

void mundurKanan()
{
  setKecepatan(kecepatan * 28);
  motor1.run(BACKWARD);
  motor4.run(BACKWARD);
  motor3.run(RELEASE);
  motor2.run(RELEASE);
}

void berhenti()
{
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void musik() {
    for (int i = 0; i < sizeof(melody) / sizeof(int); i++) {
       // tone(A5, melody[i], 1000); // Mainkan nada selama 1 detik
        delay(1000); // Tunggu selama 1 detik
        noTone(A5); // Matikan nada
        delay(1000); // Tunggu selama 1 detik sebelum memainkan nada berikutnya
    }
}

void lampuOn(){
    digitalWrite(led,HIGH);
    }
