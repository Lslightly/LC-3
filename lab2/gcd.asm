            .ORIG x3000
            LD R0, Data0
            LD R1, Data1
LOOP        NOT R2, R1
            ADD R2, R2, #1  ;   R2 <- -R1
            ADD R2, R0, R2  ;   R2 <- R0-R1
            BRz Done
            BRp Positive    ;   (R0 > R1)?(R0 <- R2):(R1 <- -R2) (R0 != R1)
            NOT R2, R2
            ADD R1, R2, #1  ;   R1 <- -R2
            BRnzp LOOP
Positive    ADD R0, R2, #0  ;   R0 <- R2
            BRnzp LOOP
Done        HALT
Data0       .FILL x0005
Data1       .FILL x000C
            .END


