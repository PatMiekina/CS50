from cs50 import get_string

# Count letters


def count_letters(text):
    letters = 0
    for i in range(len(text)):
        c = text[i]
        if c.isalpha():
            letters += 1
    return letters


# Count words
def count_words(text):
    if len(text) > 0:
        words = 1
        for i in range(len(text)):
            character = text[i]
            if character == " ":
                words += 1
            i += 1
        return words
    else:
        words = 0
        return words

# Count sentences


def count_sentences(text):
    sentences = 0
    for i in range(len(text)):
        c = text[i]
        if c == "." or c == "!" or c == "?":
            sentences += 1
    return sentences

# Count grade


def count_grade(grade):
    if grade <= 16 and grade >= 1:
        print("Grade " + str(grade))
    elif grade < 1:
        print("Before Grade 1")
    else:
        print("Grade 16+")


def main():
    # Ask user for input
    text = get_string("Text: ")

    letters = count_letters(text)
    # print(letters)
    words = count_words(text)
    # print(words)
    sentences = count_sentences(text)
    print(sentences)

    # Count average word & sentence length
    avg_word_length = (letters * 100) / words
    avg_sentence_length = (sentences * 100) / words

    # Count grade
    grade = int(round((0.0588 * avg_word_length) - (0.296 * avg_sentence_length) - 15.8))
    # print(grade)

    count_grade(grade)


main()