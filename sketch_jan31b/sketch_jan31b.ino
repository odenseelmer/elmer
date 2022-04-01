/* Level Crossing With Two Gates
By Gary Granai steamtraininfo.com 2020  (This is a modified version of Version 1.4 Geoff Bunza 2018 )
*/
#include <Servo.h>
Servo gate1servo; // create servo object to control crossing gate 1
//ASSUMPTION Gate is down at position 30 and up at position 120
Servo gate2servo; // create servo object to control crossing gate 2
//ASSUMPTION Gate is down at position 30 and up at position 120
int gate1flashers = 2; /*connect 3 Leds to the gate flasher pins.  be sure that the total current drawn by the Leds is less than the 40 ma Arduino pin load limit. If you don't want gate flashing lights, there is no need to change the code. Just don't add the lights.*/
int gate2flashers = 3;
int alarmbells = 4; /* This is an optional connection to an alarm bell module.  If you don't want the bells, there is no need to change the code. Just don't add the the module.*/
int sensor1 = 5; // IR sensor pin Assumption== Pin goes LOW when train detected
int sensor2 = 6; // IR sensor pin Assumption== Pin goes LOW when train detected
int led1 = 10; // Led 1 pin first alternating flasher
int led2 = 11; // Led 2 pin first alternating flasher
int led3 = 12; // Led 3 pin second alternating flasher
int led4 = 13; // Led 4 pin second alternating flasher
int gatelow = 30; // variable to store the servo low gate stop position
int gatehigh = 120; // variable to store the servo high gate stop position
int gateposition = 120; // variable to store the servo gateposition
int entering_sensor = 5; //this will tell which sensor was triggered first
int leaving_sensor = 6; // this will tell which sensor shows train leaving
int gates_started = 0; // this says if the crossing is active
int flash_state = 0;
long flash_time = 0;
long flash_interval = 900; // time in milliseconds between alternating flashes
int sensor_count = 0;
void setup()
{
gate1servo.attach(3); // attaches the servo on pin 3 to the servo object
gate2servo.attach(4); // attaches the servo on pin 4 to the servo object
gate1servo.write(gateposition); //start assuming no train
gate2servo.write(gateposition); //start assuming no train
pinMode(sensor1, INPUT);
pinMode(sensor2, INPUT);
pinMode(led1, OUTPUT);
pinMode(led2, OUTPUT);
pinMode(led3, OUTPUT);
pinMode(led4, OUTPUT);
digitalWrite(led1, LOW); // Start with all flashers off
digitalWrite(led2, LOW);
digitalWrite(led3, LOW);
digitalWrite(gateflashers1, LOW);
digitalWrite(gateflashers2, LOW);
digitalWrite(alarmbells, LOW);

flash_time = millis();
}
void loop()
{
if ((digitalRead (sensor1)==LOW)&& (gates_started==0)) {
gates_started = 1;
leaving_sensor = sensor2;
starting_sequence();
}
if ((digitalRead (sensor2)==LOW)&& (gates_started==0)) {
gates_started = 1;
leaving_sensor = sensor1;
starting_sequence();
}
if (gates_started) flash_leds(); //gates are down continue flashing
if ((digitalRead(leaving_sensor)==LOW)&&(gates_started==1)) { //train is starting to leave

//as long as the leaving sensor is active the train is still in the crossing
while (gates_started==1) { //now check if train is REALLY gone
sensor_count = 0;
for (int i=1; i<40; i++) {
if (digitalRead(leaving_sensor)==LOW) sensor_count++;
delay (30);
flash_leds();
}
if (sensor_count==0) gates_started=0;
flash_leds();
}
// we only get here if the train has really left the crossing
ending_sequence();
}
}
void starting_sequence() {
long wait_time;
flash_time = millis();
wait_time = millis()+3000;
while (wait_time > millis()) flash_leds(); //flash before dropping gates
for(gateposition = gatehigh; gateposition>gatelow; gateposition-=1) // goes from gatehigh degrees to gatelow degrees
{
gate1servo.write(gateposition); // tell servo to go to gateposition in variable ‘gateposition’
gate2servo.write(gateposition); // tell servo to go to gateposition in variable ‘gateposition’
flash_leds(); // keep flashing leds
delay(40); // waits 40ms to slow servo
}
}
void ending_sequence() {
for(gateposition = gatelow; gateposition<gatehigh; gateposition++) // goes from gatelow degrees to gatehigh degrees
{
gate1servo.write(gateposition); // tell servo to go to gateposition in variable ‘gateposition’
gate2servo.write(gateposition); // tell servo to go to gateposition in variable ‘gateposition’
flash_leds(); // keep flashing leds
delay(40); // waits 40ms to slow servo
}
digitalWrite(led1, LOW); // flashers completely off
digitalWrite(led3, LOW);
digitalWrite(led2, LOW);
digitalWrite(led4, LOW);
digitalWrite(gateflashers1, LOW);
digitalWrite(gateflashers2, LOW);
digitalWrite(alarmbells, LOW);
delay(30000); // 30 second delay to account for the train passing the starting entry sensor
}
void flash_leds() {
if (flash_time > millis()) return;
flash_state = ~flash_state;
digitalWrite(led1, flash_state); // Alternate flashers
digitalWrite(led3, flash_state);
digitalWrite(led2, ~flash_state);
digitalWrite(led4, ~flash_state);
flash_time = millis()+flash_interval;
}
