#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

int main(int argc, char **argv) {
    static struct option arguments_long[] = {
        {"help",    no_argument,        NULL, 'h'},
        {"users",   no_argument,        NULL, 'u'},
        {"log",     optional_argument,  NULL, 'l'},
        {"errors",  optional_argument,  NULL, 'e'},
        {NULL,      0,                  NULL,  0 }
    };
    static const char *arguments_short = "hul:e:";
    


    return 0;
}
