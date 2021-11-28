#include <stdio.h>    // Used for printf() statements
#include <wiringPi.h> 

// const int pwmPin = 18;
const int ledPin = 18; // Regular LED - Broadcom pin 23, P1 pin 16
const int butPin = 17; // Active-low button - Broadcom pin 17, P1 pin 11

int main(void)
{   
    int count = 0 , holding= 0;
    wiringPiSetupGpio();
    pinMode(ledPin, OUTPUT);
    pinMode(butPin, INPUT);      // Set button as INPUT
    pullUpDnControl(butPin, PUD_UP); 
    
    
    while(1)
    {   


        if (digitalRead(butPin)) // Button is released if this returns 1
        {
            // digitalWrite(ledPin, LOW);     // Regular LED off

        }
        else // If digitalRead returns 0, button is pressed
        {
            digitalWrite(ledPin, HIGH); // Turn LED ON
            delay(500); // Wait 75ms
            digitalWrite(ledPin, LOW); // Turn LED OFF
            delay(500); // Wait 75ms again
        }
    }
    return 0;
}