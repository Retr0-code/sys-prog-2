#include "processes.h"

#include <stdlib.h>
#include <string.h>

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

    fputs("PID PROCESSNAME\n", out);
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
