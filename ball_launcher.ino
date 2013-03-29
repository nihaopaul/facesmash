//h-bridge shield 
int m1[] = {
  5,6};
int m2[] = {
  9,10};

#include <Servo.h> 
Servo firepin;

#include <avr/sleep.h>
int wakePin = 2;                 // pin used for waking up
int sleepStatus = 0;             // variable to store a request for sleep
int count = 0;  

void setup() {
  // put your setup code here, to run once:
  pinMode(m1[0], OUTPUT); 
  pinMode(m1[1], OUTPUT);
  pinMode(m2[0], OUTPUT); 
  pinMode(m2[1], OUTPUT);
  firepin.attach(8);
  firepin.write(90);
  pinMode(wakePin, INPUT);
  attachInterrupt(0, wakeUpNow, HIGH); // use interrupt 0 (pin 2) and run function
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly: 
  Serial.print("Awake for ");
  Serial.print(count);
  Serial.println("sec");
  Serial.print("Mode: \t");
  Serial.println(digitalRead(2));
  count++;
  delay(1000); 
  if (digitalRead(2) == HIGH) {
    to_sleep();
  }
  if (count == 2) {
    Serial.println("starting motors");
    fire_start();
  }
  if (count ==10) {
    Serial.println("BALLS EYE!");
    fire();
    to_sleep();
  }
}
void to_sleep() {
    fire_stop();
    Serial.println("Timer: Entering Sleep mode");
    delay(100);     // this delay is needed, the sleep
    //function will provoke a Serial error otherwise!!
    count = 0;
    sleepNow();     // sleep function called here
}
void wakeUpNow()        // here the interrupt is handled after wakeup
{
  // execute code here after wake-up before returning to the loop() function
  // timers and code using timers (serial.print and more...) will not work here.
  // we don't really need to execute any special functions here, since we
  // just want the thing to wake up
}


void sleepNow()         // here we put the arduino to sleep
{
  /* Now is the time to set the sleep mode. In the Atmega8 datasheet
   * http://www.atmel.com/dyn/resources/prod_documents/doc2486.pdf on page 35
   * there is a list of sleep modes which explains which clocks and
   * wake up sources are available in which sleep mode.
   *
   * In the avr/sleep.h file, the call names of these sleep modes are to be found:
   *
   * The 5 different modes are:
   *     SLEEP_MODE_IDLE         -the least power savings
   *     SLEEP_MODE_ADC
   *     SLEEP_MODE_PWR_SAVE
   *     SLEEP_MODE_STANDBY
   *     SLEEP_MODE_PWR_DOWN     -the most power savings
   *
   * For now, we want as much power savings as possible, so we
   * choose the according
   * sleep mode: SLEEP_MODE_PWR_DOWN
   *
   */
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here

  sleep_enable();          // enables the sleep bit in the mcucr register
  // so sleep is possible. just a safety pin

  /* Now it is time to enable an interrupt. We do it here so an
   * accidentally pushed interrupt button doesn't interrupt
   * our running program. if you want to be able to run
   * interrupt code besides the sleep function, place it in
   * setup() for example.
   *
   * In the function call attachInterrupt(A, B, C)
   * A   can be either 0 or 1 for interrupts on pin 2 or 3.  
   *
   * B   Name of a function you want to execute at interrupt for A.
   *
   * C   Trigger mode of the interrupt pin. can be:
   *             LOW        a low level triggers
   *             CHANGE     a change in level triggers
   *             RISING     a rising edge of a level triggers
   *             FALLING    a falling edge of a level triggers
   *
   * In all but the IDLE sleep modes only LOW can be used.
   */

  attachInterrupt(0,wakeUpNow, HIGH); // use interrupt 0 (pin 2) and run function
  // wakeUpNow when pin 2 gets LOW

  sleep_mode();            // here the device is actually put to sleep!!
  // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP

  sleep_disable();         // first thing after waking from sleep:
  // disable sleep...
  detachInterrupt(0);      // disables interrupt 0 on pin 2 so the
  // wakeUpNow code will not be executed
  // during normal running time.

}
void fire_start() {
  digitalWrite(m1[0],HIGH); 
  digitalWrite(m1[1],LOW); 
  digitalWrite(m2[0],LOW); 
  digitalWrite(m2[1],HIGH); 
}

void fire_stop() {
  digitalWrite(m1[0],LOW); 
  digitalWrite(m1[1],LOW); 
  digitalWrite(m2[0],LOW); 
  digitalWrite(m2[1],LOW); 
}

void fire() {
  firepin.write(150);
  delay(250);
  firepin.write(90);
}


