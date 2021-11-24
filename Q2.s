.data               @ to define the variables we will be using
stringinputlength1: .asciz "\nEnter a positive decimal number Length1 (meters): "
stringinputwidth1: .asciz "\nEnter a positive decimal number Width1  (meters): "
stringoutput: .asciz "\nThe total area of 2 shapes is (sqm): %d\n"
s1: .word 20        @ define constant s1 variable to store one length of triangle
s2: .word 15        @ define constant s2 variable to store one length of triangle
s3: .word 0         @ define s3 which will be retrieved as input (equals to length1)
length1: .word 0    @ define length1 which will be retrieved as input
width1: .word 0     @ define width1 which will be retrieved as input
s: .word 0          @ define s which will contain (s1+s2+s3) / 2
rectarea: .word 0   @ define rectarea which will contain area of rectangle
triarea: .word 0    @ define triarea which will contain area of triangle
scantype: .asciz "%d"   @ to be used for scan type
.text               @ for actual code
.global main        @ start the assembly code
.extern printf      @ external function printf
.extern scanf       @ external function scanf

main:
    PUSH {ip, lr}               @ store return address and ip register in stack
    @ prompt and store length1 input
    LDR R0, =stringinputlength1 @ load string into R0 to perform procedural call.
    BL printf                   @ branch link to printf, like 'printf("...")' in C; prompt user for length1 input
    LDR R0, =scantype           @ load scantype address into R1 to perform scanf
    LDR R1, =length1            @ load length1 address into R0 to perform scanf
    BL scanf                    @ scans user input and store into address inside r1 (length1)

    @ prompt and store width1 input
    LDR R0, =stringinputwidth1  @ load string, same as above
    BL printf                   @ prompt user for width1 input
    LDR R0, =scantype           @ load scantype address into R1 to perform scanf
    LDR R1, =width1             @ load width1 address into R0 to perform scanf
    BL scanf                    @ scans user input and store into address inside r1 (width1)

    @ assign length1 to s3
    LDR R0, =s3                 @ load address of s3 into R0
    LDR R1, =length1            @ load address of length1 into R1
    LDR R1, [R1]                @ load value of length1 into R1
    STR R1, [R0]                @ store value of length1 into s3 (as s3 = length1)

    @ calculate value of s = (s1 + s2 + s3) / 2
    LDR R1, =s1                 @ load address of s1 into R1
    LDR R1, [R1]                @ load value of s1 into R1
    LDR R2, =s2                 @ load address of s2 into R2
    LDR R2, [R2]                @ load value of s2 into R2
    LDR R3, =s3                 @ load address of s3 into R3
    LDR R3, [R3]                @ load value of s3 into R3
    ADD R1, R1, R2              @ add s1 s2 store into s1; s1 = s1 + s2
    ADD R1, R1, R3              @ add s1 s3 store into s1; s1 = s1 + s3
    LDR R0, =s                  @ load address of s into R0
    LSR R1, R1, #1              @ logical shift right R1 by 1 bit; divide by 2
    STR R1, [R0]                @ store result value into  s

    @ calculate area of triange triarea (sqrt formula)
    LDR R1, =s1                 @ load address of s1 into R1
    LDR R1, [R1]                @ load value of s1 into R1
    LDR R0, [R0]                @ load value of s into R0
    @ Registers state: R0:s, R1:s1  R2:s2, R3;s3;
    SUB R1, R0, R1              @ subtract s1 from s and store into R1 (s - s1)
    SUB R2, R0, R2              @ subtract s2 from s and store into R2 (s - s2)
    SUB R3, R0, R3              @ subtract s3 from s and store into R3 (s - s3)
    MUL R0, R1, R0              @ multiply s and s-s1 and store into R0
    MUL R0, R2, R0              @ multiply above result by s-s2 and store into R0
    MUL R0, R3, R0              @ multiply above result by s-s3 and store into R0

    @ square root using odd number and Arithmethic Progression to derive the nth term
    MOV R1, #1                  @ store incrementing odd number starting with 1
    MOV R2, #1                  @ store square root result
    LOOP:
        SUB R0, R1              @ subtract odd number by original number
        CMP R0, #0              @ compare to see if original number is 0
        BLE END                 @ end loop if original number is 0 or less
        ADD R1, #2              @ increment to next odd number (n+2)
        ADD R2, #1              @ increment counter (result)
        B LOOP                  @ repeat as original number is greater than 0
    END:                        @ back to main program, with result stored in R1
    LDR R0, =triarea            @ load address of triarea into R1
    STR R2, [R0]                @ store result of formula into triarea

    @ calculate area of rectangle rectarea (length1 * width1)
    LDR R1, =length1            @ load address of length1 into R1
    LDR R1, [R1]                @ load value of length1 into R1
    LDR R2, =width1             @ load address of width2 into R2
    LDR R2, [R2]                @ load value of width1 into R2
    MUL R1, R2                  @ multiply length1 and width1 and store into R1
    LDR R0, =rectarea           @ load address of rectarea into R0
    STR R1, [R0]                @ store result of length*width into rectarea

    @ add both areas together totalarea
    LDR R1, =triarea            @ load address of triarea into R1
    LDR R1, [R1]                @ load value of triarea into R1
    LDR R2, =rectarea           @ load address of rectarea into R2
    LDR R2, [R2]                @ load value of rectarea into R2
    ADD R1, R1, R2              @ add triarea and rectarea and store into R1

    @ print output totalarea
    LDR R0, =stringoutput
    BL printf                   @ print string stored in R0 (output) and result stored in R1 (totalarea)
    POP {ip, pc}    @ Pop ip reg., and pop lr (link reg.) value to PC, return;