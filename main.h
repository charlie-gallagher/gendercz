#define CZ_RL_BUFSIZE 1024
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
