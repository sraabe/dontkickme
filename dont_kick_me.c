#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>

#ifndef NULL
#define NULL ((void *)0)
#endif
#define MAX_PROCS 32

BOOL CALLBACK enum_xiv_windows(
    HWND hWnd,
    LPARAM lParam);
void update_ts(char **);

struct xiv_proc
{
    long unsigned int pid;
    HWND hWnd;
};
struct xiv_proc xiv_procs[MAX_PROCS];
char *ts_s;

unsigned int i = 0;

const char *ffxiv_window_name = "FINAL FANTASY XIV";
char keys[5] = {
    0x57,
    0x53,
    0x41,
    0x44,
    0x20};

BOOL CALLBACK enum_xiv_windows(
    HWND hWnd,
    LPARAM lParam)
{
    int title_length = GetWindowTextLength(hWnd);
    if (title_length == 0)
        return 1;

    char *buf = malloc(title_length + 1);
    GetWindowText(hWnd, buf, title_length + 1);

    if (strcmp(buf, ffxiv_window_name) == 0)
    {
        xiv_procs[i].hWnd = hWnd;
        GetWindowThreadProcessId(hWnd, &xiv_procs[i].pid);
        fprintf(stdout, "Found window #%d [pid:%lu].\n", i++, xiv_procs[i].pid);
    }
    return 1;
}

char *get_current_timestamp()
{
    struct timespec tv;
    clock_gettime(CLOCK_REALTIME, &tv);

    char time_str[127];
    double fractional_seconds;
    int milliseconds;
    struct tm *tm;
    char *rfc8601;

    rfc8601 = malloc(256);

    tm = localtime(&tv.tv_sec);

    fractional_seconds = (double)tv.tv_nsec;
    fractional_seconds /= 1e6;
    fractional_seconds = round(fractional_seconds);
    milliseconds = (int)fractional_seconds;

    strftime(time_str, sizeof(time_str), "%Y-%m-%dT%H:%M:%S", tm);
    sprintf(rfc8601, "%s.%03d", time_str, milliseconds);

    return rfc8601;
}

int main(int argc, char **argv)
{
    time_t t;
    int sleep_ms, k;
    srand((unsigned)time(&t));
    EnumWindows(enum_xiv_windows, 0);

    do
    {
        for (int j = 0; j < i; ++j)
        {
            k = rand() % 5;
            SendMessage(xiv_procs[j].hWnd, WM_KEYDOWN, keys[k], 0x00010000);
            sleep_ms = rand() % 150 + 150;
            Sleep(sleep_ms);
            SendMessage(xiv_procs[j].hWnd, WM_KEYUP, keys[k], 0x00010003);
            free(ts_s);
            ts_s = get_current_timestamp();
            fprintf(stderr, "[%s] Sent keypress \"%c\" to window #%d [pid:%lu].\n", ts_s, keys[k], j, xiv_procs[j].pid);
        }

        sleep_ms = ((int)(((float)rand()/(float)(RAND_MAX)) * 900000)) + 600000;
        free(ts_s);
        ts_s = get_current_timestamp();
        fprintf(stderr, "[%s] Now sleeping for %d seconds..\n", ts_s, sleep_ms / 1000);
        Sleep(sleep_ms);
    } while (1);

    return 0;
}
