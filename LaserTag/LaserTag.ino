#define USE_SOFT_CARRIER 1
#include <IRremote.h>


#define SAMSUNG_BITS 32
#define NEC_BITS 32

#define RECV_PIN 9
#define SEND_PIN 3
#define BTN_PIN 6

IRrecv irrecv(RECV_PIN);
IRsend irsend(SEND_PIN);
  
decode_results results;

unsigned long key_value = 0;
unsigned long timem, prevtimem = 0;

int health;
int ammo;
int maxhealth = 3;
int maxammo = 400;

int team = 0;

int currentval = 0;

/*
 * CH- - team = 1
 * CH - team = 0
 * CH+ - team = 2
 * <<| - health = maxhealth
 * >>| - ammo = maxammo
 * >| - health = 0
 * - - if team != 1 health--
 * + - if team != 2 health--
 * EQ - health--
 * 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 - append currentval
 * 100+ - maxhealth = currentval
 * 200+ maxammo = currentval
 */

void setup(){
  Serial.begin(115200);
  
  Serial.println("Enabling IRin");

  irrecv.enableIRIn();
  irrecv.blink13(true);

  pinMode(BTN_PIN, INPUT_PULLUP);

  health = maxhealth;
  ammo = maxammo;
}

void loop(){
  if (irrecv.decode(&results)){
 
        if (results.value == 0XFFFFFFFF)
          results.value = key_value;

        switch(results.value){
          case 0xFFA25D:
          Serial.println("CH-");
          team = 1;
          break;
          case 0xFF629D:
          Serial.println("CH");
          team = 0;
          break;
          case 0xFFE21D:
          Serial.println("CH+");
          team = 2;
          break;
          case 0xFF22DD:
          Serial.println("|<<");
          health = maxhealth;
          break;
          case 0xFF02FD:
          Serial.println(">>|");
          ammo = maxammo;
          break ;  
          case 0xFFC23D:
          Serial.println(">|");
          health = 0;
          break ;               
          case 0xFFE01F:
          Serial.println("-");
          if (team != 1) {health--;}
          break ;  
          case 0xFFA857:
          Serial.println("+");
          if (team != 2) {health--;}
          break ;  
          case 0xFF906F:
          Serial.println("EQ");
          health--;
          break ;  
          case 0xFF6897:
          Serial.println("0");
          //blinkTimes(LED_PIN, 0);
          currentval *= 10;
          currentval += 0;
          break ;  
          case 0xFF9867:
          Serial.println("100+");
          maxhealth = currentval;
          currentval = 0;
          break ;
          case 0xFFB04F:
          Serial.println("200+");
          maxammo = currentval;
          currentval = 0;
          break ;
          case 0xFF30CF:
          Serial.println("1");
          //blinkTimes(LED_PIN, 1);
          currentval *= 10;
          currentval += 1;
          break ;
          case 0xFF18E7:
          Serial.println("2");
          //blinkTimes(LED_PIN, 2);
          currentval *= 10;
          currentval += 2;
          
          break ;
          case 0xFF7A85:
          Serial.println("3");
          //blinkTimes(LED_PIN, 3);
          currentval *= 10;
          currentval += 3;
          
          break ;
          case 0xFF10EF:
          Serial.println("4");
          //blinkTimes(LED_PIN, 4);
          currentval *= 10;
          currentval += 4;
          
          break ;
          case 0xFF38C7:
          Serial.println("5");
          //blinkTimes(LED_PIN, 5);
          currentval *= 10;
          currentval += 5;
          
          break ;
          case 0xFF5AA5:
          Serial.println("6");
          //blinkTimes(LED_PIN, 6);
          currentval *= 10;
          currentval += 6;
          
          break ;
          case 0xFF42BD:
          Serial.println("7");
          //blinkTimes(LED_PIN, 7);
          currentval *= 10;
          currentval += 7;
          
          break ;
          case 0xFF4AB5:
          Serial.println("8");
          //blinkTimes(LED_PIN, 8);
          currentval *= 10;
          currentval += 8;
          
          break ;
          case 0xFF52AD:
          Serial.println("9");
          //blinkTimes(LED_PIN, 9);
          currentval *= 10;
          currentval += 9;
          
          break ;
          default:     
            Serial.println(results.value);     
        }
        irrecv.resume(); 
  }
  timem = millis();
  if ((digitalRead(BTN_PIN) == LOW) && (timem > prevtimem + 500) && (health > 0) && (ammo > 0))
  {
    switch(team)
    {
      case 1:
        irsend.sendNEC(0xFFE01F, NEC_BITS);
//        irsend.sendSAMSUNG(0xFFE01F, SAMSUNG_BITS);
      break ;
      case 2:
        irsend.sendNEC(0xFFA857, NEC_BITS);
//        irsend.sendSAMSUNG(0xFFA857, SAMSUNG_BITS);

      break ;
      default:
        /*Serial.println("pew pew");
        digitalWrite(SEND_PIN, HIGH);
        delay(2000);
        digitalWrite(SEND_PIN, LOW);*/
        Serial.println("pew");
        irsend.sendNEC(0xFF906F, NEC_BITS);
        Serial.println("sent pew");
//        irsend.sendSAMSUNG(0xFF906F, SAMSUNG_BITS);
    }
    irrecv.enableIRIn();
    ammo--;
    prevtimem = timem;
  }
}
