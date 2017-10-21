#include <stdio.h>
#include <stdlib.h> /* malloc */
#include <string.h> /* strlen, strcpy, strncpy */
#include <ctype.h> /* isalpha, isdigit */
#include <unistd.h> /* getopt */

/**
 * Marcelo Machado      - RA 210402
 * Guilherme Mazzariol  - RA 138466
 *
 * Given a directory and some extensions, get recursively files with that extensions
 * inside the directory. For each files, get 'words', beings 'word' defined by a
 * sequence of letters + numbers. Every other char is considered a separator.
 *
 * If no extensions are given, .txt and .text should be considered.
 * Only new words should be added to the list.
 */

// define max size for words
#define MAX 999

// linked list definition
typedef struct node {
    char word[MAX];
    struct node *next;
} Node;

// global variables
Node* l_words;

// utilities
char ** parse_string(char *str, char *delimiter);
char* concat(const char *s1, const char *s2);

// important functions
void find_files_in_dir(char *dir, char *ext);
void find_words_in_file(char *filename);

// list functions
void list_to_file(Node* l, char *output_filename);
void push(Node* l, char *word);

// main function
int main (int argc, char **argv) {
    int opt, eflag = 0;
    char *working_dir = NULL;
    char *extensions = NULL;
    char *output_filename = NULL;

    // init list
    l_words = NULL;
    l_words = malloc(sizeof(Node));

    // parse arguments: extensions, working dir, output filename
    while ((opt = getopt(argc, argv, "d:e:o:")) != -1) {
        switch(opt) {
        case 'd':
            working_dir = optarg;
            break;
        case 'e':
            eflag = 1;
            extensions = optarg;
            break;
        case 'o':
            output_filename = optarg;
            break;
        default:
            abort();
        }
    }

    // extensions not provided: use default extensions
    if (!eflag) {
        extensions = (char*)malloc(9*sizeof(char));
        strcpy(extensions, "txt:text");
    }

    find_files_in_dir(working_dir, extensions);
    list_to_file(l_words, output_filename);

    return 0;
}


char ** parse_string(char *str, char *delimiter){
    /**
     * Given a delimiter, parse a string

     * @param char *str string to be parsed
     * @param char *delimiter
     * @return array with contains each peace of string
     */
    char ** res  = NULL;
    char * p = strtok (str, delimiter);
    int n_spaces = 0;

    while (p) {
        res = realloc (res, sizeof (char*) * ++n_spaces);
        if (res == NULL)
            exit (-1);
        res[n_spaces-1] = p;
        p = strtok (NULL, delimiter);
    }

    res = realloc (res, sizeof (char*) * (n_spaces+1));
    res[n_spaces] = 0;

    return res;
}

char* concat(const char *s1, const char *s2) {
    /**
     * Concat two strings

     * @param char *s1 first string
     * @param char *s1 second string
     * @return concatenate string
     */
    char *result = malloc(strlen(s1)+strlen(s2)+1);
    strcpy(result, s1);
    strcat(result, s2);

    return result;
}

void find_files_in_dir (char *dir, char *ext) {
    /**
     * Given a dir, list (recursively) files inside it
     * by calling external application 'find'.

     * @param char *dir name of the dir where to look at
     * @param char *ext list of extensions to look at, separated by '|'
     */

    FILE *fp;
    char cmd[1024];
    char path[1035];
    char ** extensions = parse_string(ext, ":");
    char *command;
    int i=0;

    command = concat("find ", dir);
    command = concat(command, " -regex '.*\\.\\(");

    while (extensions[i] != NULL){
        command = concat(command, extensions[i]);
        command = concat(command, "\\|");
        i ++;
    }
    command = concat(command, "\\)'");

    // compose external command to be run
    snprintf(cmd, sizeof(cmd), command);
    // run external command
    fp = popen(cmd,"r");
    // check for errors
    if (fp == NULL) {
        printf("Failed to run command\n");
        exit(1);
    }

    // iterate through output of 'find' command
    while (fgets(path, sizeof(path)-1, fp) != NULL) {
        path[strlen(path)-1] = 0;
        // find words in each interesting file
        find_words_in_file(path);
    }

    pclose(fp);
}

void find_words_in_file(char *filename) {
    /**
     * Find 'words' (sequences of letters and/or numbers) inside
     * a file, given its path.
     *
     * @param char *filename path of the file where to look at
     */

    int i;
    char c;
    char str[MAX];
    char word[MAX];
    FILE *fp;

    fp = fopen(filename, "r");
    if (fp) {
        i = 0;

        // read each char in file fp
        while ((c = getc(fp)) != EOF) {
            // found a letter or a digit: save to word
            if (isalpha(c) || isdigit(c)) {
                str[i] = c;
                i++;
            }
            // found a separator: save word and reset i-counter
            else {
                // this condition handles consecutive separators
                if (i) {
                    strncpy(word, str, i);
                    // add string terminator to word
                    word[i] = '\0';
                    // add words to list
                    push(l_words, word);
                }
                i = 0;
            }
        }
        fclose(fp);
    }
}

void list_to_file(Node* l, char *output_filename) {
    /**
     * Write elements from list to file.
     *
     * @param Node* l list of strings
     * @param char *output_filename file to be written
     */

    FILE *fp;
    Node* current;

    // try to open output file
    fp = fopen(output_filename, "w");
    if (fp == NULL) {
        printf("Failed to write file.\n");
        exit(1);
    }

    // walk through list and write words to file
    current = l;
    while (current->next != NULL) {
        fprintf(fp, "%s\n", current->word);
        current = current->next;
    }
    fclose(fp);
}

void push(Node *l, char word[]) {
    /**
     * Add a new word to list of words.
     *
     * @param Node* l list of strings
     * @param char word[] word to be added
     */

    Node *current, *previous;

    current = l;
    previous = NULL;
    // walk through list to add word at the end
    while (current != NULL) {
        previous = current;
        // word is already in list: do not add it
        if (strcmp(word, current->word) == 0)
            return;
        current = current->next;
    }

    // add new word to the end of the list
    previous->next = malloc(sizeof(Node));
    strcpy(previous->next->word, word);
    previous->next->next = NULL;

}
