#include <windows.h>
#include <stdio.h>
#include <time.h>

HWND hWnd;
time_t t;
int i;
int sleep_ms;

LPCSTR ffxiv_window_name = "FINAL FANTASY XIV";
char keys[5] = {
    0x57,
    0x53,
    0x41,
    0x44,
    0x20
};

int main(int argc, char **argv)
{
    hWnd = FindWindow(NULL, ffxiv_window_name);
    srand((unsigned) time(&t));

    do
    {
        i = rand() % 5;
        SendMessage(hWnd, WM_KEYDOWN, keys[i], 0x00000000 + ((rand() % 0x100 + 0xff) << 16));
        Sleep(rand() % 50 + 150);
        SendMessage(hWnd, WM_KEYUP, keys[i], 0x00010003);
        sleep_ms = (rand() % 40 + 20) * 1000;
        fprintf(stdout, "Sent keypress \"%c\", now sleeping for %d seconds..\n", keys[i], sleep_ms / 1000);
        Sleep(sleep_ms);
    } while (1);

    return(0);
}
