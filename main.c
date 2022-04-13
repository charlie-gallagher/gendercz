#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"



int main(int argc, char *argv[])
{
	/* Print the help if no file given */
	if (argc == 1) {
		printf("Quiz yourself on the genders of words!\n");
		printf("gendercz [<file>]\n\n");
		return 0;
	}

	/* Otherwise use the given file */
	int n_input_words = 0;
	FILE *p_input_file = fopen(argv[1], "r");
	if (p_input_file == NULL) {
		fprintf(stderr, "Invalid input file\n");
		exit(EXIT_FAILURE);
	}

	/* The pointer is defined but nothing allocated */
	/* But since I want an array of cz_pairs, do I need to do something
	 * different with the struct declaration?
	 */
	struct cz_pair *word_array;

	/* Initially allocate 100 structs to the list */
	if ((word_array = malloc(100 * sizeof(struct cz_pair))) == NULL) {
		fprintf(stderr, "gendercz: allocation error\n");
		exit(EXIT_FAILURE);
	}


	n_input_words = fill_word_array(word_array, p_input_file);

	if (n_input_words == 0) {
		fprintf(stderr, "Word list invalid\n");
	} else {
		/* Main questionnaire section */
		enter_questionnaire(word_array, n_input_words);
		free_word_array(word_array, n_input_words);
	}



	fclose(p_input_file);
	printf("Goodbye!\n");

	return 0;
}



/* Assumes char *word has enough memory allocated to contain the word
 * Returns the `word` pointer. 
 */
char *cz_word(const char *line, char *word)
{
    int c, i;
    int len = strlen(line);

    for (i = 0; i < strlen(line); i++) 
    {
        c = line[i];
        if (c == '\t') {
            word[i] = '\0';
            break;
        }

	/* Invalid line (missing tab) */
        if (i == len - 1) {
            return NULL;
        }

        word[i] = c;
    }
    
    return word;
}


char cz_gender(const char *line)
{
	int gender = 0L;
	int len = strlen(line);
	int i, c;

	for (i = 0; i < len; i++) {
		c = line[i];
		if (c == '\t') {
			i++;
			gender = line[i];
			return gender;
		}
	}

	return 0;
}





/* Fill the word array from the input file
 *
 * Returns the number of words in the array of structs. 
 */
int fill_word_array(struct cz_pair *pair_pt, FILE *inputf)
{
	int n_words = 0;
	int n_allocated_elements = 100;
	char line[512];
	char *pline = line;
	/* pword will be allocated over and over, divvying out the
	 * pointers to the struct array */ 
	char *pword;
	char gender;


	while (1) {
#ifdef DEBUG
		printf("Line: %d (%d elements allocated)\n", n_words, n_allocated_elements);
#endif
		/* Read line and check for EOF */
		pline = fgets(pline, 511, inputf);

		if (feof(inputf)) 
			break;

		/* Allocate 512 character string to store in the struct */
		pword = malloc(512*sizeof(char));
		if (pword == NULL) {
			fprintf(stderr, "gendercz: allocation error\n");
			free_word_array(pair_pt, n_words);
			exit(EXIT_FAILURE);
		}

		/* Parse the word and the gender */
		pword = cz_word(pline, pword);
		/* If the line was not properly formatted, pword is NULL */
		if (pword == NULL) {
			/* Free memory early and return 0 */
			free(pword);
			pword = NULL;
			free_word_array(pair_pt, n_words);
			fprintf(stderr, "Invalid line format (ln %d)\n", n_words + 1);
			return 0;
		}
		gender = cz_gender(pline);

#ifdef DEBUG
		printf("Word: %s\n    Gender: %c\n", pword, gender);
#endif
		
		/* Allocate more space if necessary */
		if (n_words > n_allocated_elements) {
			pair_pt = realloc(pair_pt, n_words + 100 * sizeof(struct cz_pair));
			n_allocated_elements += 100;
		}

		(pair_pt[n_words]).word = pword;
		(pair_pt[n_words]).gender = gender;
		n_words++;
	}

	return n_words;
}




void free_word_array(struct cz_pair *pair_pt, int n_elements)
{
	int i;

	/* Free the character strings */
	for (i = 0; i < n_elements; i++) {
		free((pair_pt[i]).word);
		(pair_pt[i]).word = NULL;
	}

	/* Free the struct array */
	free(pair_pt);
	pair_pt = NULL;
}



void enter_questionnaire(struct cz_pair *pair_pt, int n_elements)
{
	struct cz_pair *current_word = pair_pt;
	int i = 0;
	int n_correct = 0;
	int c;

	while (i < n_elements) {
		printf("\x1b[1m%s%s %s (f/m/n/q):%s ", current_word->word, PLAINTEXT, DIMTEXT, PLAINTEXT);

		while ((c = getchar()) == '\n') {
			/* Do nothing */
		}
		/* Drop the newline */
		getchar();

		if (c == 'q') {
			break;
		} else if (c == current_word->gender) {
			printf("%sGood job!%s\n", GREEN, PLAINTEXT);
			n_correct++;
		} else {
			printf("%sBad job!%s The correct gender was '%c'\n", RED, PLAINTEXT, current_word->gender);
		}

		current_word++;
		i++;
	}

	printf("\nReport: %d / %d\n", n_correct, i);
}
