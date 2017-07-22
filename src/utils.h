/*
 * --------------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tomenglund26@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Tom Englund
 * --------------------------------------------------------------------------------
 */

#include <stdbool.h>

char *str_combine(char *str1, char *str2);
bool str_contains(char *str, char *occ);
bool str_empty(char *str);
void str_remove_spaces(char *restrict trim, const char *restrict untrim);
char **str_split(char *str, const char *delim, int max_splits, int *num_splits);
bool str_startswith(const char *str, const char *prefix);
void safefree(void **pp);
