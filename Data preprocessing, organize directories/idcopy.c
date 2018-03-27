#define _CRT_SECURE_NO_WARNINGS

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "Windows.h"

// Main.
int main(int argc, const char **args) {
	// Full filename.
	char filename[512];
	// Plain filename.
	char plainFilename[512];
	// Foldername.
	char folderName[512];
	// Destination filename.
	char dstFilename[512];
	// Temporary.
	int pos;
	// Start character and number of characters for id.
	int startChar, numChars;


	// Missing argument?
	if (argc < 4) {
		printf("USAGE: idcopy <filename> <start_char> <num_chars>\n");
		return EXIT_FAILURE;
	}

	// Copy filename.
	strcpy(filename, args[1]);

	// Start character.
	startChar = atoi(args[2]);
	numChars = atoi(args[3]);

	// The filename is probably in the format "c:\folder\folder ...". Find the
	// start position of the actual filename. Start at the last character and
	// look for '/' or '\'.
	for (pos = strlen(filename) - 1; pos > 0; pos--) {
		if (filename[pos - 1] == '/' || filename[pos - 1] == '\\') break;
	}
	// Copy filename without the path to plainFilename. Ex: filename
	// "c:\folder\myfile.wav" would give plainFilename to "myfile.wav"
	strcpy(plainFilename, &filename[pos]);
	// Create the name of the wanted folder. If startChar = 1, numChars = 3
	// and filename = "abcdefg.wav", folderName would become "bcd".
	strncpy(folderName, &filename[pos + startChar], numChars);
	folderName[numChars] = '\0';

	// Debug output.Comment out for faster execution.
	printf("FOLDERNAME = \"%s\"\n", folderName);
	
	// Create folder. It doesn't matter if it already exists.
	CreateDirectory(folderName, 0);

	// Create filename with path.
	sprintf(dstFilename, "%s\\%s", folderName, plainFilename);

	// Debug output. Comment out for faste execution.
	printf("DSTFILENAME = \"%s\"\n", dstFilename);

	// Copy file.
	CopyFile(filename, dstFilename, 0);

	return EXIT_SUCCESS;
}