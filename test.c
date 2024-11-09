#include <stdio.h>

int main(int argc, char *argv[]) {
    char currChar = *"c";
    char str[] = {currChar};
    str + currChar;
    printf("%s\n", str);
}