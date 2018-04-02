////////////////////////////////////////////////////////////////////////////////
// Dictionary.
//
// dict <dictionary-filename> <input-filename>
//
// Adds new words found in input file to dictionary.
////////////////////////////////////////////////////////////////////////////////


#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Linked list structure for words.
typedef struct sWordLink Word;
struct sWordLink {
	// The word, ex: "BÅTEN".
	char word[64];
	// The phon, ex: "b o: t e n"
	char phon[128];
	// Flag if the word is new for this run.
	int isNew;
	// Next word in the list.
	Word *next;
};

int Removable(int c) {
	switch (c) {
	case '\'':
	case '+':
	case '#':
	case 'v':
		return 1;
		break;
	default:
		return 0;
	}
}

int Vowel(int c) {
	switch (c) {
	case 'A':
	case 'O':
	case 'U':
	case ']': // Å
	case 'E':
	case 'I':
	case 'Y':
	case '[': // Ä
	case '\\': // Ö
		return 1;
		break;
	default:
		return 0;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Write converted phon to file.
////////////////////////////////////////////////////////////////////////////////

void WriteConvertedPhonStr(char *dst, char *phon) {
	int index;
	int charA;
	int charB;

	dst[0] = '\0';

	index = 0;
	charA = phon[index];
	charB = phon[index + 1];
	while (charA != '\0') {
		if (Removable(charA)) {
			index++;
		}
		else if (Vowel(charA) && (charB == ':' || isdigit(charB))) {
			if (charA == ']' && charB == ':') {
				strcat(dst, "o: ");
			}
			else if (charA == 'A' && charB == ':') {
				strcat(dst, "A: ");
			}
			else if (charA == 'E' && charB == '0') {
				strcat(dst, "e ");
			}
			else if (charA == 'E' && charB == ':') {
				strcat(dst, "e: ");
			}
			else if (charA == 'I' && charB == ':') {
				strcat(dst, "i: ");
			}
			else if (charA == '[' && charB == '3') {
				strcat(dst, "E: ");
			}
			else if (charA == '\\' && charB == ':') {
				strcat(dst, "2: ");
			}
			else if (charA == 'U' && charB == ':') {
				strcat(dst, "}: ");
			}
			else if (charA == 'O' && charB == ':') {
				strcat(dst, "u: ");
			}
			else if (charA == '\\' && charB == '3') {
				strcat(dst, "2: ");
			}
			else if (charA == '[' && charB == '4') {
				strcat(dst, "E ");
			}
			else if (charA == 'Y' && charB == ':') {
				strcat(dst, "y: ");
			}
			else if (charA == '[' && charB == ':') {
				strcat(dst, "E: ");
			}
			else if (charA == '\\' && charB == '4') {
				strcat(dst, "9 ");
			}
			index += 2;
		}
		else if (isdigit(charA)) {
			if (charA == '2' && charB == 'N') {
				strcat(dst, "n` ");
			}
			else if (charA == '2' && charB == 'D') {
				strcat(dst, "d` ");
			}
			else if (charA == '2' && charB == 'T') {
				strcat(dst, "t` ");
			}
			else if (charA == '2' && charB == 'S') {
				strcat(dst, "s` ");
			}
			else if (charA == '2' && charB == 'L') {
				strcat(dst, "l` ");
			}
			index += 2;
		}
		else if (charA == 'S' && charB == 'J') {
			strcat(dst, "x\\ ");
			index += 2;
		}
		else if (charA == 'N' && charB == 'G') {
			strcat(dst, "N ");
			index += 2;
		}
		else if (charA == 'T' && charB == 'J') {
			strcat(dst, "s' "); // ???
			index += 2;
		}
		else {
			switch (charA) {
			case 'A': strcat(dst, "a "); break;
			case 'N': strcat(dst, "n "); break;
			case 'R': strcat(dst, "r "); break;
			case 'T': strcat(dst, "t "); break;
			case 'L': strcat(dst, "l "); break;
			case 'K': strcat(dst, "k "); break;
			case 'I': strcat(dst, "I "); break;
			case 'S': strcat(dst, "s "); break;
			case 'V': strcat(dst, "v "); break;
			case ']': strcat(dst, "O "); break;
			case 'M': strcat(dst, "m "); break;
			case 'E': strcat(dst, "e "); break;
			case 'D': strcat(dst, "d "); break;
			case 'G': strcat(dst, "g "); break;
			case 'F': strcat(dst, "f "); break;
			case 'J': strcat(dst, "j "); break;
			case 'H': strcat(dst, "h "); break;
			case 'B': strcat(dst, "b "); break;
			case 'P': strcat(dst, "p "); break;
			case '[': strcat(dst, "E "); break;
			case 'O': strcat(dst, "U "); break;
			case 'U': strcat(dst, "u0 "); break;
			case '\\': strcat(dst, "9 "); break;
			case 'Y': strcat(dst, "Y "); break;
			}
			index += 1;
		}
		charA = phon[index];
		charB = phon[index + 1];
	}
}

////////////////////////////////////////////////////////////////////////////////
// Create new word.
////////////////////////////////////////////////////////////////////////////////
Word *NewWord(char *word, char *phon, int isNew) {
	Word *w = (Word *)malloc(sizeof(Word));
	strcpy(w->word, word);
	strcpy(w->phon, phon);
	w->isNew = isNew;
	w->next = 0;
	return w;
}

////////////////////////////////////////////////////////////////////////////////
// Hack for Swedish characters.
////////////////////////////////////////////////////////////////////////////////
int SwedishHack(int c) {
	switch (c) {
	case 'Å': return 91; break;
	case 'Ä': return 92; break;
	case 'Ö': return 93; break;
	case 'å': return 123; break;
	case 'ä': return 124; break;
	case 'ö': return 125; break;
	default: return c;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Alphabetically compare two strings with Swedish hack.
////////////////////////////////////////////////////////////////////////////////
int AlphaCompareStrings(char *src, char *dst) {
	int srcLen = strlen(src);
	int dstLen = strlen(dst);
	int minLen;
	int srcC;
	int dstC;
	int i;

	if (srcLen < dstLen) minLen = srcLen;
	else minLen = dstLen;

	for (i = 0; i < minLen; i++) {
		srcC = SwedishHack(src[i]);
		dstC = SwedishHack(dst[i]);
		if (srcC < dstC) return -1;
		else if (srcC > dstC) return 1;
	}

	if (srcLen < dstLen) return -1;
	else if (srcLen > dstLen) return 1;
	else return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Load dictionary.
////////////////////////////////////////////////////////////////////////////////
void LoadDictionary(Word **list, char *filename) {
	FILE *file = 0;
	Word *link = 0;
	Word *w = 0;
	char word[64];
	char phon[128];
	int wordPos;
	int phonPos;
	int c;

	file = fopen(filename, "r");
	if (file) {
		do {
			wordPos = 0;
			phonPos = 0;

			// Read word.
			c = fgetc(file);
			while (!(c == ' ' || c == '\t' || c == EOF)) {
				word[wordPos++] = (char)c;
				c = fgetc(file);
			}
			word[wordPos] = '\0';

			// Read phon.
			c = fgetc(file);
			while (!(c == '\n' || c == EOF)) {
				if (c != '\r') phon[phonPos++] = c;
				c = fgetc(file);
			}
			phon[phonPos] = '\0';

			// Add, no sorting needed here.
			if (strlen(word) && strlen(phon)) {
				w = NewWord(word, phon, 0);
				// First word to be added?
				if (link == 0) {
					*list = w;
					link = w;
				}
				// Add.
				else {
					link->next = w;
					link = w;
				}
			}
		} while (c != EOF);

		fclose(file);
	}
}

////////////////////////////////////////////////////////////////////////////////
// Save dictionary.
////////////////////////////////////////////////////////////////////////////////
void SaveDictionary(Word *list, char *filename) {
	FILE *file;

	if (!list) return;

	file = fopen(filename, "w");
	if (file) {
		while (list) {
			fprintf(file, "%s %s\n", list->word, list->phon);
			list = list->next;
		}
		fclose(file);
	}
	else {
		printf("ERROR: Could not open dictionary for writing!\n");
	}
}

////////////////////////////////////////////////////////////////////////////////
// Insert new word into list if it doesn't already exist. A new list will be
// created if *list is null.
////////////////////////////////////////////////////////////////////////////////
void AddWord(Word **list, char *word, char *orgPhon) {
	Word *link;
	Word *prev;
	Word *w;
	int result;
	char phon[128];

	// Convert phon.
	WriteConvertedPhonStr(phon, orgPhon);

	// Is list empty (null)?
	if (*list == 0) {
		// Create new word that defines the start of the list.
		w = NewWord(word, phon, 1);
		*list = w;
		return;
	}

	// Search through list for the correct insertion point.
	link = *list;
	prev = 0;

	
	while (link != 0) {
		// Compare words.
		result = AlphaCompareStrings(word, link->word);
		// Equal?
		if (result == 0 && strcmp(phon, link->phon) == 0) {
			// Don't insert.
			return;
		}
		// Should new word be inserted before current.
		else if (result < 0) {
			w = NewWord(word, phon, 1);
			w->next = link;
			// Before first word?
			if (prev == 0) {
				// Make list point to the first word.
				*list = w;
			}
			else {
				prev->next = w;
			}
			return;
		}
		// Keep looking.
		else {
			prev = link;
			link = link->next;
		}
	}

	// Add last.
	w = NewWord(word, phon, 1);
	prev->next = w;
}

////////////////////////////////////////////////////////////////////////////////
// Free all words.
////////////////////////////////////////////////////////////////////////////////
void FreeWords(Word **list) {
	Word *link;
	Word *next;

	if (*list) {
		link = *list;
		while (link) {
			next = link->next;
			free(link);
			link = next;
		}
		*list = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Debug, display list.
////////////////////////////////////////////////////////////////////////////////
void PrintList(Word *list) {
	int count = 0;

	while (list) {
		count++;
		printf("%d: %s\n", count, list->word);
		list = list->next;
	}
	printf("(count = %d)\n", count);
}

////////////////////////////////////////////////////////////////////////////////
// Main.
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **args) {
	Word *list = 0;
	FILE *wordFile = 0;
	FILE *phonFile = 0;
	char dictFilename[512];
	char inputFilename[512];
	char word[64];
	char phon[128];
	int done;
	int pos;
	int c;

	if (argc < 3) {
		printf("ERROR: dict <dictionary-filename> <input-filename>\n");
		return EXIT_FAILURE;
	}

	// Copy arguments.
	strcpy(dictFilename, args[1]);
	strcpy(inputFilename, args[2]);

	// Open two instances of the input file for scanning two lines at a time.
	wordFile = fopen(inputFilename, "r");
	phonFile = fopen(inputFilename, "r");
	if (wordFile == 0 || phonFile == 0) {
		printf("ERROR: Could not open input file \"%s\"\n", inputFilename);
		return EXIT_FAILURE;
	}

	// Move phonFile to second line of the file.
	c = fgetc(phonFile);
	while (c != '\n') {
		if (c == EOF) {
			printf("ERROR: Bad input file \"%s\"\n", inputFilename);
			fclose(wordFile);
			fclose(phonFile);
			return EXIT_FAILURE;
		}
		else {
			c = fgetc(phonFile);
		}
	}

	// Load old dictionary.
	LoadDictionary(&list, dictFilename);

	// Look for words.
	done = 0;
	do {
		// Word.
		pos = 0;
		c = fgetc(wordFile);
		// Skip leading or extra spaces.
		while (c == ' ') c = fgetc(wordFile);
		// Read word, terminated by space.
		while (!(c == ' ' || c == '\n' || c == EOF)) {
			word[pos++] = (char)c;
			c = fgetc(wordFile);
		}
		word[pos] = '\0';
		// Done?
		if (c == '\n') {
			done = 1;
		}
		// Error?
		else if (c == EOF) {
			break;
		}

		// Phon.
		pos = 0;
		c = fgetc(phonFile);
		// Skip leading or extra spaces.
		while (c == ' ') c = fgetc(phonFile);
		// Read phon.
		while (!(c == ' ' || c == '\n' || c == EOF)) {
			phon[pos++] = (char)c;
			c = fgetc(phonFile);
		}
		phon[pos] = '\0';

		// Do we have something valid?
		if (strlen(word) && strlen(phon)) {
			AddWord(&list, word, phon);
		}
		else {
			break;
		}
	} while (!done);

	// Close files.
	fclose(wordFile);
	fclose(phonFile);

	// No errors?
	if (done) {
		SaveDictionary(list, dictFilename);
	}
	else {
		printf("ERROR: Error parsing \"%s\"\n", inputFilename);
	}

	// Free all words.
	FreeWords(&list);

	return EXIT_SUCCESS;
}
