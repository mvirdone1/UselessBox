#include <Servo.h> 

// Setup these directives for your particular hardware configuration
#define SWITCH_PIN 3
#define FINGER_PIN 9
#define DOOR_PIN 10
#define LED_PIN 13

// Depending on circuit design, the switch can be in the "ON" position as either high or low
#define SWITCH_ON_POLARITY LOW

// Define finger servo stops - This largely depends on the physical configuration
#define FINGER_REST 0
#define FINGER_TAP 7
#define FINGER_PUSH 126

#define DOOR_REST 16
#define DOOR_OPEN 82

Servo doorServo;
Servo fingerServo;

int volatile doorPos = DOOR_REST;
int volatile fingerPos = FINGER_REST;


int selectedMove = 0;             //move selector

void setup()
{

    // For my hardware configuration:
    // The switch pin is HIGH when the switch is in the "off" position (i.e. no action is required)
    // The switch pin is LOW when the switch is in the "on" position (i.e. it must be turned off)  
    pinMode(SWITCH_PIN, INPUT_PULLUP);

    // For debugging
    pinMode(LED_PIN, OUTPUT);


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

    doorServo.write(newPos);
    delay(delayLength);
    
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
       
    } // End Backwards
    else
    { // Forewards 
       
        for (fingerPos = fingerPos; fingerPos < newPos; fingerPos += step)
        {
            fingerServo.write(fingerPos);
            delay(delayLength);

        } // End Forewards

    }

    fingerServo.write(newPos);
    delay(delayLength);
   
}

void loop() {

    //if the switch is on, move door and finger to switch it off	
    if (digitalRead(SWITCH_PIN) == SWITCH_ON_POLARITY)
    {

        // simpleClose();

        
        if (selectedMove > 10) 
        {
        selectedMove = 0;
        } //when all moves are played, repeat the moves from beginning


        switch (selectedMove)
        {
        case 0:
        case 1:            
            simpleClose();
            break;
        case 2:
            simpleClose2();
            break;
        case 3:
            crazydoor();
            break;
        case 4:
            slow();
            break;
        case 5:
            twobits();
            break;
        case 6:
            trollClose();
            break;
        case 7:
            sneak();
            break;
        case 8:
            serious();
            break;
        case 9:
            heart();
            break;
        case 10:
            runPullHit();
            break;

        }

       selectedMove += 1;         //swith to next move

       

    }
}

// Moves

   // basic move 
void simpleClose()
{
    //move(Position, Step Size, Delay)

    //Moving door
    moveDoor(DOOR_OPEN, 3, 15);
     // moveDoor(155, 3, 15);
    
    //Moving finger    
    moveFinger(FINGER_PUSH, 5, 15);
        

    //hiding finger
    moveFinger(FINGER_REST, 5, 15);

    //hiding door
    moveDoor(DOOR_REST, 3, 15);

}


// open and wait, then move finger and wait, then switch of and hide
 void simpleClose2()
{
     
    //Moving door
    moveDoor(DOOR_OPEN, 10, 15);

    // Pause
    delay(800);

    //Move close
    moveFinger(FINGER_PUSH - 30, 10, 15);
  
    // Pause again
    delay(1000);
    
    // Flip Switch
    moveFinger(FINGER_PUSH, 5, 15);

    // Hide finger
    moveFinger(FINGER_REST, 3, 15);
  
    // Close door
    moveDoor(DOOR_REST, 3, 15);
  
 }

 //open door then close it many times, wait, then quickly reopen, switch off and hide.

 void crazydoor()
{
     // Open close rapidly
     for (int i = 0; i < 5; i++)
     {
         moveDoor(DOOR_OPEN - 30, 10, 15);
         moveDoor(DOOR_REST, 10, 15);
     }

     // Sleep for a little while to build the suspense
    delay(700);

    // Open close rapidly
    for (int i = 0; i < 5; i++)
    {
        moveDoor(DOOR_OPEN - 30, 10, 15);
        moveDoor(DOOR_REST, 10, 15);
    }

    // Sleep for a little while to build the suspense
    delay(700);
 
    // Open door and delay again
    moveDoor(DOOR_OPEN, 5, 15);
    delay(700);

    // Flip Switch
    moveFinger(FINGER_PUSH, 5, 15);

    // Hide finger
    moveFinger(FINGER_REST, 3, 15);

    // Close door
    moveDoor(DOOR_REST, 3, 15);

}


//open door,move finger very slowly forward and back to hiding very slowly, then quickly close door
void slow()
{
    //Open door
    moveDoor(DOOR_OPEN, 1, 20);

    //Move close
    moveFinger(FINGER_PUSH - 20, 1, 30);

    // Flip Switch, note because we're so close, you need a little extra umph
    moveFinger(FINGER_PUSH+5, 5, 10);

    // Hide finger
    moveFinger(FINGER_REST, 10, 15);

    // Close door
    moveDoor(DOOR_REST, 10, 15);

 }


 //serious

 void serious() {

     
     //open door
     moveDoor(DOOR_OPEN, 3, 15);

     // Move finger partially out
     moveFinger(FINGER_PUSH - 40, 5, 15);
     delay(800);

     // Slam the door a few times
     for (int i = 0; i < 4; i++)
     {
         //Close door onto finger
         moveDoor(DOOR_OPEN - 40, 15, 10);
         delay(100);

         //open door
         moveDoor(DOOR_OPEN, 15, 10);
         delay(100);
     }
     

     // Move finger back quickly and slam the door
     moveFinger(FINGER_REST, 10, 5);
     moveDoor(DOOR_REST, 10, 5);
     delay(800);


     // Do the normal sequence, but do it fast
     //Moving door
     moveDoor(DOOR_OPEN, 10, 15);
     
     // Push switch
     moveFinger(FINGER_PUSH, 10, 15);
     delay(100);

     //hiding finger
     moveFinger(FINGER_REST, 10, 15);

     //hiding door
     moveDoor(DOOR_REST, 10, 15);


     
}

void trollClose(){

    // Open door normally
    moveDoor(DOOR_OPEN, 3, 15);

    // Push the button like normal, close the lid on yourself, and sit there for a while
    moveFinger(FINGER_PUSH, 5, 15);
    delay(100);
    moveFinger(FINGER_PUSH - 8, 4, 15);
    moveDoor(DOOR_OPEN-40, 3, 15);
    delay(2000);

    // Open the door again
    moveDoor(DOOR_OPEN, 3, 15);
    
    // Push the switch a few times to make a point
    for (int i = 0; i < 4; i++)
    {
        moveFinger(FINGER_PUSH - 25, 5, 15);
        delay(200);
        moveFinger(FINGER_PUSH - 8 , 4, 15);
        delay(200);
    }    
    delay(500);

    // Close normally
    moveFinger(FINGER_REST, 5, 15);        
    moveDoor(DOOR_REST, 3, 15);
    

}

void runPullHit()
{

    // Open door normally
    moveDoor(DOOR_OPEN, 3, 15);

    // Move as if we're going to hit the switch
    moveFinger(FINGER_PUSH - 25, 5, 15);
    delay(2000);
    // Put the finger away and close normally
    moveFinger(FINGER_REST, 1, 15);
    moveDoor(DOOR_REST, 1, 15);
    delay(1500);

    // GO FAST!
    moveDoor(DOOR_OPEN, 5, 15);
    moveFinger(FINGER_PUSH, 5, 10);
    delay(100);
    moveFinger(FINGER_REST, 5, 10);
    moveDoor(DOOR_REST, 5, 10);


}

void sneak()
   {

    // Open the door slowly
    moveDoor(DOOR_OPEN, 1, 30);       
    delay(2000);

    // Move the finger slowly    
    moveFinger(FINGER_PUSH - 50, 1, 30);
    moveDoor(DOOR_REST, 20, 15);

    // Then pounce and hide
    moveFinger(FINGER_PUSH, 25, 15);
    delay(100);
    moveFinger(FINGER_REST, 25, 15);
    delay(500);
   
   }

void heart()
{
    // prep
    moveFinger(FINGER_TAP - 5, 1, 15);
    delay(100);

    // beat the heart by using the finger on the lid, then accelerate, beat for a while, then smack the switch
    for (int i = 0; i < 5; i++)
    {
        moveFinger(FINGER_TAP, 5, 10);        
        delay(400);        
        moveFinger(FINGER_TAP - 5, 25, 15);
        delay(800);
    }
    for (int i = 10; i > 0; i--)
    {
        moveFinger(FINGER_TAP, 5, 10);
        delay(80 + (32*i));
        moveFinger(FINGER_TAP - 5, 25, 15);
        delay(160 + (64 *i));
    }
    for (int i = 0; i < 12; i++)
    {
        moveFinger(FINGER_TAP, 5, 10);
        delay(80);
        moveFinger(FINGER_TAP - 5, 25, 15);
        delay(160);
    }

    // GO FAST!
    moveDoor(DOOR_OPEN, 5, 15); 
    moveFinger(FINGER_PUSH, 5, 10);
    delay(100);
    moveFinger(FINGER_REST, 5, 10);
    moveDoor(DOOR_REST, 5, 10);

}

void twobits()
{
    delay(1000);

    // Shave and a haircut
    moveFinger(FINGER_TAP, 5, 10);
    delay(50);
    moveFinger(FINGER_REST, 1, 10);
    delay(300);

    moveFinger(FINGER_TAP, 5, 10);
    delay(50);
    moveFinger(FINGER_REST, 1, 10);
    delay(100);

    moveFinger(FINGER_TAP, 5, 10);
    delay(50);
    moveFinger(FINGER_REST, 1, 10);
    delay(75);

    moveFinger(FINGER_TAP, 5, 10);
    delay(50);
    moveFinger(FINGER_REST, 1, 10);
    delay(250);

    moveFinger(FINGER_TAP, 5, 10);
    delay(50);
    moveFinger(FINGER_REST, 1, 10);
    delay(300);
    delay(1200);

    // Two bits
    moveDoor(DOOR_OPEN, 5, 15); 
    delay(300);
    moveFinger(FINGER_PUSH, 5, 10);
    delay(100);
    moveFinger(FINGER_PUSH - 15, 4, 15);
    delay(1000);

    moveFinger(FINGER_REST, 3, 15);
    moveDoor(DOOR_REST, 3, 15);
    
}



