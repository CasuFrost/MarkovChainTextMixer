
#define WORD_LENGHT 32 /*30 byte per la parola, 1 byte per un eventuale apostrofo, il byte finale per '\0' */
#define WORDS_TYPE_SIZE 40
struct wordFrequency
{
    char word[WORD_LENGHT];
    float frequency;
};

struct words
{
    char word[WORD_LENGHT];
    struct wordFrequency *near;
};

struct words *fileWords;
int size = 0;
void initWordsArray()
{
    fileWords = malloc(40);
}
void addWord(struct words w)
{
    fileWords[size * WORDS_TYPE_SIZE] = w;
    size++;
}
