#include <stdio.h>
#include <stdlib.h>

char getch()
{
    char c;
    system("stty -echo");
    system("stty -icanon");
    c = getchar();
    system("stty icanon");
    system("stty echo");
    return c;
}

int main(void)
{
    char ch = getch();
    putchar('\n');
    putchar(ch);
    return 0;
}