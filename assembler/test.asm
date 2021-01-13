
            .ORIG x3000
            LD R0, HEAD     ;   R0  head of the Link List
LOOP        ADD R2, R0, #0  ;   R2  p_pre = head
            LDR R3, R0, #0  ;   R3  p = head->next
            BRz DONE
            ADD R4, R3, #0  ;   R4  minp = p
            LDR R5, R3, #1  ;   R5  min = p->data
            ADD R1, R2, #0  ;   R1  minp_pre = p_pre
FindMin     ADD R3, R3, #0
            BRz DoneInner
            NOT R6, R5
            ADD R6, R6, #1  ;   R6  -min
            LDR R7, R3, #1  ;   R7  p->data
            ADD R6, R7, R6  ;   R6 = p->data-min
            BRzp EndIf
            ADD R5, R7, #0  ;   min = p->data
            ADD R4, R3, #0  ;   minp = p
            ADD R1, R2, #0  ;   minp_pre = p_pre
EndIf       LDR R3, R3, #0  ;   p = p->next
            LDR R2, R2, #0  ;   p_pre = p_pre->next
            BRnzp FindMin
DoneInner   LDR R5, R4, #0  ;   R5  minp->next
            STR R5, R1, #0  ;   minp_pre->next = minp->next
            LDR R1, R0, #0  ;   R1  head->next
            STR R1, R4, #0  ;   minp->next = head->next
            STR R4, R0, #0  ;   head->next = minp
            ADD R0, R4, #0  ;   head = minp
            BRnzp   LOOP
DONE        HALT
HEAD        .FILL x3100
            .END
