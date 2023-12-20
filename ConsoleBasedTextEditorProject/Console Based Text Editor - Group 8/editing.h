#pragma once
// Preprocesor directives for special key inputs when editing
#define ENTER 13
#define ARROW 224
#define LEFT 75
#define RIGHT 77
#define SEPARATOR 219
#define SEPARATORTWO 32
#define BACKSPACE 8
#define HOTKEY 126

#include "uistuff.h"
#include "backend.h"
 // Function declarations when editing
int insertInStr(char* s, size_t position, char c);
void deleteSymbol(char* s, size_t position);
void printTextWithCursor(char* s, size_t position);
void editfile(char*);
