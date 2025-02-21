#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "help.h"
#include "users.h"
#include "processes.h"

typedef int (*function_t)(FILE *out, FILE *err);
struct function_option_t {
    function_t f_ptr;
    int f_enabled;
};

void try_add_function(size_t *amount, struct function_option_t *functions, function_t function_ptr) {
    if (functions[*amount].f_enabled)
        return ;

    functions[*amount].f_ptr = function_ptr;
    functions[*amount].f_enabled = 1;
    ++(*amount);
}

int main(int argc, char **argv) {
    static struct option arguments_long[] = {
        {"help",        no_argument,        NULL, 'h'},
        {"users",       no_argument,        NULL, 'u'},
        {"processes",   no_argument,        NULL, 'p'},
        {"log",         optional_argument,  NULL, 'l'},
        {"errors",      optional_argument,  NULL, 'e'},
        {NULL,          0,                  NULL,  0 }
    };
    static const char *arguments_short = "hupl:e:";
    int opt = 0, option_index = 0;

    struct function_option_t functions[3];
    size_t functions_amount = 0;
    FILE *out = stdout, *err = stderr;

    if (argc == 1) {
        show_help(out, err);
        exit(-1);
    }

    while ((opt = getopt_long(argc, argv, arguments_short, arguments_long, &option_index))) {
        if (opt == -1)
            break;

        switch (opt) {
            case 'h':
                try_add_function(&functions_amount, functions, &show_help);
                break;
            case 'u':
                try_add_function(&functions_amount, functions, &show_users);
                break;
            case 'p':
                try_add_function(&functions_amount, functions, &show_processes);
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
                perror("Error parsing arguments\n");
                exit(-1);
                break;
        }
    }

    int status = 0;
    for (size_t i = 0; i < functions_amount; ++i)
        status += (functions[i].f_ptr)(out, err);

    fclose(err);
    fclose(out);
    return status;
}
