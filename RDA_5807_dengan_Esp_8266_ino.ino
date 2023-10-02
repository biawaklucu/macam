#include <Wire.h>
#include <EEPROM.h>

unsigned long lastButtonPress = 0;
const int clk = D5;
const int dt = D6;
const int sw = D7;

boolean TurnDetected,TurnDetectedV;
boolean up,upV;
boolean saklar;
int fs,s;
int freq,v;
int freqB;
byte freqH, freqL;

const int sclP = D1;
const int sdaP = D2;
const int Lamp = D4;

void setup() {

  //freq=1031; //gen fm
  saklar=true;
  freq = 877;
  Serial.begin(115200);

  Wire.begin(sdaP, sclP);
  radioInit();
  setFreq();
  setVolume();

  pinMode(sw, INPUT_PULLUP);
  pinMode(clk, INPUT_PULLUP);
  pinMode(dt, INPUT_PULLUP);
  pinMode(Lamp,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(clk), isr0, RISING);
}

void loop() {
  /*  if(digitalRead(vDetect)==0&&(freq>=870&&freq<=1079)){
        EEPROM.update(0,freq);
    }*/

int btnState = digitalRead(sw);

if (btnState == LOW) {
    if (millis() - lastButtonPress > 50) {
        Serial.println("Button pressed!");
        s=1;
    }
    lastButtonPress = millis();
}
 
if(s==1&&fs==0){saklar=true;fs=1;s=0;}
else if(s==1&&fs==1){saklar=false;fs=0;s=0;}


if(saklar==true){aturFreq();}
else if(saklar==false){aturVolume();}


}

ICACHE_RAM_ATTR void isr0() {
    TurnDetected = true;
  up = (digitalRead(clk) == digitalRead(dt));
    TurnDetectedV = true;
  upV = (digitalRead(clk) == digitalRead(dt));
}

void aturVolume(){

  if (TurnDetectedV) {
    Serial.println(v);
    if (upV) {
      if (v >= 15) {
        setVolume();
      } else {
        v = v + 1;
        setVolume();
      }
    } else {
      if (v <= 0) {
        setVolume();
      } else {
        v = v - 1;
        setVolume();
      }
    }
    TurnDetectedV = false;
  }  
}

void aturFreq(){
   
  if (TurnDetected) {
    Serial.println(freq);
    if (up) {
      if (freq >= 1079) {
        setFreq();
      } else {
        freq = freq + 1;
        setFreq();
      }
    } else {
      if (freq <= 870) {
        setFreq();
      } else {
        freq = freq - 1;
        setFreq();
      }
    }
    TurnDetected = false;
  }

  if (freq >= 1079) {
    lampu();
  } else if (freq <= 870) {
    digitalWrite(Lamp, LOW);
  } else {
    digitalWrite(Lamp, HIGH);
  }
}
///////////////////////
void radioInit() {
  // Initialize the RDA5807M chip 
  Wire.beginTransmission(0x11);  // Device address is 0x11
  Wire.write(0x02);              // Register address 0x02
  Wire.write(0xD0);
  Wire.write(0x03);              // Initialize the settings
  Wire.endTransmission();        // stop condition
  delay(500);                    // wait 500ms
  Wire.beginTransmission(0x11);  // Device address is 0x11
  Wire.write(0x02);              // Register address 0x02
  Wire.write(0xD0);
  Wire.write(0x05);  // Setup the radio for communications
  Wire.endTransmission();
  delay(500);
}
///////////////////////////////////
void setFreq() {
  freqB = freq - 870;        // chip needs to have freq offset from lowest freq (870)
  freqH = freqB >> 2;        // you need to break the offset freq into 2 parts (hi/low)
  freqL = (freqB & 3) << 6;  // Shift channel selection for matching register 0x03
  Wire.beginTransmission(0x11);
  Wire.write(0x03);
  Wire.write(freqH);         // write High freq byte
  Wire.write(freqL + 0x10);  // write Low freq byte
  Wire.endTransmission();
}
////////////////////////////////////
void setVolume() {
  // The volume is from 0-F and its the first bytes, leave all the bytes (0x84D0 - 0x84DF)
  Wire.beginTransmission(0x11);
  Wire.write(0x05);
  Wire.write(0x84);
  Wire.write(0xD0+v);  // set volume to 13
//0Serial.println(0xD0+v);  
  Wire.endTransmission();
}

void lampu() {
  digitalWrite(Lamp, HIGH);
  delay(400);
  digitalWrite(Lamp, LOW);
  delay(400);
}