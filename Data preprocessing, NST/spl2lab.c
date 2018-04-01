// spl2lab, no error checking. Only works with ANSI files, so the resulting
// files need to be converted to UTF8.

#define _CRT_SECURE_NO_WARNINGS

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "ctype.h"

int ReadLine(char *dst, FILE *file);
char ToUpper(char c);
int IsValidChar(char c);

// main.
int main(int argc, const char **args) {
	FILE *file = 0;
	char line[512] = "";
	int count = 0;
	char dstPath[512] = "";

	if (argc < 2) {
		printf("ERROR: %s No input file specified\n", args[0]);
		exit(EXIT_FAILURE);
	}
	if (argc >= 3) {
		char lastChar;
		strcpy(dstPath, args[2]);
		lastChar = dstPath[strlen(dstPath) - 1];
		if (!(lastChar == '\\' || lastChar == '/')) strcat(dstPath, "\\");
	}

	file = fopen(args[1], "rb");
	if (!file) {
		printf("ERROR: Could not open file\n");
		return EXIT_FAILURE;
	}

	// Find record states.
	do {
		ReadLine(line, file);
	} while (strcmp(line, "[Record states]"));

	// Parse.
	while (ReadLine(line, file)) {
		char *marker;
		// End of records?
		if (strstr(line, "[")) break;
		// Valid line?
		marker = strstr(line, ">-<>-<");
		if (marker) {
			char phrase[512];
			char lab[128];
			char labFilename[512];
			char *endMarker;
			int pos = 0;
			int id = 0;
			FILE *out = 0;
			
			// id first thing in the line, just sscanf it.
			sscanf(line, "%d", &id);

			// Scan phrase, ends with '>'.
			marker += 6;
			endMarker = strstr(marker, ">");
			while (marker != endMarker) {
				if (IsValidChar(*marker)) phrase[pos++] = ToUpper(*marker);
				marker++;
			}
			phrase[pos] = '\0';

			// Create filename from wav.
			endMarker = strstr(line, ".wav>");
			marker = endMarker;
			while (*(marker - 1) != '<') marker--;
			pos = 0;
			while (marker != endMarker) lab[pos++] = *marker++;
			lab[pos] = '\0';
			strcat(lab, ".lab");

			// Create file for phrase.
			sprintf(labFilename, "%s%s", dstPath, lab);
			out = fopen(labFilename, "w");
			fprintf(out, "%s", phrase);
			fclose(out);

			// Output.
			printf("%d, \"%s\" = \"%s\"\n", id, lab, phrase);

			count++;
		}
	}
	printf("COUNT = %d\n", count);
	fclose(file);

	return EXIT_SUCCESS;
}

// Read a line from file to dst.
int ReadLine(char *dst, FILE *file) {
	int pos = 0;
	char c;
	do {
		c = fgetc(file);

		if (c == EOF || c == '\n') break;
		if (c != '\r') {
			dst[pos] = c;
			pos++;
		}
	} while (1);
	dst[pos] = '\0';

	return !(c == EOF && pos == 0);
}

// Hack for å, ä, ö.
char ToUpper(char c) {
	switch (c) {
	case 'å': return 'Å'; break;
	case 'ä': return 'Ä'; break;
	case 'ö': return 'Ö'; break;
	default: return toupper(c); break;
	}
}

// Only allow alpha, space and å, ä, ö.
int IsValidChar(char c) {
	return (c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == ' ' || c == 'å' || c == 'ä' || c == 'ö' || c == 'Å' || c == 'Ä' || c == 'Ö'; 
}
