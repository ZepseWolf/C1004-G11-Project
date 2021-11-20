#include <stdio.h>    // Used for printf() statements
#include <wiringPi.h> 

// const int pwmPin = 18;
const int ledPin = 18; // Regular LED - Broadcom pin 23, P1 pin 16
const int butPin = 17; // Active-low button - Broadcom pin 17, P1 pin 11

void checkForInterupt(int *stop);
void triggerSequence(int count,int freq);

int main(void)
{   
    int count = 1 , holding= 0 , freq = 500 , choice =0;
    wiringPiSetupGpio();
    pinMode(ledPin, OUTPUT);
    pinMode(butPin, INPUT);      // Set button as INPUT
    pullUpDnControl(butPin, PUD_UP); 
    
    printf("\nInput blink frequency in ms: ");
    scanf("%d", &freq);
    
    printf("\nSequence 1 - Turn on leds ");
    printf("\nSequence 2 - Turn off leds");
    printf("\nSequence 3 - Blink leds");
    printf("\nSequence 0 - Nil ");
    printf("\nInput Sequence 0 - 3: ");
    scanf("%d", &choice);

    if(choice !=0){
        triggerSequence(choice,freq);
    }

    while(1)
    {   delay(10);
    
        if (digitalRead(butPin)){
            if(holding == 1){
                // Click function   
                triggerSequence(count,freq);
                count++;
                holding = 0;
            }
        }
        else{
            holding = 1;            
        }
    }
    return 0;
}

void checkForInterupt(int *stop){
    if (!digitalRead(butPin)){
        *stop = 0;
    }
}

void triggerSequence(int count , int freq){
    if(count == 1 ){
        printf("\n=== Init first sequence ===");
        digitalWrite(ledPin, HIGH);
    }
    else if(count == 2){
        printf("\n=== Init second sequence ===");
        digitalWrite(ledPin, LOW);
    }
    else if(count == 3){
        printf("\n=== Init second sequence (Long button press to go to go to next sequence) ==="  );
        int stop = 1;
        while (stop)
        {   
            delay(10); 
            checkForInterupt(&stop);
            digitalWrite(ledPin, HIGH); 
            checkForInterupt(&stop);
            delay(freq); 
            checkForInterupt(&stop);
            digitalWrite(ledPin, LOW);
            checkForInterupt(&stop);
            delay(freq); 
            checkForInterupt(&stop);
        }
        count = 0;
    }   
}