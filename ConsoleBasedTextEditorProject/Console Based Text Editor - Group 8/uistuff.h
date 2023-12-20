#pragma once
// All other preprocessor directives and libraries that need to be included
#define LINECOUNTSIZE 5
#define NAMESIZE 200
#define TEXTSIZE 10000
#define FILECOUNT 50
#define EMPTYFILE 10020
#define FILELIST "Files.txt"
#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>
#include <malloc.h>
#include <Windows.h>
#include <time.h>
 // UI function declarations
void Mainmenu(void);
void displayfile(void);
void optionsmenu(void);