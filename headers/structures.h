
#define WORD_LENGHT 32 /*30 byte per la parola, 1 byte per un eventuale apostrofo, il byte finale per '\0' */

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