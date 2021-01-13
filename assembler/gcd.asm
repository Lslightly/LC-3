.ORIG x3000
            JSR complement 
            ADD R3, R2, R1
            BRzp #4
            ADD R2, R1, #0
            ADD R1, R0, #0
            ADD R0, R2, #0
            JSR complement
            ADD R1, R2, R1;
            BRp #-2
            BRz #4
            ADD R3, R0, #0
            ADD R0, R1, R0
            ADD R1, R3, #0
            BRnzp #-8
            HALT
complement  NOT R2, R0
            ADD R2, R2, #1
            RET
            .END