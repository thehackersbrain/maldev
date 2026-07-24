#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char payload[] = {
    0x90,
    0x90,
    0xcc,
    0xc3,
};

unsigned int plen = 4;

int main(void)
{
    void *emem;
    BOOL rv;
    HANDLE th;
    DWORD oprotect = 0;

    // memory buffer for payload
    emem = VirtualAlloc(0, plen, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    RtlMoveMemory(emem, payload, plen);

    rv = VirtualProtect(emem, plen, PAGE_EXECUTE_READ, &oprotect);

    printf("\nHit any key!\n");
    getchar();

    if (rv != 0)
    {
        th = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)emem, 0, 0, 0);
        WaitForSingleObject(th, -1);
    }

    return 0;
}