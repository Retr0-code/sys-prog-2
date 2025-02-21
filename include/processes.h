#ifndef PROCESSES_H
#define PROCESSES_H

#include <stdio.h>
#include <dirent.h>
#include <linux/limits.h>

// Lists processes' IDs accending and processes' names
int show_processes(FILE *out, FILE *err);

#endif
