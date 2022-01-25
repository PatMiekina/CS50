#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int count_grade(int grade);
int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // Prompt user for text
    string text = get_string("Text: ");

    // Calculate number of letters, words & sentences
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    // printf("%i letters\n", letters);
    // printf("%i words\n", words);
    // printf("%i sentences\n", sentences);

    // Calculate avg word length & avg sentence length in a 100 word text
    float avg_word_length = 100 * letters / (float)words;
    float avg_sentence_length = sentences * 100 / (float)words;

    // printf("%f\n", avg_word_length);
    // printf("%f\n", avg_sentence_length);

    // float grade_f = (0.0588 * avg_word_length) - (0.296 * avg_sentence_length) - 15.8;
    // printf("%f\n", grade_f);

    // Calculate the grade
    int grade = round((0.0588 * avg_word_length) - (0.296 * avg_sentence_length) - 15.8);

    // Return the grade
    // printf("%i\n", grade);
    count_grade(grade);
}

int count_letters(string text)
{
    int letters = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        int c = text[i];
        if (isalpha(c))
        {
            letters ++;
        }
    }
    return letters;
}

int count_words(string text)
{
    if (strlen(text) > 0)
    {
        int words = 1;
        for (int i = 0; i < strlen(text); i++)
        {
            int character = text[i];
            if (character == ' ')
            {
                words += 1;
            }
        }
        return words;
    }
    else
    {
        int words = 0;
        return words;
    }
}

int count_sentences(string text)
{
    int sentences = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        char c = text[i];
        if (c == '.' || c == '!' || c == '?')
        {
            sentences += 1;
        }
    }
    return sentences;
}

int count_grade(int grade)
{

    if (grade <= 16 && grade >= 1)
    {
        printf("Grade %i\n", grade);
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade 16+\n");
    }
    return 0;
}
