#ifndef _TASK_HELPER_
#define _TASK_HELPER_

/*
 * Load a file inside a given buffer
 * @param file - name of the file to be read
 * @param buffer - initialised vector to hold the content of the file
 */
void load_file(const char *file, char *buffer);

/*
 * Returns a list of synonym words and its length based on a given word.
 * @param word - the given word
 * @param n - the length of the returned list
 * @param word_list - returns the pointer to a list filled with synonym words having the length n
 */
void get_synonym(const char *word, int *n, char ***word_list);

/*
 * Receives a word and returns the pointer to a location of memory that holds a friendly word
 * @param word - the given word
 * @param friendly_word - returns the pointer to a memory where the word is located
 */
void get_friendly_word(const char *word, char **friendly_word);

#endif