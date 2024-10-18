/* Receive Incoming USB MIDI using functions.  As usbMIDI
   reads incoming messages, handler functions are run.
   See the InputRead example for the non-function alterative.

   This small example shows only 3 common messages, to keep
   things simple.  See InputFunctionsComplete for an example
   with all possible messages.

   Use the Arduino Serial Monitor to view the messages
   as Teensy receives them by USB MIDI

   You must select MIDI from the "Tools > USB Type" menu

   This example code is in the public domain.
*/

#define BASE 0
#define SNARE 1
#define CYM 2
#define HH1 3

#define HH2 8
#define RIM 9
#define TOMLOW 10
#define TOMHIGH 11

#define CLAVES 23
#define COWB 22
#define RES 21 //reset hihat
#define CONC 20 //change congas

#define CIN 12
#define COUT 15
#define ROUT 16

volatile byte state = LOW;
volatile int baseSt = 0;
volatile int snareSt = 0;
volatile int cymSt = 0;
volatile int hh1St = 0;

volatile int hh2St = 0;
volatile int rimSt = 0;
volatile int tomlSt = 0;
volatile int tomhSt = 0;

volatile int clavesSt = 0;
volatile int cowSt = 0;

volatile bool sync = false;
volatile int frameSt = 0;
volatile int startSt = 0;
volatile int framecnt = 0;


#define LED 13

IntervalTimer myTimer;

void setup() {

  pinMode(BASE, OUTPUT);
  pinMode(SNARE, OUTPUT);
  pinMode(CYM, OUTPUT);
  pinMode(HH1, OUTPUT);

  pinMode(HH2, OUTPUT);
  pinMode(RIM, OUTPUT);
  pinMode(TOMLOW, OUTPUT);
  pinMode(TOMHIGH, OUTPUT);

  pinMode(CLAVES, OUTPUT);
  pinMode(COWB, OUTPUT);
  pinMode(RES, OUTPUT);
  pinMode(CONC, OUTPUT);

  digitalWrite(BASE, HIGH);
  digitalWrite(SNARE, HIGH);
  digitalWrite(CYM, HIGH);
  digitalWrite(HH1, HIGH);
  digitalWrite(HH2, HIGH);
  digitalWrite(TOMLOW, HIGH);
  digitalWrite(TOMHIGH, HIGH);
  digitalWrite(RIM, HIGH);
  digitalWrite(CLAVES, HIGH);
  digitalWrite(COWB, HIGH);
  digitalWrite(RES, HIGH);
  digitalWrite(CONC, HIGH);

  pinMode(COUT, OUTPUT);
  pinMode(ROUT, OUTPUT);
  pinMode(CIN, INPUT);
  pinMode(LED, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(CIN), clkin, CHANGE);
   
  Serial.begin(115200);
  usbMIDI.setHandleNoteOn(myNoteOn);
  usbMIDI.setHandleNoteOff(myNoteOff);
  usbMIDI.setHandleControlChange(myControlChange);
  usbMIDI.setHandleClock(frame);
  usbMIDI.setHandleStart(srt);
  usbMIDI.setHandleSystemReset(mySystemReset);
  digitalWrite(RES, HIGH);
  digitalWrite(ROUT, HIGH);
  digitalWrite(COUT, LOW);
  myTimer.begin(tck, 1000);

  
}

void mySystemReset() {
  sync=true;
}

void clkin() {
  state = !state;
  digitalWrite(LED, state);
  if(!sync)digitalWrite(COUT, digitalRead(CIN));
}

void tck() {
  if(baseSt>0)baseSt--;
  if(snareSt>0)snareSt--;
  if(cymSt>0)cymSt--;
  if(hh1St>0)hh1St--;
  if(hh2St>0)hh2St--;
  if(tomlSt>0)tomlSt--;
  if(tomhSt>0)tomhSt--;
  if(rimSt>0)rimSt--;
  if(cowSt>0)cowSt--;
  if(clavesSt>0)clavesSt--;

  if(frameSt>0)frameSt--;
  if(startSt>0)startSt--;

  if(baseSt==0)digitalWrite(BASE, HIGH);
  if(snareSt==0)digitalWrite(SNARE, HIGH);
  if(cymSt==0)digitalWrite(CYM, HIGH);
  if(hh1St==0)digitalWrite(HH1, HIGH);
  if(hh2St==0)digitalWrite(HH2, HIGH);
  if(tomlSt==0)digitalWrite(TOMLOW, HIGH);
  if(tomhSt==0)digitalWrite(TOMHIGH, HIGH);
  if(rimSt==0)digitalWrite(RIM, HIGH);
  if(cowSt==0)digitalWrite(COWB, HIGH);
  if(clavesSt==0)digitalWrite(CLAVES, HIGH);

  if(frameSt==0)digitalWrite(COUT, LOW);
  if(startSt==0)digitalWrite(ROUT, HIGH);
  

  
}

void frame() {
  sync=true;
  if(++framecnt==3) {
      digitalWrite(COUT, HIGH);
      frameSt=2;
      framecnt=0;
  }  
}

void srt() {
  sync=true;
  digitalWrite(ROUT, LOW);
  startSt=2; 
}


void loop() {
  // The handler functions are called when usbMIDI reads data.  They
  // will not be called automatically.  You must call usbMIDI.read()
  // regularly from loop() for usbMIDI to actually read incoming
  // data and run the handler functions as messages arrive.
  usbMIDI.read();
  digitalWrite(LED, digitalRead(CIN));
  //digitalWrite(COUT, digitalRead(CIN));
  
}


void myNoteOn(byte channel, byte note, byte velocity) {
  // When using MIDIx4 or MIDIx16, usbMIDI.getCable() can be used
  // to read which of the virtual MIDI cables received this message.
  
  Serial.print("Note On, ch=");
  Serial.print(channel, DEC);
  Serial.print(", note=");
  Serial.print(note, DEC);
  Serial.print(", velocity=");
  Serial.println(velocity, DEC);
  
  switch(note) {
    case 36:
      Serial.print("Base");
      digitalWrite(BASE, LOW);
      baseSt=2;
      break;

    case 40:
      Serial.print("Snare");
      digitalWrite(SNARE, LOW);
      snareSt=2;
      break;
    case 49:
      Serial.print("Cymbal");
      digitalWrite(CYM, LOW);
      cymSt=2;
      break;
    case 42:
      Serial.print("HH1");
      digitalWrite(HH1, LOW);
      hh1St=2;
      break;
    case 46:
      Serial.print("HH2");
      digitalWrite(HH2, LOW);
      hh2St=2;
      break;
    case 45:
      Serial.print("Low tom");
      digitalWrite(CONC, LOW);
      digitalWrite(TOMLOW, LOW);
      tomlSt=2;
      break;
    case 50:
      Serial.print("high tom");
      digitalWrite(CONC, LOW);
      digitalWrite(TOMHIGH, LOW);
      tomhSt=2;
      break;
    case 64:
      Serial.print("Low conga");
      digitalWrite(CONC, HIGH);
      digitalWrite(TOMLOW, LOW);
      tomlSt=2;
      break;
    case 62:
      Serial.print("high conga");
      digitalWrite(CONC, HIGH);
      digitalWrite(TOMHIGH, LOW);
      tomhSt=2;
      break;
    case 56:
      //Serial.print("cowbell");
      digitalWrite(COWB, LOW);
      cowSt=2;
      break;
     case 75:
      //Serial.print("claves");
      digitalWrite(CLAVES, LOW);
      clavesSt=2;
      break;
     case 37:
      //Serial.print("rim");
      digitalWrite(RIM, LOW);
      rimSt=2;
      break;


    default:
     break;
  }
  
}

void myNoteOff(byte channel, byte note, byte velocity) {
  /*
  Serial.print("Note Off, ch=");
  Serial.print(channel, DEC);
  Serial.print(", note=");
  Serial.print(note, DEC);
  Serial.print(", velocity=");
  Serial.println(velocity, DEC);
  */
  switch(note) {
    case 46:
      Serial.print("HH2 close");
      digitalWrite(RES, LOW);
      delay(50);
      digitalWrite(RES, HIGH);
      break;

    default:
      break;

    
  }
}

void myControlChange(byte channel, byte control, byte value) {
  /*
  Serial.print("Control Change, ch=");
  Serial.print(channel, DEC);
  Serial.print(", control=");
  Serial.print(control, DEC);
  Serial.print(", value=");
  Serial.println(value, DEC);
  */
}
