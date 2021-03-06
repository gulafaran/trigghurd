/*
 * --------------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tomenglund26@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Tom Englund
 * --------------------------------------------------------------------------------
 */

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utils.h"

void str_add_to(char *str, const char *prefix, int dest) {
  if(str == NULL || prefix == NULL || dest < 0) {
    return;
  }

  for(int i = 0; i < strlen(prefix); i++) {
    str[i + dest] = prefix[i];
  }
}

char *str_combine(char *str1, char *str2) {
  if(str1 == NULL || str2 == NULL) {
    return NULL;
  }

  size_t l = (strlen(str1) + strlen(str2) + 1);
  char *str = malloc(l);

  strcpy(str, str1);
  strcat(str, str2);

  return str;
}

bool str_contains(char *str, char *occ) {
  if(str != NULL && occ != NULL) {
    if (strstr(str, occ) != NULL) {
      return true;
    }
  }

  return false;
}

bool str_empty(char *str) {
  if(str == NULL) {
    return true;
  }

  char buffer[strlen(str)];
  bool rval = false;

  memset(buffer, 0, sizeof(buffer));
  str_add_to((char *)&buffer, str, 0);
  str_remove_spaces((char *)&buffer, str);

  if(strlen(buffer) == 0) {
    rval = true;
  }

  return rval;
}

void str_remove_spaces(char *restrict trim, const char *restrict untrim) {
  if(trim == NULL || untrim == NULL) {
    return;
  }

  while(*untrim != '\0') {
    if(!isspace(*trim)) {
      *trim = *untrim;
      trim++;
    }

  untrim++;
  }

  *trim = '\0';
}

char **str_split(char *str, const char *delim, int max_splits, int *num_splits) {
  if(str_empty(str)) {
    if(num_splits != NULL) {
      *num_splits = 0;
    }

    return NULL;
  }

  int found = 0;
  char **ret = malloc(max_splits * sizeof(char *));
  char *saveptr;
  char tok[strlen(str) + 1];
  memset(tok, 0, sizeof(tok));
  str_add_to((char *)&tok, str, 0);
  char *tmp = strtok_r((char *)&tok, delim, &saveptr);

  while(tmp != NULL && found < max_splits) {
    ret[found] = strdup(tmp);
    tmp = strtok_r(NULL, delim, &saveptr);
    found++;
  }
  *num_splits = found;
  return ret;
}

bool str_startswith(const char *str, const char *prefix) {
  size_t str_len = strlen(str);
  size_t p_len = strlen(prefix);

  return str_len < p_len ? false : strncmp(str, prefix, p_len) == 0;
}

void safefree(void **pp) {
  assert(pp);
  if(pp != NULL) {
    free(*pp);
    *pp = NULL;
  }
}
