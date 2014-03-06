/*
    Purpose:  Make using latex easier.  The input should come via a pipe
              from latex running on the .tex file you are interested in.
              The program will look through that output and determine
              what further calls to pdflatex or bibtex need to made on the
              .tex file.  Also filters out latex's output to only show
              warnings and errors.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*  These are dependent on your version of latex.  */

#define WARNING "LaTeX Warning"
#define ERROR   "! LaTeX Error"
#define BIBTEX1 "Citation"
#define BIBTEX2 "undefined"
#define LATEX   "Label(s) may have changed."

char *pgm_name = NULL;
char *lat_name = NULL;

/*  Run either bibtex or latex on the .tex file.
*/
void command(char *com)
{
    char outbuf[256];

    printf("Running %s on %s.tex...\n", com, lat_name);
    sprintf(outbuf, "%s %s | egrep \"%s|%s\"", com, lat_name, WARNING, ERROR);
    system(outbuf);
}

/*  The argument to latex_make should be the name of the .tex file without
    the .tex extension.
*/
int main(int argc, char **argv)
{
    FILE  *fp;
    char   inbuf[256];
    int    warning;
    int    need_bib = 0;
    int    need_lat = 0;

    pgm_name = argv[0];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <tex file>\n", pgm_name);
        exit(0);
    }

    lat_name = argv[1];

    printf("Now building %s.pdf...\n", lat_name);

    while (fgets(inbuf, 256, stdin)) {
        if (strstr(inbuf, ERROR)) {
            printf("%s", inbuf);
            exit(0);
        }
        if (strstr(inbuf, WARNING)) {
            printf("%s", inbuf);
            if (strstr(inbuf, BIBTEX1) && strstr(inbuf, BIBTEX2))
                need_bib = 1;
            if (strstr(inbuf, LATEX))
                need_lat = 1;
        }
    }

    if (need_bib) {
        command("bibtex");
        command("pdflatex");
        command("pdflatex");
        command("pdflatex");
    }

    if (need_lat)
        command("pdflatex");
        command("pdflatex");
        command("pdflatex");

    exit(0);
}

//END ALL

