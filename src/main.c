#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

void show_help(FILE *out, FILE *err) {
    puts("help");
}

void show_users(FILE *out, FILE *err) {
    puts("users");
}

void show_processes(FILE *out, FILE *err) {
    puts("proc");
}

int main(int argc, char **argv) {
    typedef void (*function_t)(FILE *out, FILE *err);

    static struct option arguments_long[] = {
        {"help",        no_argument,        NULL, 'h'},
        {"users",       no_argument,        NULL, 'u'},
        {"processes",   no_argument,        NULL, 'p'},
        {"log",         optional_argument,  NULL, 'l'},
        {"errors",      optional_argument,  NULL, 'e'},
        {NULL,          0,                  NULL,  0 }
    };
    static const char *arguments_short = "hul:e:";
    int opt = 0, option_index = 0;

    function_t functions[3];
    size_t function_amount = 0;
    FILE *out = stdout, *err = stderr;

    while ((opt = getopt_long(argc, argv, arguments_short, arguments_long, &option_index))) {
        if (opt == -1)
            break;

        switch (opt) {
            case 'h':
                functions[function_amount] = &show_help;
                ++function_amount;
                break;
            case 'u':
                functions[function_amount] = &show_users;
                ++function_amount;
                break;
            case 'p':
                functions[function_amount] = &show_processes;
                ++function_amount;
                break;
            case 'l':
                if ((out = fopen(optarg, "w")) == NULL) {
                    perror("Error opening log file");
                    exit(-1);
                }
                break;
            case 'e':
                if ((err = fopen(optarg, "w")) == NULL) {
                    perror("Error opening errors file");
                    exit(-1);
                }
                break;
            default:
                break;
        }
    }

    for (size_t i = 0; i < function_amount; ++i)
        (functions[i])(out, err);

    return 0;
}
