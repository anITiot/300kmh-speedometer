# 300kmh-speedometer
How to make a speedometer for a motorbike
I found some code on https://circuitdigest.com/microcontroller-projects/diy-speedometer-using-arduino-and-processing-android-app
and that was my start for this project, I'm a newbie in programming and electronics even if I had been in the IT-world for while (at least 35 year now).......
The goal with this is to have a working easy read speedometer smallsize on the top of the fairing, I noticed that I use 2 seconds when I looked at the original speedometer lowering the head and focus on the meter, thats 50m or more travelling and I want to reduce that time for security reasons.
Next step is to add 4x7bit led display and a reliable digital hall sensor.
Built a testbench and found use of f...ing fidgetspinners :) 
Added 2 hallsensors, the left for rpm and the right for speed and a 8x7bit led display but I use for 2x4x7bit.
Rebuild most of the code but it's just now in a 0.0000001b version

Sorry for the swedish words in the code but thats my native language

code:
/*Arduino code for measuring speed and engine rpm using two Hall sensors
 *
 * Coding of anITiot.se summer 2018
 * LED display
 * inspirated code by Circuitdigest.com On 14-04-2017
 */



 /*CONNECTION DETIALS
  * LED display cs 10,clk 11,din 12
  * Arduino D2  -> Hall sensor 3rd pin
  */


#include <LedControl.h>  //7x8LEDdisplay

int ledpin=13; // led on D13 will show blink on / off



float circum = 0.05;  //Measure the circumferens of your wheel and enter it here
volatile byte rotation; // variale for interrupt fun must be volatile
volatile byte engrotation; // variale for interrupt fun must be volatile

float timetaken,rpm,dtime,rps,engrpm,engrps,timetaken1,engdtime;

int v;
int r;
//int DisplayNumber;
//int DisplayString;
//int printNumber;
unsigned long pevtime;
unsigned long engpevtime;

LedControl lc=LedControl(12,11,10,1); //Ettan betyder att vi har en Display i kaskaden.

unsigned long c = 0;  //NC Norwegian creation
int period = 1000;    //NC
unsigned long time_now = 0;
unsigned long delaytime=333; 
void setup()
 {
  Serial.begin(9600);
   pinMode(ledpin,OUTPUT); //LED pin aoutput for debugging
   
   attachInterrupt(0, magnet0_detect, RISING); //secound pin of arduino used as interrupt and magnet_detect will be called for each interrupt
   rotation = rpm = pevtime = rps = 0; //Initialize all variable to zero

   attachInterrupt(1, magnet1_detect, RISING); //secound pin of arduino used as interrupt and magnet_detect will be called for each interrupt
   engrotation = engrpm = engpevtime = engrps = 0; //Initialize all variable to zero
   
   //Initiera LED-Displayen   
   lc.shutdown(0,false);
   lc.setIntensity(0,10);
   lc.clearDisplay(0);
 }
 
void loop()
{
  /*To drop to zero if vehicle stopped*/
 if(millis()-dtime>2000) //no magnet found for 1500ms
 {
  rpm= v = r = rps = c = 0; // make rpm and velocity as zero
  Serial.write(v);
  dtime=millis();
  //Serial.print("stopped""\f""0");
  Serial.println(v);
  digitalWrite(ledpin, LOW);
  DisplayString("HEL0");
  //DisplayNumber(c);

  
   //printNumber(c);
 }

 if(millis()-engdtime>2000) //no magnet found for 1500ms
 {
  engrpm = engrps = 0; // make rpm and velocity as zero
  Serial.write("engrpm0");
  engdtime=millis();
  //Serial.print("stopped""\f""0");
  //Serial.println(v);
  //digitalWrite(ledpin, LOW);
  //DisplayString("HEL0");
  //DisplayNumber(c);

  
  printNumber(engrpm);
 }
 c = v = circum * rpm * 3.6; //0.33 is the circumferens of the wheel in meter
 
}

void magnet0_detect() //Called whenever a magnet is detected

{
  rotation++;
  dtime=millis();
  if(rotation>=2)
  {
    timetaken = millis()-pevtime; //time in millisec for two rotations
    rpm = r =(1000/timetaken)*60;    //formulae to calculate rpm
    rps =(1000/timetaken);
    pevtime = millis();
    //digitalWrite(ledpin, HIGH);
   

    
  }
    //Serial.print("Speed ");
    //Serial.println(v);
    //Serial.print("rpm ");
    //Serial.println(rpm);
    //Serial.print("rps ");
    Serial.println(rps);
    //printNumber(v/3);
    
    DisplayNumber(c/3);
    
    
}

void magnet1_detect() //Called whenever a magnet is detected

{
  engrotation++;
  engdtime=millis();
  if(engrotation>=2)
  {
    timetaken1 = millis()-engpevtime; //time in millisec for two rotations
    engrpm =(1000/timetaken1)*60;    //formulae to calculate rpm
    engrps =(1000/timetaken1);
    engpevtime = millis();
    //digitalWrite(ledpin, HIGH);
   
  }
  printNumber(engrps);
  }

//Skriver ut ett tal.
void DisplayNumber(unsigned long number){
  if(number > 99999999){
    DisplayString("E9"); //Betyder Error 9, talet är för högt
    return;
  }
  String s = String(number);
  
  DisplayString(s);
}


void DisplayString(String s)
{
  int l = s.length();
  if(l > 8){
    s = "E8"; //Betyder Error 8, strängen är för lång
    l = s.length();
   
  }
  //lc.setChar(0, 7, getChar(s, l-8), false);
  //lc.setChar(0, 6, getChar(s, l-7), false);
  //lc.setChar(0, 5, getChar(s, l-6), false);
  //lc.setChar(0, 4, getChar(s, l-5), false);
  lc.setChar(0, 3, getChar(s, l-4), false);
  lc.setChar(0, 2, getChar(s, l-3), false);
  lc.setChar(0, 1, getChar(s, l-2), false);
  lc.setChar(0, 0, getChar(s, l-1), false);
  //delay(delaytime);
  //if(millis() > time_now + period){ //NC
  //      time_now = millis();        //NC
}

//Denna returnerar vilket tecken som är på platsen i strängen.
//Är platsen utanför strängen returneras ett blanksteg.
char getChar(String s, int i){
  char rc = ' ';
  if(s.length() > i ){
    rc = s.charAt(i);
  }
  return rc;
  
}
void printNumber(int r) {  
    int ones;  
    int tens;  
    int hundreds; 
    int tusen;
    int tiotusen;
    boolean negative=false;

    if(r < -99999 || r > 99999)  
        return;  
    if(r<0) {  
        negative=true; 
        r=r*-1;  
    }
    ones=r%10;  
    r=r/10;  
    tens=r%10;  
    r=r/10; 
    hundreds=r%10;  
    r=r/10;
    tusen=r%10;  
    r=r/10;
    tiotusen=r;  
    //if(negative) {  
        //print character '-' in the leftmost column  
        //lc.setChar(0,4,'-',false);  } 
    //else {
        //print a blank in the sign column  
        //lc.setChar(0,4,' ',false);
        //lc.setChar(0,4,' ',false);
        //lc.setChar(0,3,' ',false);  
    //}  
    //Now print the number digit by digit
    //lc.setDigit(0,4,(byte)tiotusen,false);
    //lc.setDigit(0,3,(byte)tusen,false); 
    lc.setDigit(0,7,(byte)hundreds,false);
    lc.setDigit(0,6,(byte)tens,false); 
    lc.setDigit(0,5,(byte)ones,false);
    //lc.setDigit(0,4,(byte)ones,false); 
    //delay(delaytime);
    
}    

