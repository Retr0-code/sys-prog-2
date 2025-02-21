#include "help.h"

int show_help(FILE *out, FILE *err) {
    fputs("Help Menu:\n", out);
    fprintf(out, "--help            -h          Shows help menu\n");
    fprintf(out, "--processes       -p          Shows running processes\n");
    fprintf(out, "--users           -u          Shows users and their home dirs\n");
    fprintf(out, "--log <file>      -l <file>   Redirects stdout to <file>\n");
    fprintf(out, "--errors <file>   -e <file>   Redirects stderr to <file>\n");
    return 0;
}

