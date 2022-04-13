#define CZ_RL_BUFSIZE 1024
#define PLAINTEXT "\x1b[0m"
#define DIMTEXT "\x1b[2m"
#define RED "\x1b[1;31m"
#define GREEN "\x1b[1;32m"
char *cz_word(const char *line, char *word);
char cz_gender(const char *line);

struct cz_pair
{
	char *word;
	int gender;
};

int fill_word_array(struct cz_pair *pair_pt, FILE *inputf);
void enter_questionnaire(struct cz_pair *pair_pt, int n_elements);
void free_word_array(struct cz_pair *pair_pt, int n_elements);
