#include "progressbar.h"

/* Initialize the progress bar */
ProgressBar init_progress_bar (unsigned long maxlength, char left, char right, char fill, char empty) {
    ProgressBar p;
    p.maxlength = maxlength;
    p.curlength = 0;

    p.leftmargin = left;
    p.rightmargin = right;
    p.fillchar = fill;
    p.emptychar = empty;
    return p;
}

/* Get the current time */
time_t get_time() {
    time_t rawtime;
    time ( &rawtime );
    return rawtime;
}

/* Create the timestamp string from a time. */
char* timestamp (time_t rawtime) {
    struct tm * ptm;
    ptm = gmtime ( &rawtime );
    char* result = (char*)malloc(5 * sizeof(char));
    sprintf (result, "%2d:%02d", (ptm->tm_hour+PDT)%24, ptm->tm_min);
    return result;
}

/* Draw the progress bar with timestamps */
void draw_progress_bar (ProgressBar* p, time_t time_beg, time_t time_end) {
    /** Move to the beginning of the line. */
    printf("\r");

    /** Print the current timestamp and the end timestamp */
    printf("%s / %s", timestamp(time_beg), timestamp(time_end));

    /** Print the left margin char. */
    printf("%c", p->leftmargin);

    /** Make sure that maxlength >= curlength */
    if (p->maxlength < p->curlength)
        p->curlength = p->curlength;

    /** Print the progress with the Fill char. */
    for(unsigned long i = 0; i < p->curlength; i++)
        printf("%c", p->fillchar);

    /** Complete whats left with the Fill char. */
    for(unsigned long i = 0; i < p->maxlength - p->curlength; i++)
        printf("%c", p->emptychar);

    /** Print the right margin char. */
    printf("%c", p->rightmargin);

    /** Show the updated progress bar */
    fflush(stdout);
}
