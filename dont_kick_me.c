#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#ifndef NULL
#define NULL ((void *) 0)
#endif

HWND hWnd;
time_t t, ts;
char *ts_s;
int i, sleep_ms;
long unsigned int *pid;

const char *ffxiv_window_name = "FINAL FANTASY XIV";
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
    pid = malloc(sizeof(long unsigned int));

    GetWindowThreadProcessId(hWnd, pid);
    srand((unsigned) time(&t));

    do
    {
        i = rand() % 5;
        SendMessage(hWnd, WM_KEYDOWN, keys[i], 0x00000000 + ((rand() % 0x100 + 0xff) << 16));
        Sleep(rand() % 50 + 150);
        SendMessage(hWnd, WM_KEYUP, keys[i], 0x00010003);
        sleep_ms = (rand() % 600 + 600) * 1000;
        ts = time(NULL);
        ts_s = ctime(&ts);
        ts_s[strlen(ts_s) - 1] = 0;
        fprintf(stdout, "[%s] [pid:%lu] Sent keypress \"%c\", now sleeping for %d seconds..\n", ts_s, *pid, keys[i], sleep_ms / 1000);
        Sleep(sleep_ms);
    } while (1);

    return(0);
}
