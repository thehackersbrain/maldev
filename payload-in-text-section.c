#include <windows.h>
#include <stdio.h>

int main(void)
{
    void *emem;
    BOOL rv;
    HANDLE th;
    DWORD oprotect = 0;

    unsigned char payload[] = {
        0x90,
        0x90,
        0xcc,
        0xc3};

    unsigned int plen = 4;

    // payload memory buffer
    emem = VirtualAlloc(0, plen, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    printf("%-20s : 0x%-016p\n", "payload addr", (void *)payload);
    printf("%-20s : 0x%-016p\n", "exec_mem addr", (void *)emem);

    RtlMoveMemory(emem, payload, plen);

    rv = VirtualProtect(emem, plen, PAGE_EXECUTE_READ, &oprotect);

    printf("\nHit Any Key!\n");
    getchar();

    if (rv != 0)
    {
        th = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)emem, 0, 0, 0);
        WaitForSingleObject(th, -1);
    }

    return 0;
}