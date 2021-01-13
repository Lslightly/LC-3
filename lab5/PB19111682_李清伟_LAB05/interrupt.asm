; Unfortunately we have not YET installed Windows or Linux on the LC-3,
; so we are going to have to write some operating system code to enable
; keyboard interrupts. The OS code does three things:
;
;    (1) Initializes the interrupt vector table with the starting
;        address of the interrupt service routine. The keyboard
;        interrupt vector is x80 and the interrupt vector table begins
;        at memory location x0100. The keyboard interrupt service routine
;        begins at x1000. Therefore, we must initialize memory location
;        x0180 with the value x1000.
;    (2) Sets bit 14 of the KBSR to enable interrupts.
;    (3) Pushes a PSR and PC to the system stack so that it can jump
;        to the user program at x3000 using an RTI instruction.

        .ORIG x800
        ; (1) Initialize interrupt vector table.
        LD R0, VEC
        LD R1, ISR
        STR R1, R0, #0

        ; (2) Set bit 14 of KBSR.
        LDI R0, KBSR
        LD R1, MASK
        NOT R1, R1
        AND R0, R0, R1
        NOT R1, R1
        ADD R0, R0, R1
        STI R0, KBSR

        ; (3) Set up system stack to enter user space.
        LD R0, PSR
        ADD R6, R6, #-1
        STR R0, R6, #0
        LD R0, PC
        ADD R6, R6, #-1
        STR R0, R6, #0
        ; Enter user space.
        RTI
        
VEC     .FILL x0180
ISR     .FILL x1000
KBSR    .FILL xFE00
MASK    .FILL x4000
PSR     .FILL x8002
PC      .FILL x3000
        .END

        .ORIG x3000
        ; *** Begin user program code here ***
        LEA R0, ICS
LOOP    PUTS
        JSR DELAY
        BRnzp LOOP

DELAY   ST R1, SaveR1
        LD R1, COUNT
REP     ADD R1, R1, #-1
        BRp REP
        LD R1, SaveR1
        RET
COUNT   .FILL   x7FFF
SaveR1  .BLKW   #1

ICS     .STRINGZ        "ICS2020 "
        ; *** End user program code here ***
        .END

        .ORIG x1000
        ; *** Begin interrupt service routine code here ***
        ST R0, SaveR0
        ST R2, SaveR2
        GETC
        LD R2, Zero
        NOT R2, R2
        ADD R2, R2, #1
        ADD R2, R0, R2
        BRn Invalid
        LD R2, Nine
        NOT R2, R2
        ADD R2, R2, #1
        ADD R2, R0, R2
        BRp Invalid
        ADD R2, R0, #0
        LD R0, NewLine
        OUT
        ADD R0, R2, #0
        OUT
        LEA R0, IsString
        PUTS
        LD R0, NewLine
        OUT
        LD R0, SaveR0
        LD R2, SaveR2
        RTI
Invalid ADD R2, R0, #0
        LD R0, NewLine
        OUT
        ADD R0, R2, #0
        OUT
        LEA R0, NotString
        PUTS
        LD R0, NewLine
        OUT
        LD R0, SaveR0
        LD R2, SaveR2
        RTI
SaveR0  .BLKW   #1
SaveR2  .BLKW   #1
Zero    .FILL   x0030
Nine    .FILL   x0039
NewLine .FILL   x000A
IsString        .STRINGZ        " is a decimal digit."
NotString       .STRINGZ        " is not a decimal digit."

        ; *** End interrupt service routine code here ***
        .END