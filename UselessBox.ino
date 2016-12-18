#include <Servo.h> 

// Setup these directives for your particular hardware configuration
#define SWITCH_PIN 3
#define FINGER_PIN 9
#define DOOR_PIN 10
#define LED_PIN 13

// Depending on circuit design, the switch can be in the "ON" position as either high or low
#define SWITCH_ON_POLARITY LOW

#define ASCII_OFFSET 48

Servo doorServo;
Servo fingerServo;

int volatile doorPos = 0;
int volatile fingerPos = 0;
int volatile doorOffset = 0;
int volatile fingerOffset = 0;


int volatile setOffset = 0;
int volatile door_1_finger_0 = 0;


int serial_char = 0;
int pos = 0;

void setup()
{


    // The pin is HIGH when the switch is in the "off" position (i.e. no action is required)
    // The pin is LOW when the switch is in the "on" position (i.e. it must be turned off)  
    pinMode(SWITCH_PIN, INPUT_PULLUP);

    // For debugging
    pinMode(LED_PIN, OUTPUT);

    
    // Servo setup
    doorServo.attach(DOOR_PIN);           //setup door servo
    doorServo.write(0);           //set door to hiding position 
    fingerServo.attach(FINGER_PIN);          //setup finger servo    
    fingerServo.write(0);            //set finger to hiding position


    //open serial terminal for troubleshooting
    Serial.begin(9600);

    // A little pause before operation
    delay(100);

}

void loop()
{

    if (Serial.available() > 0)
    {
        serial_char = Serial.read();

        // First check, see if this is a decimal character. If not, set the active servo below
        if (serial_char >= 47 && serial_char < 58)
        {
            // Take an ASCII char ant convert it to a position 0=0, 1=20, 2=40, ... 9=180
            // Servo Pos = (char-ASCII_OFFSET) * 20

            // Convert ASCII into a number
            if (setOffset == 0)
            {
                pos = ((serial_char - ASCII_OFFSET) * 20);
            }
            

            // see if we are updating the finger or the door
            if (door_1_finger_0 == 0)
            {
                if (setOffset == 1)
                {
                    fingerOffset = (serial_char - ASCII_OFFSET);
                }

                fingerPos = pos + fingerOffset;
                fingerServo.write(fingerPos);


            }
            else
            {
                if (setOffset == 1)
                {
                    doorOffset = (serial_char - ASCII_OFFSET);
                }

                doorPos = pos + doorOffset;
                doorServo.write(doorPos);
                

            }
        }
        // see if it is a "D" or "d", if so, set the active device to door
        else if (serial_char == 68 || serial_char == 100)
        {
            door_1_finger_0 = 1;
            setOffset = 0;

        }
        // see if it is a "F" or "f", if so, set the active device to finger
        else if (serial_char == 70 || serial_char == 102)
        {
            door_1_finger_0 = 0;
            setOffset = 0;
        }
        // see if it is a "O" or "o", if so, set the offset for the active servo
        else if (serial_char == 79 || serial_char == 111)
        {
            setOffset = 1;
        }

 


    }

// on each loop, update the LED so you can troubleshoot the switch polarity
if (digitalRead(SWITCH_PIN) == SWITCH_ON_POLARITY)
{
    // Turn on if the switch is on, otherwise turn off
    digitalWrite(LED_PIN, HIGH);
}
else
{
    digitalWrite(LED_PIN, LOW);
}

}