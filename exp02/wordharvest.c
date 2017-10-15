#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>

void search_words(){
    // search for files with the extensions .txt, .text and .asc;
    // start the search from directory /tmp/;
    // save the words found in the file words_tmp.
}

// CHAMADA: wordharvest -e txt:text:asc -d /tmp/ -o words_tmp
int main(int argc, char **argv) {
    char *eflag = NULL;
    char *dflag = NULL;
    char *ovalue = NULL;
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

    for (index = optind; index < argc; index++)
        printf ("Non-option argument %s\n", argv[index]);
    return 0;
}
