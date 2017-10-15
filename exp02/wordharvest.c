#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void search_words(char *e){
    // OK get params
    // search for files with the extensions .txt, .text and .asc;
    // start the search from directory /tmp/;
    // save the words found in the file words_tmp.
}

char ** parse_string(char *str){
    char ** res  = NULL;
    char * p = strtok (str, ":");
    int n_spaces = 0;

    /* split string and append tokens to 'res' */
    while (p) {
        res = realloc (res, sizeof (char*) * ++n_spaces);
        if (res == NULL)
            exit (-1); /* memory allocation failed */
        res[n_spaces-1] = p;
        p = strtok (NULL, ":");
    }
    /* realloc one extra element for the last NULL*/
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


    printf ("eflag = %s, dflag = %s, ovalue = %s\n",
            eflag, dflag, ovalue);
    extensions = parse_string(eflag);
    /* print the result
        for (i = 0; i < n_spaces; ++i)
            printf ("res[%d] = %s\n", i, res[i]);*/
    for (index = optind; index < argc; index++)
        printf ("Non-option argument %s\n", argv[index]);
    return 0;
}
