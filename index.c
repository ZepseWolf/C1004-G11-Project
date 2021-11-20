#include <stdio.h>    // Used for printf() statements
#include <wiringPi.h> 

// const int pwmPin = 18;
const int ledPin = 18; // Regular LED - Broadcom pin 23, P1 pin 16
const int butPin = 17; // Active-low button - Broadcom pin 17, P1 pin 11

void blink(int *freq);

int main(void)
{   
    int count = 1 , holding= 0 , freq = 500 ;
    wiringPiSetupGpio();
    pinMode(ledPin, OUTPUT);
    pinMode(butPin, INPUT);      // Set button as INPUT
    pullUpDnControl(butPin, PUD_UP); 
    
    
    while(1)
    {   delay(10);
        if (digitalRead(butPin)) // Button is released if this returns 1
        {
            if(holding == 1){
                // click   
                printf("\n Clicking");
                if(count == 1 ){
                    printf("\n start c1 "  );
                    digitalWrite(ledPin, HIGH); 
                    delay(freq); 
                    digitalWrite(ledPin, LOW); 
                    delay(freq); 
                    printf("\n end c1 "  );
                    
                }
                else if(count == 2){
                    printf("\n start c2 "  );
                    digitalWrite(ledPin, HIGH);
                    printf("\n end c2 "  );
                //     count = 3;
                    // count++;
                }
                else if(count == 3){
                    printf("\n start c3 "  );
                    digitalWrite(ledPin, LOW);
                    printf("\n end c3 "  ); 
                    count = 0;
                }
                
            printf("\n Button clicked! , count:%d and holding : %d" ,count,holding );
            count++;
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

void blink(int *freq){
    digitalWrite(ledPin, HIGH); 
    delay(*freq); 
    digitalWrite(ledPin, LOW); 
    delay(*freq); 
}