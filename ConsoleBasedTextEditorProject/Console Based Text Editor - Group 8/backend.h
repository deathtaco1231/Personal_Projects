#pragma once
#include "uistuff.h"

typedef struct FileDirShell { // struct for a copy of the file directory
	char files[FILECOUNT][NAMESIZE];
}DIRCOPY;
// All backend function declarations
void createfiledirectory(void);
void getnameinput(char*);
int getmenuinput(void);
void printfiles(void);
void createfile(void);
void programexit(void);
bool overwriteprotect(char*);
bool addfiletolist(char*);
void deletefile(void);
bool removefilefromlist(char*);
void openfile(void);
void removenewline(char*);
int getlinecount(FILE*);
void savefile(char*, char**, int);
void randomfilename(char*);

