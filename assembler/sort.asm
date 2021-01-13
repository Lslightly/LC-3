.ORIG x3000
        LDI     R0, HEAD        ;加载首地址

Outer   LDR     R0, R0, #0      ;即因为R0存的是下一个节点的地址所以等价于a = a -> next 
        BRnp    Inner           ;如果R0为0则比较完毕
        HALT

Inner   LDI     R1, HEAD
Cycle   LDR     R2, R1, #0      ;加载下一节点地址       
        BRz     Outer        
        LDR     R3, R1, #1      ;加载数据
        LDR     R4, R2, #1
        
        NOT     R5, R4          ;取补码
        ADD     R5, R5, #1
        ADD     R5, R5, R3      
        BRnz    Next
        STR     R4, R1, #1      ;用交叉存储的方式交换数据
        STR     R3, R2, #1
        
Next    LDR     R1, R1, #0
        BRnzp   Cycle
        
    HEAD    .FILL x3100
    .END