#define _CRT_SECURE_NO_WARNINGS
//converts dictionary NST_sampa to dictionary MFA_IPA
// run with converter <input-file> <output-file>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int Vowel(int c) {
	switch (c) {
	case 'A':
	case 'O':
	case 'U':
	case 'Å': // Å
	case 'E':
	case 'I':
	case 'Y':
	case 'Ä': // Ä
	case 'Ö': // Ö
	case 'a':
	case 'o':
	case 'u':
	case 'å': // Å
	case 'e':
	case 'i':
	case 'y':
	case 'ä': // Ä
	case 'ö': // Ö
		return 1;
		break;
	default:
		return 0;
	}
}

void WriteConvertedPhon(FILE *file, char *phon) {
	
	int index;
	int charA;
	int charB;
	int len = strlen(phon);

	index = 0;
	charA = phon[index];
	charB = phon[index + 1];
	while (charA != '\0' && index < len) {
		if (Vowel(charA) && (charB == ':' || charB == '0')) {
			if (charA == 'A' && charB == ':') {
				fprintf(file, "al ");
			}
			else if (charA == 'E' && charB == ':') {
				fprintf(file, "ael ");
			}
			else if (charA == 'e' && charB == ':') {
				fprintf(file, "el ");
			}
			else if (charA == 'i' && charB == ':') {
				fprintf(file, "il ");
			}
			else if (charA == 'o' && charB == ':') {
				fprintf(file, "ol ");
			}
			else if (charA == 'u' && charB == '0') {
				fprintf(file, "ox ");
			}
			else if (charA == 'y' && charB == ':') {
				fprintf(file, "uel ");
			}
			else if (charA == 'u' && charB == ':') {
				fprintf(file, "ul ");
			}

			index += 3;
		}
		else if (charA == '2' && charB == ':') {
			fprintf(file, "oel ");
			index += 3;
		}
		else if (charA == 's' && charB == '`') {
			fprintf(file, "C ");
			index += 3;
		}
		else if (charA == 'd' && charB == '`') {
			fprintf(file, "dr ");
			index += 3;
		}
		else if (charA == 'l' && charB == '`') {
			fprintf(file, "lr "); // ???
			index += 3;
		}
		else if (charA == 'n' && charB == '`') {
			fprintf(file, "nr ");
			index += 3;
		}
		else if (charA == 't' && charB == '`') {
			fprintf(file, "tr ");
			index += 3;
		}
		else if (charA == 'x' && charB == '\\') {
			fprintf(file, "S ");
			index += 3;
		}
		else if (charA == 's' && charB == '\'') {
			fprintf(file, "S ");
			index += 3;
		}
		else if (charA == '}' && charB == ':') {
			fprintf(file, "uxl ");
			index += 3;
		}
		else {
			switch (charA) {
			case 'a': fprintf(file, "a "); break;
			case 'E': fprintf(file, "ae "); break;
			case 'b': fprintf(file, "b "); break;
			case 'd': fprintf(file, "d "); break;
			case 'e': fprintf(file, "e "); break;
			case 'f': fprintf(file, "f "); break;
			case 'g': fprintf(file, "g "); break;
			case 'h': fprintf(file, "h "); break;
			case 'I': fprintf(file, "i "); break;
			case 'j': fprintf(file, "j "); break;
			case 'k': fprintf(file, "k "); break;
			case 'l': fprintf(file, "l "); break;
			case 'm': fprintf(file, "m "); break;
			case 'n': fprintf(file, "n "); break;
			case 'N': fprintf(file, "ng "); break;
			case 'O': fprintf(file, "o "); break;
			case '9': fprintf(file, "oe "); break;
			case 'p': fprintf(file, "p "); break;
			case 'r': fprintf(file, "r "); break;
			case 's': fprintf(file, "s "); break;
			case 't': fprintf(file, "t "); break;
			case 'U': fprintf(file, "u "); break;
			case 'Y': fprintf(file, "ue "); break;
			case 'v': fprintf(file, "v "); break;
			}
			index += 2;
		}
		charA = phon[index];
		charB = phon[index + 1];
	}
}

int ReadLine(FILE *file, char *word, char *phon) {
	int pos;
	int c;

	pos = 0;
	c = fgetc(file);
	if (c == EOF) return 0;
	while (!(c == EOF || c == ' ' || c == '\t' || c == '\n')) {
		if (c != '\r') word[pos++] = c;
		c = fgetc(file);
	}
	word[pos] = '\0';

	pos = 0;
	pos = 0;
	c = fgetc(file);
	while (!(c == EOF || c == '\n')) {
		if (c != '\r') phon[pos++] = c;
		c = fgetc(file);
	}
	phon[pos] = '\0';

	return 1;
}

int main(int argc, char **args) {
	FILE *inFile;
	FILE *outFile;
	char word[256];
	char phon[256];

	// Enough arguments?	
	if (argc < 3) {
		printf("converter <input-file> <output-file>\n");
		exit(EXIT_FAILURE);
	}

	// Open input file.
	inFile = fopen(args[1], "r");
	if (!inFile) {
		printf("ERROR: Could not open \"%s\" for reading\n", args[1]);
	}

	// Open output file.
	outFile = fopen(args[2], "w");
	if (!outFile) {
		printf("ERROR: Could not open \"%s\" for writing\n", args[2]);
	}

	// Loop until end of file has been reached.
	while (ReadLine(inFile, word, phon)) {
		if (strlen(word) && strlen(phon)) {
			fprintf(outFile, "%s ", word);
			WriteConvertedPhon(outFile, phon);
			fprintf(outFile, "\n");
		}
	}

	// Close files.
	fclose(inFile);
	fclose(outFile);

	return EXIT_SUCCESS;
}
