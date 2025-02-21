#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <dirent.h>
#include <linux/limits.h>

int show_help(FILE *out, FILE *err) {
    fputs("Help Menu:", out);
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
        fprintf(err, "Error: Unable to open %s", passwd_path);
        return -1;
    }

    char *line = NULL;
    size_t len = 0;
    fputs("USERNAME HOMEDIR", out);
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
    const char *proc_path = "/proc";
    DIR *proc = NULL;
    if ((proc = opendir(proc_path)) == NULL) {
        fprintf(err, "Error: Unable to open %s", proc_path);
        return -1;
    }

    char status_path[PATH_MAX];
    FILE *status_file = NULL;
    struct dirent *proc_entry = NULL;

    char *line = NULL;
    size_t len = 0;

    fputs("PID PROCESSNAME", out);
    while ((proc_entry = readdir(proc)) != NULL) {
        if (atol(proc_entry->d_name) == 0)
            continue;
        
        sprintf(status_path, "%s/%s/status", proc_path, proc_entry->d_name);
        if ((status_file = fopen(status_path, "r")) == NULL) {
            fprintf(err, "Error: Unable to open %s", status_path);
            closedir(proc);
            return -1;
        }

        if (getline(&line, &len, status_file) == -1) {
            fprintf(err, "Error: Unable to read %s", status_path);
            closedir(proc);
            fclose(status_file);
            return -1;
        }
        size_t prefix_len = strcspn(line, "\t") + 1;
        fprintf(out, "%s %s", proc_entry->d_name, line + prefix_len);
        fclose(status_file);
    }
    closedir(proc);
    return 0;
}

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
