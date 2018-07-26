/*Arduino Code for measuring speed of the Vechile using Hall Sensor
 * Coded by Circuitdigest.com
 * On 14-04-2017
 */

 /*CONNECTION DETIALS
  * Arduino D11 -> RX of BT Module
  * Arduino D12 -> Tx of BT
  * Arduino D2  -> Hall sensor 3rd pin
  */

//#include <SoftwareSerial.h>// import the serial library

//SoftwareSerial Cycle_BT(11, 12);  // RX, TX
int ledpin=12; // led on D13 will show blink on / off
//int BluetoothData; // the data given from Computer


float radius_of_wheel = 1.9;  //Measure the radius of your wheel and enter it here
volatile byte rotation; // variale for interrupt fun must be volatile
float timetaken,rpm,dtime;
int v;
unsigned long pevtime;

 
void setup()
 {
  Serial.begin(9600);
   //Cycle_BT.begin(9600); //start the Bluetooth communication at 9600 baudrate
   pinMode(ledpin,OUTPUT); //LED pin aoutput for debugging
   //pinMode(2, INPUT_PULLUP);
   //attachInterrupt(digitalPinToInterrupt(2), magnet_detect, RISING);
   attachInterrupt(0, magnet_detect, RISING); //secound pin of arduino used as interrupt and magnet_detect will be called for each interrupt
   rotation = rpm = pevtime = 0; //Initialize all variable to zero
 }
 
void loop()
{
  /*To drop to zero if vehicle stopped*/
 if(millis()-dtime>2000) //no magnet found for 1500ms
 {
  rpm= v = 0; // make rpm and velocity as zero
  Serial.write(v);
  dtime=millis();
  Serial.print("stopped""\f""0");
  //Serial.print("\r");
  digitalWrite(ledpin, LOW);

 }
 v = radius_of_wheel * rpm ; //0.33 is the radius of the wheel in meter
 //v = radius_of_wheel * rpm * 0.37699; //0.33 is the radius of the wheel in meter
}
 
void magnet_detect() //Called whenever a magnet is detected
{
  rotation++;
  dtime=millis();
  if(rotation>=2)
  {
    timetaken = millis()-pevtime; //time in millisec for two rotations
    rpm=(1000/timetaken)*60;    //formulae to calculate rpm
    pevtime = millis();
    digitalWrite(ledpin, HIGH);
    //rotation=0;
    //if (rotation>=2){
    //digitalWrite(ledpin, HIGH);
 
  }
  
    //Cycle_BT.write(v);
    //Cycle_BT.println("Magnet detected...."); //enable while testing the hardware
    //Serial.println("Magnet detected...."); //enable while testing the hardware
    Serial.print("Speed ");
    Serial.println(v);
    
  }

