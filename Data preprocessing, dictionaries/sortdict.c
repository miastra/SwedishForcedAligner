////////////////////////////////////////////////////////////////////////////////
// Sort dictionary:
//   sortdict <unsorted-dictionary-filename> <sorted-dictionary-filename>
//
////////////////////////////////////////////////////////////////////////////////


#define _CRT_SECURE_NO_WARNINGS

// Works when the complete list contains sorted sub lists, where (in most cases)
// the last added link can be used for adding the next. Useful for sorting.
#define SPEED_HACK

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


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


void AddWord(Word **list, char *word, char *phon);

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
void LoadDictionary(Word **list, char *filename, int sort) {
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

			// Add.
			if (strlen(word) && strlen(phon)) {
				if (sort) {
					printf("Adding word %s \n", word);
					AddWord(list, word, phon);
				}
				else {
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
void AddWord(Word **list, char *word, char *phon) {
#ifdef SPEED_HACK
	// speed hack for sorted sublists.
	static Word *lastWord = 0;
	//
#endif
	Word *link;
	Word *prev;
	Word *w;
	int result;

	// Is list empty (null)?
	if (*list == 0) {
		// Create new word that defines the start of the list.
		w = NewWord(word, phon, 1);
		*list = w;
		return;
	}

	prev = 0;

#ifdef SPEED_HACK
	// speed hack for sorted sublists.
	if (lastWord && lastWord != *list && AlphaCompareStrings(word, lastWord->word) >= 0) {
		link = lastWord;
	}
	// Search through list for the correct insertion point.
	else {
		link = *list;
	}
#else
	link = *list;
#endif
	while (link != 0) {
		// Compare words.
		result = AlphaCompareStrings(word, link->word);
		// Equal?
		if (result == 0) {
			// Don't insert.
#ifdef SPEED_HACK
			// added after run, should work.
			lastWord = link;
#endif
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
#ifdef SPEED_HACK
			// speed hack for sorted sublists.
			lastWord = w;
#endif
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
	char sortedFilename[512];

	if (argc < 3) {
		printf("ERROR: sortdict <dictionary-filename> <sorted-filename>\n");
		return EXIT_FAILURE;
	}

	strcpy(dictFilename, args[1]);
	strcpy(sortedFilename, args[2]);

	// Load and sort.
	printf("Loading dictionary ...\n");
	LoadDictionary(&list, dictFilename, 1);
	printf("Done!\n");

	if (list) {
		printf("Saving dictionary ...\n");
		SaveDictionary(list, sortedFilename);
		printf("Done!\n");
	}
	else {
		printf("ERROR: Could not load dictionary \"%s\"\n", dictFilename);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
