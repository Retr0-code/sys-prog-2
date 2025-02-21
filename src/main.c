#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

int show_help(FILE *out, FILE *err) {
    fprintf(out, "--help            -h          Shows help menu\n");
    fprintf(out, "--processes       -p          Shows running processes\n");
    fprintf(out, "--users           -u          Shows users and their home dirs\n");
    fprintf(out, "--log <file>      -l <file>   Redirects stdout to <file>\n");
    fprintf(out, "--errors <file>   -e <file>   Redirects stderr to <file>\n");
    return 0;
}

int show_users(FILE *out, FILE *err) {
    const char *passwd_path = "/etc/passwd";
    FILE *passwd = NULL;
    if ((passwd = fopen(passwd_path, "r")) == NULL) {
        fprintf(err, "Unable to open %s", passwd_path);
        return -1;
    }

    char *line = NULL;
    size_t len = 0;
    while ((getline(&line, &len, passwd)) != -1) {
        static const size_t fields_amount = 5;
        char *field = NULL;
        if ((field = strtok(line, ":")) == NULL)
                field = "(null)";

        for (size_t i = 0; i <= fields_amount; ++i) {
            switch (i) {
            case 0:
                if (field[0] == '\n')
                    ++field;

                fprintf(out, "%s ", field);
                break;

            case 5:
                size_t strend = strlen(field) - 1;
                field[strend] = field[strend] == '\n' ? '\0' : field[strend];
                fprintf(out, "%s\n", field);
                break;
            
            default:
                break;
            }
            if ((field = strtok(NULL, ":")) == NULL)
                field = "(null)";
        }
    }
    if (line != NULL)
        free(line);
    
    fclose(passwd);
    return 0;
}

int show_processes(FILE *out, FILE *err) {
    
}

int main(int argc, char **argv) {
    typedef int (*function_t)(FILE *out, FILE *err);

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
                fprintf(err, "Error parsing arguments\n");
                exit(-1);
                break;
        }
    }

    for (size_t i = 0; i < function_amount; ++i)
        (functions[i])(out, err);

    fclose(err);
    fclose(out);
    return 0;
}
