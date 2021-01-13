.ORIG	x3000
INIT    AND R0,R0,#0
        ADD R1,R0,#0
        ADD R2,R0,#0
        ADD R3,R0,#0
        ADD R4,R0,#3
        ADD R5,R0,#5
        ADD R6,R0,#8   
        LEA R0,PLAYER
        STR R3,R0,#0
        JSR STATEOUT

GETIN       JSR PLAYERCALL
            GETC
            OUT
            LD R1,PAPHA
            ADD R1,R1,R0
            GETC
            OUT
            LD R2,PNUMB0
            ADD R2,R2,R0
            LD R0,NEWLINE
            OUT
            
            
JUDGE       ADD R1,R1,#0
            BRn INVALID
            ADD R3,R1,#-2
            BRp INVALID
            ADD R2,R2,#0
            BRn INVALID
            JSR MINUS
            ADD R3,R1,#0
            BRnp #3
            ADD R3,R2,R4
            BRn INVALID
            ADD R4,R3,#0
            ADD R3,R1,#-1
            BRnp #3
            ADD R3,R2,R5
            BRn INVALID
            ADD R5,R3,#0
            ADD R3,R1,#-2
            BRnp #3
            ADD R3,R2,R6
            BRn INVALID
            ADD R6,R3,#0
            
            ADD R3,R4,R5
            ADD R3,R3,R6
            BRz WIN
            LD  R1,PLAYER
            NOT R1,R1
            ST  R1,PLAYER
            JSR STATEOUT
            BRnzp GETIN


STATEOUT    ST R7,RETPLACE
            LEA R0,ROWSTRINGA
            PUTS
            ADD R1,R4,#0
            JSR STONEOUT
            LEA R0,ROWSTRINGB
            PUTS
            ADD R1,R5,#0
            JSR STONEOUT
            LEA R0,ROWSTRINGC
            PUTS
            ADD R1,R6,#0
            JSR STONEOUT 
            LD R7,RETPLACE
            RET



MINUS       NOT R2,R2
            ADD R2,R2,#1
            RET

            
INVALID     LEA R0,ERRSTRING
            PUTS
            AND R0, R0, #0
            ADD R0, R0, x000A
            OUT
            BRnzp GETIN

PLAYERCALL  LD R3,PLAYER
            BRn #3
            LEA R0,PLAY1
            PUTS
            BRnzp #2
            LEA R0,PLAY2
            PUTS
            RET



PNUMB0      .FILL	xFFD0            
PAPHA       .FILL   xFFBF

WIN         LD R1 PLAYER
            BRz #3
            LEA R0,PLAY1WIN
            PUTS
            BRnzp #2
            LEA R0,PLAY2WIN
            PUTS
            HALT            

ROWSTRINGA .STRINGZ	"ROWA: "
ROWSTRINGB .STRINGZ	"ROWB: "
ROWSTRINGC .STRINGZ	"ROWC: "
ERRSTRING   .STRINGZ	"Invalid move. Try again."

STONEOUT    ADD R1,R1,#0
            BRz #4
            LD R0, STONE
            OUT
            ADD R1,R1,#-1
            BRp STONEOUT
            LD R0,NEWLINE
            OUT
            RET
STONE   .FILL   x006F   
NEWLINE .FILL	x000A   

RETPLACE .BLKW   #1
PLAYER  .BLKW	#1
PLAY1   .STRINGZ	"Player 1, choose a row and number of rocks:"
PLAY2   .STRINGZ	"Player 2, choose a row and number of rocks:"
PLAY1WIN .STRINGZ	"Player 1 Wins."
PLAY2WIN .STRINGZ	"Player 2 Wins."

.END