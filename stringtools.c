#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "stringtools.h"

int safestringinput(char * str, size_t size) {
    //returns 1 if overflow occured, otherwise 0 if no chars had to be truncated
    bool overflowoccured = false;
    fgets(str, size, stdin);
    str[strcspn(str, "\n")] = '\0';
    if (strlen(str) == size - 1) {
        cleanbuffer();
        overflowoccured = true;
    }
    fflush(stdin);
    if (overflowoccured)
        return 1;
    else
        return 0;
}

void cleanbuffer() {
    //cleans the stdin buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

bool isnumerical(char * string) {
    //returns true if the string consists of digits only
    char * ptr = string;
    while (* ptr != '\0') {
        if (* ptr < '0' || * ptr > '9')
            return false;
        ptr++;
    }
    return true;
}

bool isletter(char * string) {
    //returns true if string consists of letters and spaces only
    char * ptr = string;
    while (* ptr != '\0') {
        if ((* ptr < 'A' && * ptr != ' ') || (* ptr > 'Z' && * ptr < 'a') || * ptr > 'z')
            return false;
        ptr++;
    }
    return true;
}

void remspaces(char* source) {
    //removes spaces from the string
    char * i = source,
         * j = source;
    while(*j != 0) {
        *i = *j++;
        if(*i != ' ')
          i++;
    }
    *i = 0;
}

bool isfloat(char * string) {
    //returns true if the input is a float, otherwise false
    int decimalplaces = 0;
    bool decimal = false;
    char * ptr = string;
    while (* ptr != '\0') {
        if ((* ptr < '0' && * ptr != '.') || * ptr > '9' || decimalplaces > 2)
            return false;
        if (* ptr == '.')
            decimal = true;
        if (decimal)
            decimalplaces++;
        ptr++;
    }
    return true;
}
