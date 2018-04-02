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
void RemoveSubstring(char *s, const char *toremove);

// main.
int main(int argc, char **args) {
	FILE *file = 0;
	char line[512] = "";
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

	// Find file name.

	int pos = 0;
	char lab[128];
	char toDict[128];
	char cleanFilename[256];
	char *marker;

	// Check backwards to find last slash or backslash. After
	// this sign the file name begins.
	char *start = args[1];
	for (int i = strlen(args[1]) - 1; i > 0; i--) {
		if (args[1][i - 1] == '\\' || args[1][i - 1] == '/') {
			start = &args[1][i];
			break;
		}
	}

	// Copy everything to cleanFilename
	strcpy(cleanFilename, start);
	// Remove .smp.mix
	RemoveSubstring(cleanFilename, ".smp.mix");

	printf("Filename = %s\n", cleanFilename);
	strcpy(lab, cleanFilename);
	strcpy(toDict, lab);
	strcat(toDict, ".txt");
	strcat(lab, ".lab");

	// Find phrase.
	do {
		if (!ReadLine(line, file)) {
			fclose(file);
			printf("ERROR: TEXT NOT FOUND!\n");
			return EXIT_FAILURE;
		}
	} while (strcmp(line, "TEXT:"));

	// Parse.
	
		marker = strstr(line, "TEXT:");
		if (marker) {
			ReadLine(line, file);
			char phrase [512];
			
			char labFilename[512];
			char todictFilename[512];
			
			
			int id = 0;
			FILE *out = 0;
			pos = 0;


			
			for (unsigned int i = 0; i < strlen(line); i++) {
				if (IsValidChar(line[i])) {
					phrase[pos] = ToUpper(line[i]);
					pos++;
				}
			}

			phrase[pos] = '\0';

			/* Comment out lines below if you want lab and txt files with the X-words*/
			
			if (strstr(phrase, "XTVEKX")) {
				RemoveSubstring(phrase, "XTVEKX");
			}

			if (strstr(phrase, "XSMACKX")) {
				RemoveSubstring(phrase, "XSMACKX");
			}

			if (strstr(phrase, "XINANDX")) {
				RemoveSubstring(phrase, "XINANDX");
			}

			if (strstr(phrase, "XAVBRORDX")) {
				RemoveSubstring(phrase, "XAVBRORDX");
			}

			if (strstr(phrase, "XKLICKX")) {
				RemoveSubstring(phrase, "XKLICKX");
			}

			if (strstr(phrase, "XSKRATTX")) {
				RemoveSubstring(phrase, "XSKRATTX");
			}

			if (strstr(phrase, "XSUCKX")) {
				RemoveSubstring(phrase, "XSUCKX");
			}

			if (strstr(phrase, "XUTANDX")) {
				RemoveSubstring(phrase, "XUTANDX");
			}
			if (strstr(phrase, "XHARKLINGX")) {
				RemoveSubstring(phrase, "XHARKLINGX");
			}
			
			/* comment out lines above if you want lab and txt files with the X-words*/

			// Create file for phrase.
			sprintf(labFilename, "%s%s", dstPath, lab);
			out = fopen(labFilename, "w");
			fprintf(out, "%s", phrase);
			fclose(out);

			// Output. Comment out for faster execution.
			printf("%d, \"%s\" = \"%s\"\n", id, lab, phrase);

			FILE *todict = 0;

			char pronphrase[512];
			char pronphrasealt[512];
			pos = 0;
			ReadLine(line, file);
			if (strcmp(line, ".") == 0) {
				ReadLine(line, file);
			}
			for (unsigned int i = 0; i < strlen(line); i++) {
				pronphrase[pos] = line[i];
				pos++;
			}
			pronphrase[pos] = '\0';
			if (strstr(pronphrase, "PHONEME:")) {
				RemoveSubstring(pronphrase, "PHONEME:");
			}
			if (!strstr(pronphrase, ".")) {
				ReadLine(line, file);
				strcat(pronphrase, " ");
				strcat(pronphrase, line);
			}
			printf("%s\n", pronphrase);

			do {
				if (!ReadLine(line, file)) break;
				printf("Söker Labels:\n"); //debug output, comment out for faster execution
			} while (strncmp(line, "Labels:", 7));

			printf("LINE = %s\n", line);
			pos = 0;
			marker = strstr(line, "Labels:");
			printf("MARKER = %s\n", marker);
			marker += 7;
			for (unsigned int i = 0; i < (strlen(line) - 7); i++) {
				pronphrasealt[pos] = *marker;
				pos++;
				marker++;
			}
			pronphrasealt[pos] = '\0';
			printf("%s\n", pronphrasealt);
			
			// Create file for dictionary.
			sprintf(todictFilename, "%s%s", dstPath, toDict);
			todict = fopen(todictFilename, "w");
			fprintf(todict, "%s\n%s\n%s", phrase, pronphrase, pronphrasealt);
			fclose(todict);

			// Output.
			printf("%d, \"%s\" = \"%s\"\n", id, toDict, phrase);
		}

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
	case '}': return 'Å'; break;
	case '{': return 'Ä'; break;
	case '|': return 'Ö'; break;
	case '\\': return 'Ö'; break;
	case '_': return ' '; break;
	default: return toupper(c); break;
	}
}

// Only allow alpha, space and å, ä, ö. 48-57 ASCII-signs 0 to 9, _ as in "grand_hotell"
int IsValidChar(char c) {
	return (c >= 48 && c <= 57 || c >= 65 && c <= 90) || (c >= 97 && c <= 125) || c == ' ' || c == 'å' || c == 'ä' || c == 'ö' || c == 'Å' || c == 'Ä' || c == 'Ö' || c == '_' || c == '\\';
}

//Remove "xtvekx"  and other unwanted strings
void RemoveSubstring(char *s, const char *toremove)
{
	while (s = strstr(s, toremove))
		memmove(s, s + strlen(toremove), 1 + strlen(s + strlen(toremove)));
}
