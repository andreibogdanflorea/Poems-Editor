// Florea Andrei-Bogdan, 313CD

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util/task_helper.h"

#define MAXPOEM 100000
#define MAXCMD 1000
#define MAXWORD 100
#define SEPARATORS " .,;!?:"
#define SEPARATORS2 " .,;!?:\n"

void makeUpper(char *letter) {
    if (*letter >= 'a' && *letter <= 'z')
    	*letter -= 32;
}

void makeLower(char *letter) {
    if (*letter >= 'A' && *letter <= 'Z')
    	*letter += 32;
}

void uppercase(char *poem) {
    char newVerse = 1;
    for (; *poem != '\0'; poem++) {
        if (newVerse && !(*poem == '\n')) {
			// daca este inceput de vers si nu spatiul dintre strofe
            makeUpper(poem);
            newVerse = 0;
        } else {
            makeLower(poem);
            if (*poem == '\n') {
                newVerse = 1;
            }
        }
    }
}

void trimming(char *poem) {
    char trimmed[MAXPOEM];
    memset(trimmed, 0, sizeof(trimmed));
    int len = 0;
    trimmed[0] = '\0';
    char *word = strtok(poem, SEPARATORS);
    // selectez doar cuvintele, fara punctuatie
    while (word != NULL) {
        int lword = strlen(word);
		if (word[0] == '\n')
		    len--;
		for (int i = 0; i < lword; i++) {
	            trimmed[len++] = word[i]; // le adaug in sirul nou format
	    }
		trimmed[len++] = ' ';
        word = strtok(NULL, SEPARATORS);
    }
    trimmed[len - 1] = '\0';
    strcpy(poem, trimmed);
}

int isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

void silly(char *poem, float probability) {
    for (; *poem != '\0'; poem++) {
        if (isLetter(*poem)) {
            float sample = rand() % 100 / 99.0;
            if (sample < probability) {
                if ((*poem >= 'A' && *poem <= 'Z')) {
				    makeLower(poem);
                } else {
				    makeUpper(poem);
                }
		    }
        }
    }
}

void friendly(char *poem) {
    char result[MAXPOEM];
    int lres = 0;
    char word[MAXWORD], *friendlyWord;
    int len = strlen(poem), lword = 0;

    for (int i = 0; i <= len; ++i) {
        if (isLetter(poem[i])) {
        	// formez cuvantul curent
            word[lword++] = poem[i];
        } else {
            word[lword] = '\0';
            get_friendly_word(word, &friendlyWord);
            if (friendlyWord == NULL) {
                for (int j = 0; j < lword; ++j) {
                    result[lres++] = word[j];
                    // nu am gasit diminutiv, deci adaug cuvantul
                }
            } else {
                int lfriendlyWord = strlen(friendlyWord);
                for (int j = 0; j < lfriendlyWord; ++j) {
                    result[lres++] = friendlyWord[j];
                    // am gasit diminutiv, deci adaug diminutivul
                }
                friendlyWord = NULL;
            }
            result[lres++] = poem[i]; // pastrez punctuatia
            lword = 0;
            word[lword] = '\0';
        }
    }
    strcpy(poem, result);
}

char *lastWord(char *verse, int lv, int *wordStartPos, int *wordEndPos) {
    int pos = lv - 1;
    while (!isLetter(verse[pos])) { // daca la final apare punctuatie
        pos--;
    }

    // pozitia la care se termina cuvantul in cadrul versului
    *wordEndPos = pos + 1;
    char *revWord = malloc(MAXWORD * sizeof(char));
    char *word = malloc(MAXWORD * sizeof(char));
    int len = 0;
    int i;

    for (i = pos; i >= 0 && isLetter(verse[i]); --i) {
        // construiesc inversul cuvantului
        revWord[len++] = verse[i];
    }

	*wordStartPos = i + 1;

    for (int i = 0; i < len; ++i) {
    	// construiesc cuvantul
        word[i] = revWord[len - i - 1];
    }

    word[len] = '\0';
    free(revWord);
    return word;
}

char *createVerse(char *poem, int *lenVerse, int *pos) {
	// construiesc versul incepand cu pozitia pos din poem
	char *verse = strtok(poem + *pos, "\n");
	*lenVerse = strlen(verse); // lungimea versului
	*pos += *lenVerse + 1; // pozitia urmatorului vers, daca exista
	return verse;
}

char *make_rhyme(char *w1, char *w2, int len1, int len2,
	char **list1, char **list2, int *verseNo) {
	char *synonym = NULL;
	int lw1 = strlen(w1);
	int lw2 = strlen(w2);

	// daca deja rimeaza, sinominul sa zicem este chiar primul cuvant
	// diferenta privita ca int trebuie sa fie 32 sau 0 pentru rima
	int diff = (int)abs(w1[lw1 - 1] - w2[lw2 - 1]);
	if (diff == 0 || diff == 32) {
		synonym = w1;
		*verseNo = 1;
		return synonym;
	}

	// daca nu rimeaza, cautam in lista primului cuvant
	for (int i = 0; i < len1; ++i) {
		char *currentWord = list1[i];
		int currentLen = strlen(currentWord);
		int diff = (int)abs(currentWord[currentLen - 1] - w2[lw2 - 1]);
		// daca cuvantul curent formeaza rima
		if (diff == 0 || diff == 32) {
			if (synonym == NULL) {
				synonym = currentWord;
				*verseNo = 1;
			} else if (strcmp(currentWord, synonym) < 0) {
				// daca am gasit un sinonim mai mic lexicografic
				synonym = currentWord;
			}
		}
	}

	if (synonym != NULL)
		return synonym;

	// daca nu a gasit sinonim pentru primul cuvant, cautam pentru al doilea
	for (int i = 0; i < len2; ++i) {
		char *currentWord = list2[i];
		int currentLen = strlen(currentWord);
		int diff = (int)(abs(currentWord[currentLen - 1] - w1[lw1 - 1]));
		if (diff == 0 || diff == 32) {
			if (synonym == NULL) {
				synonym = currentWord;
				*verseNo = 2;
			} else if (strcmp(currentWord, synonym) < 0) {
				synonym = currentWord;
			}
		}
	}

	if (synonym == NULL) {
		*verseNo = 1;
		synonym = w1;
	}

	return synonym;
}

void addWithSynonym(char *poem, char *verse, char *syn, int start, int end) {
	// adauga un vers intr-un poem, inlocuind
	// ultimul cuvant al versului cu sinonimul sau
	strncat(poem, verse, start);
	strcat(poem, syn);
	if ((verse + end) != NULL)
		strcat(poem, verse + end);
	strcat(poem, "\n");
}

void addWithoutSynonym(char *poem, char *verse) {
	// adauga un vers intreg
	strcat(poem, verse);
	strcat(poem, "\n");
}

void rhimy(char *poem, char *rhymeType) {
	int pos;
	char *newpoem = calloc(MAXPOEM, sizeof(char));
	// parcurgem folosind pointerul poem, deci trebuie salvata adresa initiala
	char *poemSave = poem;
	int len = strlen(poem), lenCrt = 0;

	// cat timp nu am parcurs intregul poem
	while (*poem != '\0' && lenCrt < len) {
		int lv1, lv2, lv3, lv4;
		pos = 0;
		int posStart1, posStart2, posStart3, posStart4;
		int posEnd1, posEnd2, posEnd3, posEnd4;

		// luam cele patru versuri din strofa
		char *verse1 = createVerse(poem, &lv1, &pos);
		char *verse2 = createVerse(poem, &lv2, &pos);
		char *verse3 = createVerse(poem, &lv3, &pos);
		char *verse4 = createVerse(poem, &lv4, &pos);

		// avansam un numar de pozitii corespunzatoare in poem
		poem = poem + pos;
		lenCrt += pos + 1;
		if (*poem == '\n') {
			poem++;
		}

		// luam ultimele cuvinte ale versurilor
		char *word1 = lastWord(verse1, lv1, &posStart1, &posEnd1);
		char *word2 = lastWord(verse2, lv2, &posStart2, &posEnd2);
		char *word3 = lastWord(verse3, lv3, &posStart3, &posEnd3);
		char *word4 = lastWord(verse4, lv4, &posStart4, &posEnd4);

		int synLen1, synLen2, synLen3, synLen4;
		char **synList1, **synList2, **synList3, **synList4;

		// generam listele de sinonime
		get_synonym(word1, &synLen1, &synList1);
		get_synonym(word2, &synLen2, &synList2);
		get_synonym(word3, &synLen3, &synList3);
		get_synonym(word4, &synLen4, &synList4);

		char *selectedSynonym1, *selectedSynonym2;
		int selectedVerse1, selectedVerse2;

		if (strcmp(rhymeType, "imperecheata") == 0) {
			// selectam sinonimul potrivit pentru versurile 1,2 si 3,4
			// si in selectedVerse avem in care vers trebuie inlocuit
			selectedSynonym1 = make_rhyme(word1, word2, synLen1, synLen2,
				synList1, synList2, &selectedVerse1);
			selectedSynonym2 = make_rhyme(word3, word4, synLen3, synLen4,
				synList3, synList4, &selectedVerse2);

			// construim sirul nou
			if (selectedVerse1 == 1) { // versurile 1 si 2
				addWithSynonym(newpoem, verse1, selectedSynonym1,
					posStart1, posEnd1);
				addWithoutSynonym(newpoem, verse2);
			} else {
				addWithoutSynonym(newpoem, verse1);
				addWithSynonym(newpoem, verse2, selectedSynonym1,
					posStart2, posEnd2);
			}

			if (selectedVerse2 == 1) { // versurile 3 si 4
				addWithSynonym(newpoem, verse3, selectedSynonym2,
					posStart3, posEnd3);
				addWithoutSynonym(newpoem, verse4);
			} else {
				addWithoutSynonym(newpoem, verse3);
				addWithSynonym(newpoem, verse4, selectedSynonym2,
					posStart4, posEnd4);
			}
		} else if (strcmp(rhymeType, "incrucisata") == 0) {
			selectedSynonym1 = make_rhyme(word1, word3, synLen1, synLen3,
				synList1, synList3, &selectedVerse1);
			selectedSynonym2 = make_rhyme(word2, word4, synLen2, synLen4,
				synList2, synList4, &selectedVerse2);

			if (selectedVerse1 == 1) { // versul 1
				addWithSynonym(newpoem, verse1, selectedSynonym1,
					posStart1, posEnd1);
			} else {
				addWithoutSynonym(newpoem, verse1);
			}

			if (selectedVerse2 == 1) { // versul 2
				addWithSynonym(newpoem, verse2, selectedSynonym2,
					posStart2, posEnd2);
			} else {
				addWithoutSynonym(newpoem, verse2);
			}

			if (selectedVerse1 == 1) { // versul 3
				addWithoutSynonym(newpoem, verse3);
			} else {
				addWithSynonym(newpoem, verse3, selectedSynonym1,
					posStart3, posEnd3);
			}

			if (selectedVerse2 == 1) { // versul 4
				addWithoutSynonym(newpoem, verse4);
			} else {
				addWithSynonym(newpoem, verse4, selectedSynonym2,
					posStart4, posEnd4);
			}
		} else { // rima imbratisata
			selectedSynonym1 = make_rhyme(word1, word4, synLen1, synLen4,
				synList1, synList4, &selectedVerse1);
			selectedSynonym2 = make_rhyme(word2, word3, synLen2, synLen3,
				synList2, synList3, &selectedVerse2);

			if (selectedVerse1 == 1) { // versul 1
				addWithSynonym(newpoem, verse1, selectedSynonym1,
					posStart1, posEnd1);
			} else {
				addWithoutSynonym(newpoem, verse1);
			}

			if (selectedVerse2 == 1) { // versurile 2 si 3
				addWithSynonym(newpoem, verse2, selectedSynonym2,
					posStart2, posEnd2);
				addWithoutSynonym(newpoem, verse3);
			} else {
				addWithoutSynonym(newpoem, verse2);
				addWithSynonym(newpoem, verse3, selectedSynonym2,
					posStart3, posEnd3);
			}
			if (selectedVerse1 == 1) { // versul 4
				addWithoutSynonym(newpoem, verse4);
			} else {
				addWithSynonym(newpoem, verse4, selectedSynonym1,
					posStart4, posEnd4);
			}
		}

		// adaugam un \n, spatiu intre strofe
		strcat(newpoem, "\n");
		free(word1); free(word2); free(word3); free(word4);

		if (*(poem - 1) == '\0') {
			break;
		}
	}

	// copiem la adresa salvata, a sirului initial, sirul nou format
	strncpy(poemSave, newpoem, strlen(newpoem) - 2);
	poemSave[strlen(newpoem) - 2] = '\0';
	free(newpoem);
}

void print(char *poem) {
	printf("%s\n\n", poem);
}

int main(void) {
    srand(42);
    char *poem = calloc(MAXPOEM, sizeof(char));
    char command[MAXCMD];
    while (1) {
        fgets(command, MAXCMD, stdin);
        int len = strlen(command);
        command[len - 1] = '\0';
        char *option = strtok(command, " ");

        if (strcmp(option, "load") == 0) {
            char *path = strtok(NULL, " ");
            load_file(path, poem);
        } else if (strcmp(option, "uppercase") == 0) {
            uppercase(poem);
        } else if (strcmp(option, "trimming") == 0) {
            trimming(poem);
        } else if (strcmp(option, "make_it_silly") == 0) {
            char *probabilityString = strtok(NULL, " ");
            float probability = atof(probabilityString);
            silly(poem, probability);
        } else if (strcmp(option, "make_it_friendlier") == 0) {
            friendly(poem);
        } else if (strcmp(option, "make_it_rhyme") == 0) {
            char *rhymeType = strtok(NULL, " ");
            rhimy(poem, rhymeType);
        } else if (strcmp(option, "print") == 0) {
            print(poem);
        } else {
            break;
        }
    }
	return 0;
}
