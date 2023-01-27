#include <IRremote.h>

//IR-Remote
#define NO1 0xFFA25D // 1
#define NO2 0xFF629D // 2 
#define NO3 0xFFE21D // 3 
#define NO4 0xFF22DD // 4
#define NO5 0xFF02FD // 5 
#define NO6 0xFFC23D // 6 
#define NO7 0xFFE01F // 7
#define NO8 0xFFA857 // 8
#define NO9 0xFF906F // 9
#define NO0 0xFF9867 // 0
#define STAR 0xFF6897 // * 
#define HASH 0xFFB04F // #
#define ARRDOWN 0xFF4AB5 // Arrow down
#define ARRUP 0xFF18E7 // Arrow up
#define ARRLEFT 0xFF10EF // Arrow left
#define ARRRIGHT 0xFF5AA5 // Arrow right
#define OK 0xFF38C7 // OK

int RECV_PIN = 2; // define input pin on Arduino 
int CH1_PIN = 3;
int CH2_PIN = 4;
int CH3_PIN = 5;
int CH4_PIN = 6;
int CH5_PIN = 7;
int CH6_PIN = 8;
int BLACK_PIN = 9;
int AUTO_PIN = 10;
int BANK_DOWN_PIN = 11;
int BANK_UP_PIN = 12;
int TAB_PIN = 13;
int MUSIC_PIN = 15;

//"CHASE1", "CHASE2", "CHASE3", "CHASE4", "CHASE5", "CHASE6"
bool Chases [] = {false, false, false,false, false, false};
int ChasesPIN [] = {CH1_PIN, CH2_PIN, CH3_PIN, CH4_PIN, CH5_PIN, CH6_PIN};
int NumberOfChases = 6;
//"BLACKOUT", "AUTO"
bool Modes [] = {false, false};
int ModesPIN [] = {9, 10};

bool LastChases [6];
bool STROBE = false;

IRrecv irrecv(RECV_PIN); 
decode_results results; // decode_results class is defined in IRremote.h

void pressButton(int pin)
{
    digitalWrite(pin, HIGH);
    delay(50);
    digitalWrite(pin, LOW);
    delay(50);
    
    if (pin < 9){
       Chases[pin-3] = !Chases[pin-3];  //save changes in memory-Array
    }
    else{
       Modes[pin-9] = !Modes[pin-9];
    }
    
    String out = "Button " + String(pin) + " pressed"; //remove for 
    Serial.println(out);                               //production
}

void pressTab()
{
    digitalWrite(TAB_PIN, HIGH);
    delay(40);
    digitalWrite(TAB_PIN, LOW);
    delay(10);
}

void tab(int time)
{
  if (time < 20){
      time = 20;
  }
  else
  {
    time -= 20;
  }
  pressTab();
  delay(time);
  pressTab();  
}


void reactivateLastChases(){
  for (int i = 0; i < NumberOfChases; ++i){
    if (LastChases[i] != Chases[i]){
      pressButton(ChasesPIN[i]);
    }
  }
}
void deactivateAllChases(){
  for (int i = 0; i < NumberOfChases; ++i){
    if (Chases[i] == true){
      pressButton(ChasesPIN[i]);
    }
  }
}

void switchStrobe()
{
  if (!STROBE){;
    for (int i = 0; i < NumberOfChases; ++i)
    {
      LastChases[i] = Chases[i];;
    }
    deactivateAllChases();
    pressButton(CH6_PIN); 
  }
  else{
    reactivateLastChases();
  }
  STROBE = !STROBE;
}

void setup() {
  Serial.begin(9600);
  Serial.println("Start");
  pinMode(CH1_PIN, OUTPUT);
  pinMode(CH2_PIN, OUTPUT);
  pinMode(CH3_PIN, OUTPUT);
  pinMode(CH4_PIN, OUTPUT);
  pinMode(CH5_PIN, OUTPUT);
  pinMode(CH6_PIN, OUTPUT);
  pinMode(BLACK_PIN, OUTPUT);
  pinMode(AUTO_PIN, OUTPUT);
  pinMode(BANK_DOWN_PIN, OUTPUT);
  pinMode(BANK_UP_PIN, OUTPUT);
  pinMode(TAB_PIN,OUTPUT);
  pinMode(MUSIC_PIN,OUTPUT);
  
  irrecv.enableIRIn(); // Start the receiver 

  delay(2000);
  pressButton(AUTO_PIN);
  delay(50);
  pressButton(CH2_PIN);
  delay(50);
  
  Serial.println("End setup"); // remove 
}

void PhysicalRemote(uint32_t result)
{
  switch(result)
    {
      case NO1:
        pressButton(CH1_PIN);
      break;
      case NO2:
        pressButton(CH2_PIN);
      break;
      case NO3:
        pressButton(CH3_PIN);
      break;
      case NO4:
        pressButton(CH4_PIN);
      break;
      case NO5:
        pressButton(CH5_PIN);
      break;
      case NO6:
        pressButton(CH6_PIN);
      break;
      case NO7:
        tab(800);
      break;
      case NO8:
        tab(400);
      break;
      case NO9:
        tab(80); //actually 100ms, but because of precision problems 80ms is more reliable to get a 100ms tab-distance
      break;
      case STAR:
        switchStrobe();
      break;
      case NO0:
        pressButton(BLACK_PIN);
      break;
      case HASH:
        //nothing
      break;
      case ARRDOWN:
        pressButton(BANK_DOWN_PIN);
      break;
      case ARRUP:
        pressButton(BANK_UP_PIN);
      break;
      case ARRLEFT:
        pressButton(MUSIC_PIN);
      break;
      case ARRRIGHT:
        pressButton(AUTO_PIN);
      break;
      case OK:
        pressTab();      
      break;
    }
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    
    PhysicalRemote(results.value);
    
    irrecv.resume(); // Receive the next value
  }
}
