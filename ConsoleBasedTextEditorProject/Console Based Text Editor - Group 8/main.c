
#include "uistuff.h"
#include "backend.h"	

int main(int argc, char* argv[])
{
	if (argc <= 1)
	{
		printf("\nPlease enter your name as an argument.\n");
		exit(2);
	}
	printf("\nWelcome to command line text editor, %s! Options below, enter number associated with desired option.", argv[1]);
	srand(time(0)); // Seeds rand with time since 0
	FILE* fp = fopen(FILELIST, "r");
	if (!fp)
		createfiledirectory(); // If file directory doesnt exist, call function to make one
	if (fp)
		fclose(fp);
	bool run = true;
	while (run)
	{
		Mainmenu(); // Print main menu
		int in = getmenuinput();
		system("@cls||clear");
		switch (in) {
		case 1:
		{
			createfile(); // Begins the file creation process
			break;
		}
		case 2:
		{
			printfiles(); // Print all files in directory
			break;
		}
		case 3:
		{
			openfile(); // Begin file editing process
			break;
		}
		case 4:
		{
			deletefile(); // Begin file deletion process
			break;
		}
		case 5:
		{
			displayfile(); // Display file of user choice
			break;
		}
		case 6:
		{
			run = false; // Ends run loop before breaking
			break;
		}
		default:
		{
			printf("\nPlease enter a valid option.\n"); // If input isnt a valid option, inform user
			break;
		}
		}
	}
	atexit((*programexit)); // Calls programexit function before closing program
}

