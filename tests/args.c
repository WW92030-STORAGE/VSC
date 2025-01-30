// C program named mainreturn.c to demonstrate the working
// of command line arguement
#include <stdio.h>

// defining main with arguments
int main(int argc, char** argv)
{
    printf("You have entered %d arguments:\n", argc);

    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }
    return 0;
}

// when you do this you say ./args <arg1> <arg2> ...
// the args get transferred into the main method in the argv array of C strings. 
// argc gives the number of useful things in argv. If you manually transfer N arguments ... the argc shows N + 1. 
// The first (0th) index has the path of the executable. The value argv[argc] is NULL.