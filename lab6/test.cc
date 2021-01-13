#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

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
    ch = getch();
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

int main(void)
{
    char ch;
    while (1)
    {
        system("stty -echo");
        while (!kbhit())
        {
            system("stty echo");
            printf("ICS2020 ");
            fflush(stdout);
            system("stty -echo");
            if (kbhit())
                break;
            delay();
        }
        putchar('\n');
        ch = getch();
        if (ch <= '9' && ch >= '0')
            printf("%c is a decimal digit\n", ch);
        else
            printf("%c is not a decimal digit\n", ch);
    }
    return 0;
}