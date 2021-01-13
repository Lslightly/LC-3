#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>


void lab02(void);

void lab03(void);

void lab04(void);

void lab05(void);

void OutRow(int rocksA, int rocksB, int rocksC);

void Prompt(int turn);

void Input(int *rocksA, int *rocksB, int *rocksC, int *turn);

int Check(int rocksA, int rocksB, int rocksC, int turn);

int main(void)
{
    printf("lab02:\n");
    lab02();

    printf("\nlab03:\n");
    lab03();

    printf("\nlab04:\n");
    lab04();

    printf("\nlab05:\n");
    lab05();

    return 0;
}

void lab02()
{
    int R0, R1, R2;
    printf("input R0:");
    scanf("%d", &R0);
    printf("input R1:");
    scanf("%d", &R1);
    do
    {
        R2 = R0 - R1;
        if (R2 > 0)
            R0 = R2;
        else
            R1 = -R2;
    } while (R2 != 0);  //  R2 == 0为辗转相减法算法的结束
    printf("%d\n", R0);
}

typedef struct LNode
{
    int i;
    struct LNode *next;
    int value;
} LNode, *LinkList;

void lab03(void)
{
    int num = 0;
    printf("input the number of nodes you want to create:");
    scanf("%d", &num);
    LinkList *List;
    List = (LinkList *)malloc((num+1) * sizeof(LinkList));
    for (int i = 0; i <= num; i++)
    {
        List[i] = (LinkList)malloc(sizeof(*List[i]));
    }
    List[0] = NULL;
    printf("input the value and next node index(1 node must be the first node of the linklist)\n");
    for (int i = 1; i <= num; i++)
    {
        printf("the %d node:", i);
        int next = 0;
        scanf("%x%d", &List[i]->value, &next);
        getchar();
        List[i]->next = List[next];
        List[i]->i = i;
    }

    LinkList HEAD = (LinkList)malloc(sizeof(*HEAD));
    HEAD->next = List[1];
    LinkList head = HEAD;
    LinkList p, p_pre, minp, minp_pre;
    int min;
    while (head->next)
    {
        p_pre = head;
        p = head->next;
        minp = p;
        minp_pre = p_pre;
        min = p->value;
        while (p)
        {
            if (p->value < min)
            {
                min = p->value;
                minp = p;
                minp_pre = p_pre;
            }
            p = p->next;
            p_pre = p_pre->next;
        }
        minp_pre->next = minp->next;
        minp->next = head->next;
        head->next = minp;
        head = minp;
    }

    printf("value\tnode index\n");
    p = HEAD->next;
    while (p)
    {
        printf("%x\t%d\n", p->value, p->i);
        p = p->next;
    }
    return;
}

void lab04()
{
    int turn = 0;
    int checkresult = 0;
    int rocksA = 3, rocksB = 5, rocksC = 8;
    while (!checkresult)
    {
        OutRow(rocksA, rocksB, rocksC);
        Input(&rocksA, &rocksB, &rocksC, &turn);
        checkresult =  Check(rocksA, rocksB, rocksC, turn);
    }
    return ;
}

void OutRow(int rocksA, int rocksB, int rocksC)
{
    printf("RowA: ");
    for (int i = 0; i < rocksA; i++)
    {
        putchar('o');
    }
    putchar('\n');
    printf("RowB: ");
    for (int i = 0; i < rocksB; i++)
    {
        putchar('o');
    }
    putchar('\n');
    printf("RowC: ");
    for (int i = 0; i < rocksC; i++)
    {
        putchar('o');
    }
    putchar('\n');
    return ;
}

void Prompt(int turn)
{
    if (turn)
    {
        printf("Player 2, choose a row and number of rocks:");
    }
    else
    {
        printf("Player 1, choose a row and number of rocks:");
    }
}

void Input(int *rocksA, int *rocksB, int *rocksC, int *turn)
{
    int Invalidflag = 1;
    char row, number;
    while (Invalidflag)
    {
        Invalidflag = 0;
        Prompt(*turn);
        row = getchar();
        number = getchar();
        getchar();
        number = number-'0';
        if (row < 'A' || row > 'C')
        {
            Invalidflag = 1;
        }
        if (row == 'A')
        {
            if (number > 0 && number <= *rocksA)
                *rocksA -= number;
            else
                Invalidflag = 1;
        }
        else if (row == 'B')
        {
            if (number > 0 && number <= *rocksB)
                *rocksB -= number;
            else
                Invalidflag = 1;
        }
        else if (row == 'C')
        {
            if (number > 0 && number <= *rocksC)
                *rocksC -= number;
            else
                Invalidflag = 1;
        }
        if (Invalidflag)
        {
            printf("Invalid move. Try again.");
        }
        putchar('\n');
    }
    *turn = 1-*turn;

}

int Check(int rocksA, int rocksB, int rocksC, int turn)
{
    if (rocksA == 0 && rocksB == 0 && rocksC == 0)
    {
        if (turn == 0)
            printf("Player 1 Wins.\n");
        else
            printf("Player 2 Wins.\n");
        return 1;
    }
    return 0;
}


int kbhit()
{
    struct termios oldt, newt;
    int oldf;
    int ch;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

void delay(void)
{
    int count = 100000000;
    while (count != 0)
    {
        --count;
    }
    return;
}

void lab05()
{
    char ch;
    while (1)
    {
        system("stty -echo");
        while (!kbhit())
        {
            printf("ICS2020 ");
            fflush(stdout);
            if (kbhit())
                break;
            delay();
        }
        putchar('\n');
        ch = getchar();
        if (ch <= '9' && ch >= '0')
            printf("%c is a decimal digit\n", ch);
        else
            printf("%c is not a decimal digit\n", ch);
    }
}


