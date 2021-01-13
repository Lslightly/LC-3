#   算法

*   辗转相减法
    *   通过不断的两数相减并将结果移给被减数，然后不断重复相减的过程，直到两数相等，则停止，两个数都是最大公约数
    *   若a > b,则(a, b) = (a-b, b)
    *   若a < b,则(a, b) = (a, -(a-b));
    *   若a = b,则最大公约数即a

#   写代码

```
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
Data0       .FILL x0018
Data1       .FILL x0020
            .END

```

1.  由算法,R0-R1是一个共同操作，如果R0-R1=0则已经找到结果，直接跳转到``HALT``，否则，判断R0-R1>0，如果成立，则将结果(储存在R2中)赋给R0，否则应将-R2赋给R1，所以整体看来，就是将差的绝对值赋给R0，R1中较大的那个

#   测试数据

Example
(3, 12)
![](../Pictures/(3,%2012).png)

反过来
(12, 3)
![](../Pictures/(12,3).png)

1,1较为特殊
(1, 1)
![](../Pictures/(1,1).png)

(1, 8)
![](../Pictures/(1,8).png)

两个偶数，且最大公约数不是这两个数
(24, 32)
![](../Pictures/(24,32).png)

一个偶数，一个奇数，但不互质
(33, 24)
![](../Pictures/(33,24).png)

两个奇数，互质
(5, 7)
![](../Pictures/(5,7).png)

一个偶数，一个奇数，互质
(5, 12)
![](../Pictures/(5,12).png)
