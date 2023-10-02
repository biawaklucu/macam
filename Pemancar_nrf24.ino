#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9,8); // CE, CSN

void setup() {
  pinMode(A1, INPUT_PULLUP);
  radio.begin();
  radio.openWritingPipe(0xF0F0F0F0E1LL);
  radio.setPALevel(RF24_PA_MAX);
}

void loop() {
  if (digitalRead(A1) == LOW) {
    const char text[] = "ON";
    radio.write(&text, sizeof(text));
  }  else {
    const char text[] = "OFF";
    radio.write(&text, sizeof(text));
  }
  
}
