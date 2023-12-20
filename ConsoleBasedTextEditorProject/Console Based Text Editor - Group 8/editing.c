#include "editing.h"

void deleteSymbol(char* s, size_t position)
{
    size_t i, len = strlen(s);

    for (i = position; i < len; i++) // Shift each character over by 1 after deleting character at current position
        s[i] = s[i + 1];
}
int insertInStr(char* s, size_t position, char c)
{
    size_t i;
    if (position == 0) // Inserting character in brand new/empty string
    {
        (s)[0] = c;
        s[1] = '\0';
    }
    else
        for (i = strlen(s) + 1; i >= position; i--) // Shift each character after current positon over by 1 to make room for insertion
            (s)[i] = i == position ? c : (s)[i - 1];
    return 1;
}
void printTextWithCursor(char* s, size_t position)
{
    size_t currPos, length = strlen(s);

    for (currPos = 0; currPos <= length; currPos++)
    {
        putchar(currPos < position ? s[currPos] : currPos == position ? SEPARATOR : s[currPos - 1]);// Moves seperator to current cursoor position and swaps letter accordingly so it is still visible
    }
    putchar(SEPARATOR); // Places seperator once letters are moved
}
void editfile(char* name)
{
    FILE* fp = fopen(name, "r");
    int maxsel = getlinecount(fp); // The max amount of array elements to be accessed with the current number of lines in the file
    size_t* pos = malloc(sizeof(size_t) * (maxsel + 1)); // Array of size_t
    char** s = malloc(sizeof(char*) * (maxsel + 1)); // Dynamic array of char*'s
    if (s == NULL || pos == NULL) // Checking mallocs
        exit(1);
    system("@cls||clear"); // This clears the screen when called
    char line[TEXTSIZE]; // Temp location for each line read from file
    printf("\nPress enter to start new line/paragraph. Traverse using the left and right arrow keys.\nEnsure you are at the end of a line to start a new line.\nTo access options, press ~ and click enter.\n\n");
    for (int i = 0; i <= maxsel; i++)
    {
        fgets(line, TEXTSIZE, fp);
        if (i == maxsel) // Remove new line before printing so cursor stays on last line printed
            removenewline(line);
        if (maxsel == 0 && strlen(line) == EMPTYFILE) // Correctly formats and displays brand new file with no contents
        {
            s[i] = malloc(sizeof(char) + 1);
            if (s[i] == NULL)
                exit(1);
            strcpy(s[i], "");
        }
        else
        {
            s[i] = malloc(sizeof(char) * strlen(line) + 1); // Allocates memory for each line grabbed from file
            if (s[i] == NULL) // Malloc error checking
                exit(1);
            printf("%s", line); // Display each line as printed
            removenewline(line); // Remove new line before adding to the editing buffer
            strcpy(s[i], line);
        }
        pos[i] = strlen(s[i]); // Set position value corrosponding to each line
    }
    if (fp)
        fclose(fp);
    unsigned char c;
    int sel = maxsel;
    while ((c = _getch()) != HOTKEY) // Gets each keyboard press, breaks this loop if HOTKEY value entered
    {
        putchar('\r');
        switch (c)
        {
        case ARROW: // Also works with delete
            c = _getch();
            if (c == LEFT)
            {
                if (pos != NULL && pos[sel] > 0) // Decrements position value for current line if position value greater than 0 (going below 0 would result in a crash trying to read -1 from the buffer)
                    pos[sel]--;
                else if (sel != 0) // Moves to end of previous line so long as current line is not the first line in the file
                {
                    sel--;
                    system("@cls||clear");
                    for (int i = 0; i <= sel; i++) // Print all lines up to and including new current line being edited
                    {
                        printf(s[i]);
                        if (i != sel)
                            printf("\n");
                    }
                    putchar('\r');
                }
            }
            if (c == RIGHT)
            {
                if (pos != NULL && pos[sel] < strlen(s[sel])) // Incriments current position value if position value isnt already at the end of the current line
                    pos[sel]++;
                else if (sel != maxsel) // Moves to beginning of next line if current line isnt the last line currently loaded
                {
                    sel++;
                    system("@cls||clear");
                    for (int i = 0; i <= sel; i++) // Print all lines up to and including new line being edited
                    {
                        printf(s[i]);
                        if (i != sel)
                            printf("\n");
                    }
                    putchar('\r');
                }
            }
            break;
        case BACKSPACE:
            if (pos[sel] > 0) // If position is not at the beginning of the line (with no characters to delete before the cursor)
                deleteSymbol(s[sel], --pos[sel]);
            break;
        case ENTER:
            if (pos[sel] == strlen(s[sel])) // Must be at the end of the current line to add a new line 
            {
                system("@cls||clear");
                if (sel == maxsel) // Reallocates memory if max number of lines is reached, so line count is not limited
                {
                    char** tmpbuf = realloc(s, sizeof(char*) * (++maxsel + 1)); // Reallocs to tmp buffer for both to check for errors and prevent memory leakage 
                    size_t* tmppos = realloc(pos, sizeof(size_t) * (maxsel + 1));
                    if (tmppos == NULL || tmpbuf == NULL)
                        exit(1);
                    s = tmpbuf; // If both successful, change original pointers to new pointers
                    pos = tmppos;
                }
                sel++;
                s[sel] = malloc(sizeof(char) + 1); // Allocate memory for new char* (each individual line is its own char* so new char* needs new memory)
                if (s[sel] == NULL)
                    exit(1);
                strcpy(s[sel], "\0"); // Initalize new line
                pos[sel] = 0; // Set new line's position to 0
                for (int i = 0; i <= sel; i++)
                {
                    printf(s[i]); // Reprint file data including new line created 
                    if (i != sel)
                        printf("\n");
                }
                putchar('\r');
            }
            break;
        default:
            if (pos[sel] >= 0 && pos[sel] <= strlen(s[sel])) // Checks that position is not a value out of range (if data error occured before this point)
            {
                char* tmp = realloc(s[sel], sizeof(char) * strlen(s[sel]) + 2); // Reallocate current string buffer to make room for the new character to be added
                if (tmp == NULL)
                    exit(1);
                s[sel] = tmp;
                insertInStr(s[sel], pos[sel]++, c);
            }
        }
        if (c != ENTER) // New line has no data to be printed so do not call function if enter was pressed
            printTextWithCursor(s[sel], pos[sel]);
    }
    bool run = true;
    while (run) // If hotkey pressed this loop will run
    {
        optionsmenu();
        int choice = getmenuinput();
        switch (choice) {
        case 1:
        {
            savefile(name, s, maxsel); // Calls function to save updated contents to file
            run = false; // Breaks loop
            break;
        }
        case 2:
        {
            run = false; // Just breaks loop, does not save updated file data
            break;
        }
        default: // User must enter a valid choice
        {
            printf("\nPlease enter one of the presented options.");
            break;
        }
        }
    }
    system("@cls||clear");
    for (int i = 0; i <= maxsel; i++)
        free(s[i]); // Free each char* used, then free pointer to char* array and position array
    free(s);
    free(pos);
}