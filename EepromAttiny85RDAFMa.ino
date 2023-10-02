
//attiny85
// sda pin 5
//scl pin 7

#include <TinyWireM.h>
#include <EEPROM.h>

#define Lamp PB5
#define clk PB1
#define dt PB3
#define vDetect PB4

volatile boolean TurnDetected;
volatile boolean up;

int freq;
int freqB;
byte freqH, freqL;

void isr0 ()  {
  TurnDetected = true;
  up = (digitalRead(clk) == digitalRead(dt));
}

ISR(PCINT0_vect)
    {
     isr0();
  }

void setup()
{

 freq=1031; 
 /*
int a=EEPROM.read(0);
if(a>=870||a<=1079){
  //freq=a;
}
else {
  freq=1031;
}*/
TinyWireM.begin();
radioInit();
setFreq();
setVolume();
    
cli(); // Disable interrupts during setup
PCMSK |= (1 << PCINT1); // Enable interrupt handler (ISR) for our chosen interrupt pin (PCINT1/PB1/pin 6)
GIMSK |= (1 << PCIE); // Enable PCINT interrupt in the general interrupt mask
pinMode(clk, INPUT); // Set our interrupt pin as input with a pullup to keep it stable
pinMode(dt,INPUT); 
pinMode(Lamp,OUTPUT); 
    sei(); //last line of setup - enable interrupts after setup
pinMode(vDetect,INPUT);
}

void loop(){
  /*  if(digitalRead(vDetect)==0&&(freq>=870&&freq<=1079)){
        EEPROM.update(0,freq);
    }*/
   
if (TurnDetected){
    if(up){
      if (freq >= 1079){
        setFreq();
       }
      else{   
      freq=freq+1;
      setFreq();
      }}
    else{
      if (freq <= 870){
        setFreq();
        }
      else{
      freq=freq-1;
      setFreq();
      }}
      TurnDetected = false;
    }
    
   if(freq>=1079){
    lampu();
   } else if(freq<=870){
    digitalWrite(Lamp,HIGH);
   }else{digitalWrite(Lamp,LOW);}
}
 

///////////////////////
void radioInit(){
// Initialize the RDA5807M chip 
TinyWireM.beginTransmission(0x11); // Device address is 0x11
TinyWireM.write(0x02); // Register address 0x02
TinyWireM.write(0xD0); TinyWireM.write(0x03); // Initialize the settings
TinyWireM.endTransmission(); // stop condition
delay(500); // wait 500ms
TinyWireM.beginTransmission(0x11); // Device address is 0x11
TinyWireM.write(0x02); // Register address 0x02
TinyWireM.write(0xD0); TinyWireM.write(0x05); // Setup the radio for communications
TinyWireM.endTransmission();
delay(500);
}
///////////////////////////////////
void setFreq(){
  freqB = freq - 870; // chip needs to have freq offset from lowest freq (870)
    freqH = freqB>>2; // you need to break the offset freq into 2 parts (hi/low)
    freqL = (freqB&3)<<6; // Shift channel selection for matching register 0x03
    TinyWireM.beginTransmission(0x11);
    TinyWireM.write(0x03);
    TinyWireM.write(freqH); // write High freq byte
    TinyWireM.write(freqL + 0x10); // write Low freq byte
    TinyWireM.endTransmission();
}
////////////////////////////////////
void setVolume(){
 // The volume is from 0-F and its the first bytes, leave all the bytes (0x84D0 - 0x84DF)   
TinyWireM.beginTransmission(0x11);
TinyWireM.write(0x05);
TinyWireM.write(0x84); TinyWireM.write(0xDD); // set volume to 13
TinyWireM.endTransmission();
}

void lampu(){
digitalWrite(Lamp,HIGH);
delay(400);
digitalWrite(Lamp,LOW);
delay(400);
}
