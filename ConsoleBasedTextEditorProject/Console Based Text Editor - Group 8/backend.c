#include "backend.h"
#include "editing.h"
void getnameinput(char* name)
{
	fgets(name, NAMESIZE, stdin); // Adds user input to given buffer
	removenewline(name);
}
void removenewline(char* buffer) // removes newline to prevent errors reading and displaying data
{
	for (int i = 0; i < strlen(buffer); i++)
		if (buffer[i] == '\n') // Remove newline at end of buffer when found
			buffer[i] = '\0';
}
void createfiledirectory(void)
{
	FILE* fp = fopen(FILELIST, "a");
	for (int i = 0; i < FILECOUNT; i++) // Create FILECOUNT number of empty slots in new file directory
	{
		fprintf(fp, "EMPTY\n");
	}
	if (fp)
		fclose(fp);
}
int getmenuinput(void)
{
	char in[NAMESIZE];
	fgets(in, NAMESIZE, stdin);
	int num = atoi(in); // Converts user input into number, or -1 if number is not entered
	return num;
}
void printfiles(void)
{
	FILE* fp = fopen(FILELIST, "r");
	int filenum = 1; // Num used to make displaying list of files easier to read for user
	char filename[NAMESIZE];
	printf("\nFiles listed below:\n");
	for (int i = 0; i < FILECOUNT; i++) // Display each file that is not an empty slot
	{
		fgets(filename, NAMESIZE, fp);
		removenewline(filename);
		if (strcmp(filename, "EMPTY") != 0)
			printf("%d: %s\n", filenum++, filename); // Incriments displayed counter each time it is used
	}
	if (fp)
		fclose(fp);
}
void createfile(void)
{
	printf("\nEnter name of new file (type .txt at the end of the name to make it a notepad text file) For a random name simply press enter: ");
	char name[NAMESIZE];
	getnameinput(name); // Get name if new file from user
	if (strcmp(name, "\0") == 0)
		randomfilename(name); // Create random nameif no name is entered
	if (overwriteprotect(name) == false) // Checks if file already exists with the name given by user
		printf("\nFile with that name already exists. Please delete existing file before creating new file with this name.");
	else
	{
		if (addfiletolist(name) == false) // Addsfile to list, but displays error message if false is returned
			printf("\nFile limit of %d reached. Please delete some files in the delete menu to create a new file.", FILECOUNT);
		else
		{
			FILE* fp = fopen(name, "w"); // If file was sucessfully added, create and format new file
			fprintf(fp, "%d", 0);
			fclose(fp);
		}
	}
}
bool overwriteprotect(char* name)
{
	FILE* fp = fopen(FILELIST, "r");
	for (int i = 0; i < FILECOUNT; i++) // Compare each file in directory to file name given
	{
		char filecmp[NAMESIZE];
		fgets(filecmp, NAMESIZE, fp);
		removenewline(filecmp);
		if (strcmp(filecmp, name) == 0) // If match is found, return false
			return false;
	}
	return true; // If no match found, return true
}
void programexit(void)
{
	printf("Program exiting.");
}
bool addfiletolist(char* name)
{
	char filename[NAMESIZE];
	DIRCOPY fileduplicate; // Dircopy struct used to house duplicate of file directory
	int num = -1;
	FILE* fp = fopen(FILELIST, "r");
	for (int i = 0; i < FILECOUNT; i++)
	{
		fgets(filename, NAMESIZE, fp);
		strcpy(fileduplicate.files[i], filename); // Adds each file name from old directory to new directory
		removenewline(filename);
		if (strcmp(filename, "EMPTY") == 0) // Mark an empty slot to be used for housing the name of the new file
			num = i;
	}
	if (fp)
		fclose(fp);
	if (num == -1)
		return false; // Return false if file directory is full (a file needs to be deleted before new ones can be added)
	FILE* fp2 = fopen(FILELIST, "w");
	for (int i = 0; i < FILECOUNT; i++)
	{
		if (num == i) // Set empty slot to new file name
		{
			fprintf(fp2, name);
			fprintf(fp2, "\n");
		}
		else
			fprintf(fp2, fileduplicate.files[i]); // All other slots remain unchanged
	}
	if (fp2)
		fclose(fp2);
	return true;
}
void deletefile(void)
{
	printf("\nEnter name of file to be deleted: ");
	char name[NAMESIZE];
	getnameinput(name);
	if (removefilefromlist(name) == false) // If false, file not found with the name given by user
		printf("\nFile with that name not found.");
	else
	{
		removenewline(name);
		if (remove(name) != 0) // Will return something other than 0 if error occurs during deletion, so file may still exist but be inaccessible by the program
			printf("\nFile deletion error. File removed from list but may need to be deleted manually.");
		else
			printf("\nFile deleted successfully.");
	}
}
bool removefilefromlist(char* name)
{
	char filename[NAMESIZE];
	DIRCOPY fileduplicate; // Use of struct for duplicate of file directory
	bool changemade = false;
	FILE* fp = fopen(FILELIST, "r");
	for (int i = 0; i < FILECOUNT; i++) // Copy each name from file directory into duplicate
	{
		fgets(filename, NAMESIZE, fp);
		strcpy(fileduplicate.files[i], filename);
		removenewline(filename);
		if (strcmp(filename, name) == 0) // If the name to be deleted is the same as the one just grabbed from the file directory, replace it with EMPTY
		{
			strcpy(fileduplicate.files[i], "EMPTY\n");
			changemade = true; // Mark that a change was made
		}
	}
	if (fp)
		fclose(fp);
	if (changemade == false)
		return false; // Return false if no change was made to the file directory
	FILE* fp2 = fopen(FILELIST, "w");
	for (int i = 0; i < FILECOUNT; i++)
	{
		fprintf(fp2, fileduplicate.files[i]); // Make new file directory with updated information
	}
	if (fp2)
		fclose(fp2);
	return true;
}
void openfile(void)
{
	printf("\nEnter the name of the file you wish to open:");
	char name[TEXTSIZE];
	getnameinput(name);
	if (overwriteprotect(name) == true) // If overwrite protect returns true, no file exists with name given so cannot proceed
		printf("\nFile not found with that name.");
	else
		editfile(name); // If file does exist, begin editing

}
int getlinecount(FILE* fp)
{
	char count[LINECOUNTSIZE];
	fgets(count, LINECOUNTSIZE, fp);
	removenewline(count);
	int num = atoi(count);
	if (num < 0) {
		printf("\nError reading data from file. Program aborting.");
		exit(-1);
	}
	return num;
}

void savefile(char* name, char** contents, int linecount)
{
	FILE* fp = fopen(name, "w"); // Overwrites previous file with same name
	fprintf(fp, "%d", linecount);
	fprintf(fp, "\n");
	for (int i = 0; i <= linecount; i++) // Prints each line from buffer with newline at end (newline was removed when placed in buffer)
	{
		fprintf(fp, "%s", contents[i]);
		fprintf(fp, "\n");
	}
	if (fp)
		fclose(fp);
}
void randomfilename(char* name)
{
	strcpy(name, "File"); // All randomly generated files start with the word 'File'
	char num[NAMESIZE];
	_itoa(rand() % 999999, num, 10); // Random number generated to be added on to file name
	for (int i = 4; i < 10; i++)
		name[i] = num[i - 4]; // Append file name to add random number
}

