
#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    uint8_t code[] = {
        0x55,                                     // push   rbp
        0x48, 0x81, 0xec, 0x50, 0x00, 0x00, 0x00, // sub    rsp,0x50
        0x48, 0x8d, 0x6c, 0x24, 0x20,             // lea    rbp,[rsp+0x20]
        0xb8, 0x01, 0x00, 0x00, 0x00,             // mov    eax,0x1
        0xb9, 0x02, 0x00, 0x00, 0x00,             // mov    ecx,0x2
        0x03, 0xc1,                               // add    eax,ecx
        0xb9, 0x03, 0x00, 0x00, 0x00,             // mov    ecx,0x3
        0xf7, 0xe9,                               // imul   ecx
        0x89, 0x44, 0x25, 0x00,                   // mov    DWORD PTR [rbp+riz*1+0x0],eax
        0xb8, 0x02, 0x00, 0x00, 0x00,             // mov    eax,0x2
        0xb9, 0x03, 0x00, 0x00, 0x00,             // mov    ecx,0x3
        0x8b, 0x54, 0x25, 0x00,                   // mov    edx,DWORD PTR [rbp+riz*1+0x0]
        0x0f, 0xaf, 0xca,                         // imul   ecx,edx
        0x03, 0xc1,                               // add    eax,ecx
        0x89, 0x44, 0x25, 0x04,                   // mov    DWORD PTR [rbp+riz*1+0x4],eax
        0x8b, 0x44, 0x25, 0x04,                   // mov    eax,DWORD PTR [rbp+riz*1+0x4]
        0xb9, 0x64, 0x00, 0x00, 0x00,             // mov    ecx,0x64
        0x03, 0xc1,                               // add    eax,ecx
        0x89, 0x44, 0x25, 0x00,                   // mov    DWORD PTR [rbp+riz*1+0x0],eax
        0x48, 0x81, 0xc4, 0x50, 0x00, 0x00, 0x00, // add    rsp,0x50
        0x5d,                                     // pop    rbp
        0xc3                                      // ret
    };

    size_t buffer_size = sizeof(code);

    uint8_t* buffer = (uint8_t*)VirtualAlloc(NULL, buffer_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    if (buffer) {

        memcpy(buffer, code, buffer_size);

        DWORD old_protect = 0;

        VirtualProtect(buffer, buffer_size, PAGE_EXECUTE_READ, &old_protect);

        int(*func)(void) = (int(*)(void))buffer;

        printf("func() == %d\n", func());

        VirtualFree(buffer, 0, MEM_RELEASE);

        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}

