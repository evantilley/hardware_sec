#include <string.h>

int main() {
    char* str = "Hello World\n";
    long len = strlen(str);
    int ret = 0;

    __asm__(
    "MOV R0, #2\n\t" // output to stderr
    "MOV R1, %[toPrint]\n\t" // load in message
    "MOV R2, %[length]\n\t" // load in length
    "MOV R7, #4\n\t"
    "SWI 0"
    :
    : [toPrint] "r" (str), [length] "r" (len)
);
    return 0;
}
