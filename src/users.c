#include "users.h"

#include <string.h>
#include <stdlib.h>

int show_users(FILE *out, FILE *err) {
    const char *passwd_path = "/etc/passwd";
    FILE *passwd = NULL;
    if ((passwd = fopen(passwd_path, "r")) == NULL) {
        fprintf(err, "Error: Unable to open %s", passwd_path);
        return -1;
    }

    char *line = NULL;
    size_t len = 0;
    fputs("USERNAME HOMEDIR\n", out);
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
