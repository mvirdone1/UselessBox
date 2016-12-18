#include <Servo.h> 

// Setup these directives for your particular hardware configuration
#define SWITCH_PIN 3
#define FINGER_PIN 9
#define DOOR_PIN 10
#define LED_PIN 13

// Depending on circuit design, the switch can be in the "ON" position as either high or low
#define SWITCH_ON_POLARITY LOW

// Define finger servo stops - This largely depends on the physical configuration
#define FINGER_REST 10
#define FINGER_PRE_RUN 90
#define FINGER_PUSH 180

#define DOOR_REST 80
#define DOOR_OPEN 180

Servo doorServo;
Servo fingerServo;

int volatile doorPos = DOOR_REST;
int volatile fingerPos = FINGER_REST;
int volatile movingForward = 0;
int volatile transitionToOff = 0;
int volatile led = 0;

int selectedMove = 0;             //move selector

void setup()
{


    // The interrupt pin is HIGH when the switch is in the "off" position (i.e. no action is required)
    // The interrupt pin is LOW when the switch is in the "on" position (i.e. it must be turned off)  
    pinMode(SWITCH_PIN, INPUT_PULLUP);

    // For debugging
    pinMode(LED_PIN, OUTPUT);

    // Setup the input pin with a change detection interrupt
    attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), myISR, CHANGE);

    // Servo setup
    doorServo.attach(DOOR_PIN);           //setup door servo
    fingerServo.attach(FINGER_PIN);          //setup finger servo
    doorServo.write(DOOR_REST);           //set door to hiding position 
    fingerServo.write(FINGER_REST);            //set finger to hiding position

    // A little pause before operation
    delay(100);
}

void moveDoor(int newPos, int step, int delayLength)
{
    if (newPos < doorPos)
    {

        for (doorPos = doorPos; doorPos >= newPos; doorPos -= step)
        {
            doorServo.write(doorPos);
            delay(delayLength);
        }
    }
    else
    {
        for (doorPos = doorPos; doorPos < newPos; doorPos += step)
        {
            doorServo.write(doorPos);
            delay(delayLength);
        }

    }


    
}

void moveFinger(int newPos, int step, int delayLength)
{
    // See if the arm is moving forwards or backwards
    if (newPos < fingerPos)
    { 
        // Backwards

        for (fingerPos = fingerPos; fingerPos >= newPos; fingerPos -= step)
        {
            fingerServo.write(fingerPos);
            delay(delayLength);
        }

        // Always go to the very last "rest" position
        fingerServo.write(newPos);
        delay(delayLength);

    } // End Backwards
    else
    { // Forewards 

        movingForward = 1;

        // Note that for forewards, there's an interrupt that will get set when the switch is returned. 
        // This means that we don't have to be precise with our hardware configuration, nor do we have the servo stalling pushing against a switch which won't move

        
        for (fingerPos = fingerPos; fingerPos < newPos; fingerPos += step)
        {
            fingerServo.write(fingerPos);

            // See if the switch got put back (via interrupt)
            if (transitionToOff == 1)
            {
                fingerServo.write(fingerPos-step);
                // Clear the interrupt flag
                transitionToOff = 0;
                movingForward = 0;
                break;
            }

            delay(delayLength);

            // See if the switch got put back (via interrupt)
            if (transitionToOff == 1)
            {
                fingerServo.write(fingerPos - step);
                // Clear the interrupt flag
                transitionToOff = 0;
                movingForward = 0;
                break;
            }
        } // End Forewards

    }
   
}

void loop() {

    //if the switch is on, move door and finger to switch it off	
    if (digitalRead(SWITCH_PIN) == SWITCH_ON_POLARITY)
    {

        simpleClose();

        /*
        if (selectedMove > 9) {
        selectedMove = 0;
        } //when all moves are played, repeat the moves from beginning



        if (selectedMove == 0) {
        simpleClose();
        }
        else if (selectedMove == 1) {
        simpleClose();
        }
        else if (selectedMove == 2) {
        simpleClose2();
        }
        else if (selectedMove == 3) {
        crazydoor();
        }
        else if (selectedMove == 4) {
        slow();
        }
        else if (selectedMove == 5) {
        serious();
        }
        else if (selectedMove == 6) {
        trollClose();
        }
        else if (selectedMove == 7) {
        simpleClose();
        }
        else if (selectedMove == 8) {
        matrix();
        }
        else if (selectedMove == 9) {
        sneak();
        }

       selectedMove += 1;         //swith to next move

       */

    }
}

// Moves

   // basic move 
void simpleClose()
{
    //Move(Position, Step Size, Delay)

    //Moving door
    moveDoor(85, 3, 15);
     // moveDoor(155, 3, 15);


    
    //Moving hand    
    moveFinger(FINGER_PUSH, 5, 10);

    /*
    fingerServo.write(180);
    while (transitionToOff == 0)
    {
        fingerServo.write(180);
    }
    
    transitionToOff = 0;

    fingerServo.write(0);
    delay(100);
    */

    //hiding hand
    moveFinger(FINGER_REST, 4, 50);

    //hiding door
    moveDoor(80, 3, 15);

}

/*
// open and wait, then move finger and wait, then switch of and hide
 void simpleClose2()
{
//Moving door
  for(pos = 80; pos < 155; pos += 3)
  {
  doorServo.write(pos);
  delay(15);
  }
  delay(800);
  //Moving hand
  for(pos = 0; pos < 100; pos += 4)
  {
  fingerServo.write(pos);
  delay(15);
  }
  delay(1000);
  for(pos = 100; pos < 129; pos += 4)
  {
  fingerServo.write(pos);
  delay(15);
  }

  //hiding hand
  for(pos = 129; pos>=0; pos-=5)
  {
  fingerServo.write(pos);
  delay(15);
  }

  //hiding door
  for(pos = 155; pos>=80; pos-=3)
  {
  doorServo.write(pos);
  delay(15);


 }

 }

 //open door then close it many times, wait, then quickly reopen, switch off and hide.

 void crazydoor()
{

 //Moving door
  for(pos = 80; pos < 125; pos += 3)
  {
  doorServo.write(pos);
  delay(15);
  }

  //hiding door
  for(pos = 125; pos>=80; pos-=5)
  {
  doorServo.write(pos);
  delay(15);
  }
 //Moving door
  for(pos = 80; pos < 110; pos += 3)
  {
  doorServo.write(pos);
  delay(15);
  }

  //hiding door
  for(pos = 110; pos>=80; pos-=15)
  {
  doorServo.write(pos);
  delay(15);
  }
  delay(700);
 //Moving door
  for(pos = 80; pos < 125; pos += 3)
  {
  doorServo.write(pos);
  delay(15);
  }
  delay(700);
  //hiding door
  for(pos = 125; pos>=80; pos-=5)
  {
  doorServo.write(pos);
  delay(15);
  }

//Moving door
  for(pos = 80; pos < 155; pos += 8)
  {
  doorServo.write(pos);
  delay(15);
  }

  //Moving hand
  for(pos = 0; pos < 129; pos += 3)
  {
  fingerServo.write(pos);
  delay(15);
  }

  //hiding hand
  for(pos = 129; pos>=0; pos-=3)
  {
  fingerServo.write(pos);
  delay(15);
  }

  //hiding door
  for(pos = 155; pos>=80; pos-=15)
  {
  doorServo.write(pos);
  delay(15);
  }

}


//open door,move finger very slowly forward and back to hiding very slowly, then quickly close door
void slow()
{

//Moving door
    for(pos = 80; pos < 155; pos += 1)
    {
    doorServo.write(pos);
    delay(30);
    }

    //Moving hand
    for(pos = 0; pos < 129; pos += 1)
    {
    fingerServo.write(pos);
    delay(30);
    }

    //hiding hand
    for(pos = 129; pos>=0; pos-=1)
    {
    fingerServo.write(pos);
    delay(30);
    }

    //hiding door
    for(pos = 155; pos>=125; pos-=1)
    {
    doorServo.write(pos);
    delay(30);
    }
    delay(100);
    for(pos = 125; pos>=80; pos-=4)
    {
    doorServo.write(pos);
    delay(15);
    }


 }

 //serious

 void serious() {

//Moving door
    for(pos = 80; pos < 155; pos += 3)
    {
    doorServo.write(pos);
    delay(15);
    }

    //Moving hand
    for(pos = 0; pos < 70; pos += 1)
    {
    fingerServo.write(pos);
    delay(15);
    }
    delay(800);


    //hiding door
    for(pos = 155; pos>=130; pos-=3)
    {
    doorServo.write(pos);
    delay(15);
    }

    //hiding door
    for(pos = 130; pos < 155; pos+=3)
    {
    doorServo.write(pos);
    delay(15);
    }
     //hiding door
    for(pos = 155; pos>=130; pos-=3)
    {
    doorServo.write(pos);
    delay(15);
    }
    //hiding door
    for(pos = 130; pos < 155; pos+=3)
    {
    doorServo.write(pos);
    delay(15);
    }

    fingerServo.write(40);
    delay(1000);

    //Moving hand
    for(pos = 40; pos < 129; pos += 4)
    {
    fingerServo.write(pos);
    delay(15);
    }

    //hiding hand
    for(pos = 129; pos>=0; pos-=4)
    {
    fingerServo.write(pos);
    delay(15);
    }


    for(pos = 120; pos>=80; pos -= 1)
    {
    doorServo.write(pos);
    delay(15);
    }



}

void trollClose(){
//Moving door
    for(pos = 80; pos < 155; pos += 3)
    {
    doorServo.write(pos);
    delay(15);
    }

    //Moving hand
    for(pos = 0; pos < 127; pos += 4) 
    {
    fingerServo.write(pos);
    delay(15);
    }
    //hiding door
    for(pos = 155; pos>=130; pos-=3)
    {
    doorServo.write(pos);
    delay(15);
    }
    delay(2000);

    for(pos = 130; pos < 155; pos += 3)
    {
    doorServo.write(pos);
    delay(15);
    }

    for(pos = 155; pos>=140; pos-=3)
    {
    doorServo.write(pos);
    delay(15);
    }
    for(pos = 140; pos < 155; pos += 3)
    {
    doorServo.write(pos);
    delay(15);
    }
    delay(500);
    //hiding hand
    for(pos = 127; pos>=0; pos-=4)
    {
    fingerServo.write(pos);
    delay(15);
    }

    //hiding door
    for(pos = 155; pos>=80; pos-=3)
    {
    doorServo.write(pos);
    delay(15);
    }

}

void matrix()
{

 //Moving door
    for(pos = 80; pos < 155; pos += 3)
    {
    doorServo.write(pos);
    delay(15);
    }

    //Moving hand
    for(pos = 0; pos < 80; pos += 4)
    {
    fingerServo.write(pos);
    delay(15);
    }

    for(pos = 80; pos < 129; pos += 1)
    {
    fingerServo.write(pos);
    delay(30);
    }
    delay(300);

    for(pos = 129; pos>=0; pos-=4)
    {
    fingerServo.write(pos);
    delay(10);
    }

    //hiding door
    for(pos = 155; pos>=80; pos-=3)
    {
    doorServo.write(pos);
    delay(15);
    }

}

void sneak()
   {
   //Moving door
    for(pos = 80; pos < 130; pos += 1)
    {
    doorServo.write(pos);
    delay(30);
    }
    delay(2000);

    //Moving hand
    for(pos = 0; pos < 40; pos += 1)
    {
    fingerServo.write(pos);
    delay(30);
    }

    delay(500);

    for(pos = 130; pos < 155; pos += 4)
    {
    doorServo.write(pos);
    delay(15);
    }
    delay(100);

    for(pos = 40; pos < 90; pos += 4)
    {
    fingerServo.write(pos);
    delay(15);
    }
    delay(500);
    //hiding hand
    for(pos = 90; pos>=70; pos-=4)
    {
    fingerServo.write(pos);
    delay(15);
    }
    delay(100);
    for(pos = 70; pos < 90; pos += 4)
    {

    fingerServo.write(pos);
    delay(15);
    }
    delay(100);
    for(pos = 90; pos>=70; pos-=4)
    {
    fingerServo.write(pos);
    delay(15);
    }
    delay(100);

    for(pos = 70; pos < 129; pos += 4)
    {

    fingerServo.write(pos);
    delay(15);
    }

    for(pos = 129; pos>=0; pos-=4)
    {
    fingerServo.write(pos);
    delay(15);
    }
    //hiding door
    for(pos = 155; pos>=80; pos-=3)
    {
    doorServo.write(pos);
    delay(15);
    }
   }
   */


void myISR()
{
    // Note that this ISR will only fire on transitions, so this should indicate that the switch has been changed back to its original position
    if ((digitalRead(SWITCH_PIN) != SWITCH_ON_POLARITY) && (movingForward == 1))
    {
        // Pull the servo off the switch instantly        
        transitionToOff = 1;

        if (led == 0)
        {
            led = 1;
            digitalWrite(LED_PIN, HIGH);
        }
        else
        {
            led = 0;
            digitalWrite(LED_PIN, LOW);
        }

        
    }


}

