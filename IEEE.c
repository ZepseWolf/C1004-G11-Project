#include <stdio.h>
#include <math.h>
void decimalconver(double n, double d[], int *k);                                                                   // Function that converts the integral part of the decimal to binary
void fractconver(double n, double d[], int*k);                                                                      // Function that converts the fractional part of the decimal to binary
void binaryrep(double d[], double f[],int dnum, int fnum, double mantissa[],int *exponent, int *exnum, double ex[]);// Function that converts the decimal of the exponent and
double abs_val(double y);                                                                                           // compute the absolute value of a number
void binaryconver(double d[], int *ptr_sum);                                                                        // converts 8 bit binary exponents to decimal
void mantissaconver(double d[], int expo, double *mantisum);                                                        // converts 23bit mantissa (plus 1), to decimal
void sum(double dec2, double frac2, double *ptr_sum);                                                               // compute the decimal value of the IEEE 754 input value 

int main()
{   
    double intpart, input, num, fractpart;       // intpart = integral part of the input in decimal, fractpart = fractional part of the input in decimal, num=the absolute value of the input, input = user's input (to convert to IEEE 754)
    double decint[32], frac[32], mantissa[23];   // decint = array that stores integral part of the input in binary, frac = arry that stores the fractional part of the input in binary, mantissa = array that stores the generated mantissa in binary
    int choice;                                  // user choice for which coversion
    int dnum;                                    //the number of binary digits in the integral part. 
    int fnum;                                    // the number of binary digits in the fractional part.
    int aexponent=0;                             // actual exponent
    double exponent[8];                          //8bit biased exponent
    int exnum,biasedexpo=0;                      //number of binary digits in the exponent part (to fulful function requirements)
    int sign = 0;                                // sign bit(choice1)
    int sign2=0,exposum=0, actexpo;              // sign bit(choice 2), exposum = exponent+biased, actexpo = actual exponent
    double exinput[9], mantinput[23], totalsum=0, mantisum=1; //exponent input array, mantissa input array, total sum = decimal of IEEE 754 floating number, mantisum = decimal value of the mantissa + 1

    printf("\nPlease select 1 to convert from dec to IEEE 754 binary floating number, 2 for to convert from IEEE 754 binary floating number into decimal number.");
    printf("\n\nEnter an option: ");
    scanf("%d",&choice);

   
    if (choice == 1)
    {
        printf("Please input a real number: ");
        scanf("%lf",&input); //named the input number fractpart 

        if(input <= 0)
        {
            sign = 1;
        }

        num = abs_val(input);   //removes the negative sign of the users input if applicable

        fractpart = modf(num, &intpart); //splits number into integer part and fractional part

        decimalconver(intpart,decint,&dnum); //converts integral part of the input to binary and store it in array decint
        fractconver(fractpart, frac, &fnum); // converts fractional part of the input to binary and store it in array frac
    
        for(int i=0; i<dnum; i++)
        {
            printf("%g",decint[i]);   //prints the integral part of the input
        }
        printf(".");                  // decimal point
        for(int i=0; i<fnum; i++)
        {
            printf("%g",frac[i]);     //prints out the fractional part of the input
        }

        binaryrep(decint, frac, dnum, fnum, mantissa,&aexponent,&exnum,exponent); //generates, the actual exponent, 8 bit exponent in an array, and mantissa in an array
        biasedexpo=aexponent+127; //compute the biased exponent by adding 127 to the actual aexponent
        decimalconver(biasedexpo,exponent,&exnum);//convert the decimal biased exponent to binary amd store it in the array exponent

        printf("\nThe input of the decimal number is: %g",input);   //Prints the user input
        printf("\nAfter conversion, the 1-bit signbit is: %d",sign);//Prints the sign bit
        printf("\nThe actual exponent is: %d", aexponent);          //Prints the actual exponent
        printf("\n The 8 bit exponent is: ");
        for(int i=0; i<8; i++)                                      //Prints the 8 bit exponent array
        {
            printf("%g",exponent[i]);
        }
        printf("\n The 23-bit fracton is: ");                       //Prints the 23 bit mantissa array
        for(int i=0; i<23; i++)
        {
            printf("%g",mantissa[i]);
        } 
        printf("\n IEEE754 binary floating point: %d ",sign);       //Prints the whole IEEE 754 Binary floating point
        for(int i=0; i<8; i++)
        {
            printf("%g",exponent[i]);
        }
        printf(" ");
        for(int i=0; i<23; i++)
        {
            printf("%g",mantissa[i]);
        } 
    }

    if(choice == 2)       //User chooses to convert IEEE 754 binary floating point number to decimal
    {
        printf("\nPlease input a 32 bit IEEE 754 binary floating point Number: ");
        printf("\nSign bit: ");   //Ask user to input sign bit first
        scanf("%d",&sign2);

        for(int i=0;i<8;i++)
        {
            printf("\nEnter your 8 bit exponent input Bit[%d]: ",i+1); // Ask user to input 8 bit exponent, stored in exinput array
            scanf("%lf",&exinput[i]);
        }

        for(int i=0;i<23;i++)
        {
            printf("\nEnter your 23 bit Mantissa Bit[%d]: ",i+1); // Ask user to input 23 bit mantissa, stored in mantinput array
            scanf("%lf",&mantinput[i]);
        }



        printf("Your input is: %d ",sign2); //Prints out the user input
        for(int i=0;i<8;i++)
        {
            printf("%g",exinput[i]);
        }
        printf(" ");
        for(int i=0;i<23;i++)
        {
            printf("%g",mantinput[i]);
        }


    binaryconver(exinput, &exposum);  //convert the 8bit exponent input by user to decimal
    actexpo = exposum - 127; //calculating the actual exponent by subtracting the bias away
    mantissaconver(mantinput,actexpo, &mantisum); //converts the whole mantisa, including the 1. in the front to decimal
    sum(mantisum,actexpo,&totalsum); //computing the decimal value of the IEEE754 binary floating number.

    if(sign2==1)
    {
        totalsum= totalsum*-1; //if sign userinput is 1, converts the decimal value to negative
    }

    printf("\nBefore conversion, the 1 bit sign bit is: %d",sign2); //Prints out the sign bit
    printf("\nThe 8 bit biased exponent is: ");                     //Prints out the 8bit biased exponent
    for(int i=0;i<8;i++)
    {
        printf("%g",exinput[i]);
    } 
    printf("\nThe actual exponent is: %d", actexpo);                //Prints out the actual exponent
    printf("\nThe 23 bit fraction is: ");                           //Prints out the mantissa
    for(int i=0;i<23;i++)
    {
        printf("%g",mantinput[i]);
    }
    printf("\nAfter conversion, the decimal number is: %lf", totalsum);//Prints out the decimal value of the IEEE754 Binary Floating point number

    }
    return 0;

}

double abs_val(double y)
{
if(y<0)
return(y*-1);
else
return(y);
}
  
void decimalconver(double n, double d[], int *ptr_dnum) //function to convert the integral part of the decimal to binary
{
    int i = 0, k = 0;
    double mule[32]; //using a temporay storage to store the binary values in the oppoite
    double remainder;
        for(int p=0;p<32;p++)
        {
            mule[p]=0;
        }
        while (n > 0)
        {
            remainder =  fmod(n,2);  //generating each binary values by dividing the decimal by 2 and getting the remainder
            n = n/2;
            if(n==1.5)
            {
                n=1;
                mule[i]= remainder; // storeing the binary values(in opposite) in the temporary array
                i++;
                mule[i]= 1;
                break;
            }
            n = (int)n;
            mule[i]= remainder;
            if(n != 0)
            {
                i++;
            }
        }
        if(i != 0)
        {
            if(i<7)
            {
                i=7;   // making the array at least 8 bit long to avoid error when flipping the temporary array
            }
            for(int j=i; j>=0;j--)
            {
                d[k] = mule[j]; //flipping the temporary array and storing the true binary values in the main array
                k++;            // calculating the number of bits in the main array
            }
        }
        *ptr_dnum = k;          // storing the number of bits to be able to print the array accurately later on

}

void fractconver(double f, double fra[], int*ptr_fnum) // function to convert the fractional part of the decimal to binary
{
    int i = 0, k = 0;
    double  discard;
    double remainder;
        while (f != 1 && i<10) // limiting the fraction binary to 10 bits
        {
            k = 0; // reset k
            f = f * 2; // multiplay fraction decimal by 2
            if(f>=1)   // if results is more than or equal to one
            {
                k = 1; // store binary 1
            }
            if(f<1)    // if results is less than one
            {
                k = 0; // store binary 0
            }
            fra[i]=k;  // store results in fractional binary array
            if(f != 1.0)
            {
                f = modf(f, &discard); // filter out the decimal part for the next round of *2
            }
            i++;
        }
        *ptr_fnum = i;   //stores the number of binary in the array to print out accurately later on
  
}

void binaryrep(double d[], double f[], int dnum, int fnum, double mantissa[],int *ptr_expo,int *exnum, double ex[])
{

    int dcount=0; //counts the first '1' in the integral part
    int breakpoint;
    int biasedexpo;
    int fcount=0; // counts the first '1' in the fractional part if there is no '1' in the integral part
    int m=0; //mantissa array counter
    for(int i=0; i<dnum; i++)
    {
        if(d[i] == 1)
        {
            dcount = i;
            break;
        }
    }   

    if(dcount == dnum) // meaning there is no '1' in the integral part.
    {
        for(int i=0; i<fnum; i++)
        {
           
            if(f[i] == 1)
            {
                fcount = i;
                break;
            }
        }
        *ptr_expo = -(fcount+1); // exponent will be the negative of total number of digits in the fractional minus the positon of the first '1' in the fractonal
        breakpoint = fcount+1;   // E.G. 0.001001 fcount = 2, breakpoint will be position number 3
        for(int i=breakpoint;i<fnum;i++)
        {
            mantissa[m] = f[i];
            m++;
        }
        int count = m; // capturing which digit out of 23 was the mantissa array filled up
        for(int i=0;i<(23-count);i++)
        {
            mantissa[m]=0; // filling in the rest of the array with '0'
            m++;
        }

    }
    if (dcount != dnum)
    {
        *ptr_expo = (dnum-1)-dcount; // exponent will be the total number of digits in the intergral minus the position number of the first '1'. 1110 4-1-0=3
        breakpoint = *ptr_expo;       
        breakpoint = dnum - breakpoint; 
        for(int i=breakpoint; i<dnum; i++)
            {
                mantissa[m] = d[i]; // inserting the digits of the integral into mantissa array after breakpoint
                m++;
            }
        for(int i=0; i<fnum;i++)
        {
            mantissa[m] = f[i]; // inserting the fractional digits into mantissa array after inserting integral portion
            m++;
        }
        int count = m; // capturing which digit out of 23 was the mantissa array filled up
        for(int i=0;i<(23-count);i++)
        {
            mantissa[m]=0; // filling in the rest of the array with '0'
            m++;
        }
    }



    
}

void binaryconver(double d[], int *ptr_sum) // function to convert binary to decimal(without fractions)
{
    int i;
    float check=0;
    for(i=0; i<8;i++)        // coded to suit the conversion of 8bit exponent to binary as only the 8bit exponent requires this funciton in the whole program
    {
        
        if(d[i]==1)         // checks which bit in the binary is a 1
        {
            *ptr_sum=*ptr_sum + pow(2, 7-i); // multiplies by 2 to the power of n, where n depends on the position of the bit
            check = *ptr_sum;

        }
    }
}

void mantissaconver(double d[], int expo, double *mantisum) //function to convert fraction binary to decimal
{
  
        int i;
        float check=0; // coded to suit the conversion of the mantissa to decimal, including the 1. at the front.
        for(i=0; i<23;i++)
        {
            if(d[i]==1)
            {
                *mantisum=*mantisum + pow(2, -(i+1));
                check=*mantisum;
            }
        }
    

}

void sum(double manti, double expo, double *ptr_sum) // computes the final decimal, multiplying the mantissa with the exponent
{
    *ptr_sum= manti*pow(2,expo);
}
