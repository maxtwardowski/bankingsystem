#ifndef __STRINGTOOLS_H__
#define __STRINGTOOLS_H__

int safestringinput(char * str, size_t size);
void cleanbuffer();
bool isnumerical(char * string);
bool isletter(char * string);
void remspaces(char* source);
bool isfloat(char * string);

#endif
