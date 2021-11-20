#include <stdio.h>    // Used for printf() statements
#include <wiringPi.h> 

// const int pwmPin = 18;
const int ledPin = 18; // Regular LED - Broadcom pin 23, P1 pin 16
const int butPin = 17; // Active-low button - Broadcom pin 17, P1 pin 11

int main(void)
{   
    int count = 1 , holding= 0;
    wiringPiSetupGpio();
    pinMode(ledPin, OUTPUT);
    pinMode(butPin, INPUT);      // Set button as INPUT
    pullUpDnControl(butPin, PUD_UP); 
    
    
    while(1)
    {   
        if (digitalRead(butPin)) // Button is released if this returns 1
        {
            // digitalWrite(ledPin, LOW);     // Regular LED off
            if(holding == 1){
                // click
                if(count == 1 ){
                    digitalWrite(ledPin, HIGH); // Turn LED ON
                    delay(500); // Wait 75ms
                    digitalWrite(ledPin, LOW); // Turn LED OFF
                    delay(500); // Wait 75ms again
                    digitalWrite(ledPin, HIGH); // Turn LED ON
                    delay(500); // Wait 75ms
                    digitalWrite(ledPin, LOW); // Turn LED OFF
                    delay(500);
                    count++;
                }
                else if(count == 2){
                    printf("\n start c2 "  );
                    digitalWrite(ledPin, HIGH);
                    delay(500);
                    printf("\n end c2 "  );
                    count++;
                }
                else if(count == 3){
                    printf("\n start c3 "  );
                    // delay(500);
                    digitalWrite(ledPin, LOW);
                    delay(500);
                    printf("\n end c3 "  ); 
                    count = 0;
                }
                printf("\n Button clicked! , count:%d and holding : %d" ,count,holding );
                holding = 0;
            }
        }
        else // If digitalRead returns 0, button is pressed
        {
            // Do some blinking on the ledPin:
            holding = 1;            
        }
    }
    return 0;
}