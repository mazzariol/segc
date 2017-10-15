#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// OK   get params
// OK   search for files with the extensions .txt, .text and .asc;
// OK   start the search from directory /tmp/;
// save the words found in the file words_tmp

char *tmp_file = "/tmp/out.log";

char ** read_data_from_file(const char *filename);
char* concat(const char *s1, const char *s2);
void search_files(char **extensions, char *dir);
char ** parse_string(char *str, char *delimiter);
char ** read_words_from_file(const char *filename);

char ** read_words_from_file(const char *filename){
    char ** lines = NULL;
    FILE *fp;
    int ch;

    fp = fopen(filename,"r");
    if(fp == NULL) {
        perror("Error in opening file");
        return NULL;
    }
    while((ch = (char)fgetc(fp) ) != EOF) {
        if( feof(fp)) {
            break ;
        }
        // Consider char between 0-9 || A-Z || a-z
        if ((48 <= ch && ch <= 57) || (65 <= ch && ch <= 90) || (97 <= ch && ch <= 122)){
            printf("%c", ch);
        } else {
            printf("\n");
        }
    }

    fclose(fp);

    return lines;
}

char ** read_data_from_file(const char *filename){
    sleep(1);
    char info[50];
    FILE *arq;

    arq = fopen(filename, "r");
    if(arq == NULL)
        printf("Erro, nao foi possivel abrir o arquivo\n");
    else
        while( (fgets(info, sizeof(info), arq))!=NULL ) {
            printf("%s", info);
            if (info[strlen(info) -1] == '\n')
                info[strlen(info) -1] = '\0';
            read_words_from_file(info);
        }

    fclose(arq);

    return 0;
}

char* concat(const char *s1, const char *s2) {
    char *result = malloc(strlen(s1)+strlen(s2)+1);
    strcpy(result, s1);
    strcat(result, s2);

    return result;
}

void search_files(char **extensions, char *dir){
    char *new_str;
    int i=0;
    new_str = concat("find ", dir);
    new_str = concat(new_str, " -regex '.*\\.\\(");

    while (extensions[i] != NULL){
        new_str = concat(new_str, extensions[i]);
        new_str = concat(new_str, "\\|");
        i ++;
    }
    new_str = concat(new_str, "\\)'");
    new_str = concat(new_str, " > ");
    new_str = concat(new_str, tmp_file);

    // execute 'find' command
    popen(new_str, "r");
    free(new_str);
}

/**
 * Parse string from delimiter
 * */
char ** parse_string(char *str, char *delimiter){
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
// CHAMADA: wordharvest -e txt:text:asc -d /tmp/ -o words_tmp
int main(int argc, char **argv) {
    char * eflag = NULL;
    char * dflag = NULL;
    char * ovalue = NULL;
    char ** extensions = NULL;
    int index;
    int c;

    opterr = 0;

    while ((c = getopt (argc, argv, "e:d:o:")) != -1)
        switch (c)
        {
            case 'e':
                eflag = optarg;
                break;
            case 'd':
                dflag = optarg;
                break;
            case 'o':
                ovalue = optarg;
                break;
            case '?':
                if (optopt == 'c')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr,
                             "Unknown option character `\\x%x'.\n",
                             optopt);
                return 1;
            default:
                abort ();
        }

    extensions = parse_string(eflag, ":");

    search_files(extensions, dflag);

    read_data_from_file(tmp_file);
    /* print the result
        for (i = 0; i < n_spaces; ++i)
            printf ("res[%d] = %s\n", i, res[i]);*/
    for (index = optind; index < argc; index++)
        printf ("Non-option argument %s\n", argv[index]);

    free(extensions);
    return 0;
}
