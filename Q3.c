#include <stdio.h>    
#include <wiringPi.h> 

const int ledPin = 18; // Regular LED 
const int butPin = 17; // Active-low button - Broadcom pin 17

void checkForInterrupt(int *stop);
void triggerSequence(int* count,int freq);

int main(void)
{   // Default inputs
    int count = 1 , holding= 0 , freq = 500;
    
    wiringPiSetupGpio();         // Init wiringPi
    pinMode(ledPin, OUTPUT);     // Set Pin 18 as output for led
    pinMode(butPin, INPUT);      // Set Pin 17 as input for button
    pullUpDnControl(butPin, PUD_UP); 

    // Input blink frequency 
    printf("\nInput blink frequency in ms: ");
    scanf("%d", &freq);

    // Select starting sequence 
    printf("\nSequence 1 - Turn on leds ");
    printf("\nSequence 2 - Turn off leds");
    printf("\nSequence 3 - Blink leds");
    printf("\nSequence 0 - Nil ");
    printf("\nInput Sequence 0 - 3: ");
    scanf("%d", &count);

    //Check for wrong input
    if(count != 0 && (count == 1 || count == 2 ||count == 3 )){
        triggerSequence(&count,freq);
        count ++;
    }else{
        // Default starting sequence 
        count = 1;
    }
    while(1)
    {   
        delay(10); // Due to high tick , machine may malfunction hence delayed every action
        if (digitalRead(butPin)){
            if(holding == 1){
                // Enter Click function   
                triggerSequence(&count,freq);
                count++;
                holding = 0;
            }
        }
        else{
            // Detect Clicked 
            holding = 1;            
        }
    }
    return 0;
}

void checkForInterrupt(int *stop){
    // Check for button press
    if (!digitalRead(butPin)){
        *stop = 0;
    }
}

void triggerSequence(int *count , int freq){
    // Scenario sequence
    if(*count == 1 ){
        printf("\n=== Init first sequence ===");
        digitalWrite(ledPin, HIGH);
    }
    else if(*count == 2){
        printf("\n=== Init second sequence ===");
        digitalWrite(ledPin, LOW);
    }
    else if(*count == 3){
        printf("\n=== Init third sequence (Long button press to go to go to next sequence) ==="  );
        int stop = 1;
        while (stop)
        {   
            delay(10); 
            checkForInterrupt(&stop);
            digitalWrite(ledPin, HIGH); 
            checkForInterrupt(&stop);
            delay(freq); 
            checkForInterrupt(&stop);
            digitalWrite(ledPin, LOW);
            checkForInterrupt(&stop);
            delay(freq); 
            checkForInterrupt(&stop);
        }
        *count = 0;
    }   
}