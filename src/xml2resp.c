
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "x2r.h"
#include "x2r_log.h"
#include "x2r_ws.h"
#include "x2r_xml.h"

#include <config.h>
#ifdef HAVE_GETOPT_H
#include <getopt.h>
#else
#include "vcs_getopt.h"
#endif
#include "log.h"


/**
 * Handle command-line options.
 *
 * We don't use getopt because Windows.
 */
static int parse_opts(int argc, char *argv[], evalresp_log_t **log, FILE **in, FILE **out) {

    int status = X2R_OK, level = 0;
    char *input = NULL, *output = NULL;
    struct option cmdline_flags[]={
        {"verbose", optional_argument, NULL, 'v'},
        {"output",  required_argument, NULL, 'o'},
        {0,0,0,0}};
    int level_auto = 1, longoptind = -1, opt;

    *in = stdin;
    *out = stdout;

    while(EOF != (opt = getopt_long(argc, argv, ":v::o:", cmdline_flags, &longoptind)) && X2R_OK == status)
    {
        switch (opt)
        {
            case 'v':
                if (optarg)
                {
                    int len = strlen(optarg), i = 0;
                    for (; i < len && isdigit(optarg[i]); i++);
                    if (i >= len)
                    {
                        level = atoi(optarg);
                        level_auto = 0;
                    }
                    else
                    {
                        /*conflicting modes for verbosity */
                        status = X2R_ERR_USER;
                    }
                }
                else if (level_auto)
                {
                    level++;
                }
                break;
            case 'o':
                if (output)
                {
                    free(output);
                }
                output = strdup(optarg);
                break;
            case ':':
                if ( 'v' == optopt || 0 == longoptind )
                {
                    /* there is a potential bug with optional args on mac */
                    level++;
                    break;
                }
                /* missing argument */
                status = X2R_ERR_USER;
                break;
            case '?':
                status = X2R_ERR_USER;
                break;
        }
        longoptind = -1;
    }
    if ( X2R_OK != status || 1 <  argc - optind )
    {
        if (output)
        {
            free(output);
        }
        return X2R_ERR_USER;
    }
    else if (argc - optind)
    {
        input = strdup(argv[optind]);
    }

    /*XXX if ((status = x2r_alloc_log(level, stderr, log)))
    {
        if (output)
        {
            free(output);
        }
        if (input)
        {
            free(input);
        }
        return status;
    } */
    evalresp_log(*log, INFO, 0, "Logging to stderr");
    /*XXX x2r_info(*log, "Logging to stderr"); */

    if (input) {
        if (!(*in = fopen(input, "r"))) {
            evalresp_log(*log, ERROR, 0, "Cannot open %s to read", input);
            status = X2R_ERR_IO;
            /*XXX status = x2r_error(*log, X2R_ERR_IO, "Cannot open %s to read", input); */
            if (output)
            {
                free(output);
            }
            free(input);
            return status;
        }
        evalresp_log(*log, INFO, 0, "Input from %s", input);
        /*XXX x2r_info(*log, "Input from %s", input); */
    } else {
        evalresp_log(*log, INFO, 0, "Input from stdin");
        /*XXX x2r_info(*log, "Input from stdin"); */
    }

    if (output) {
        if (!(*out = fopen(output, "w"))) {
            evalresp_log(*log, ERROR, 0, "Cannot open %s to write", output);
            status = X2R_ERR_IO;
            /*XXX status = x2r_error(*log, X2R_ERR_IO, "Cannot open %s to write", output); */
            if (input)
            {
                free(input);
            }
            free(output);
            return status;
        }
        evalresp_log(*log, INFO, 0, "Output to %s", output);
        /*XXX x2r_info(*log, "Output to %s", output); */
    } else {
        evalresp_log(*log, INFO, 0, "Output to stdout");
        /*XXX x2r_info(*log, "Output to stdout"); */
    }

    if (output)
    {
        free(output);
    }
    if (input)
    {
        free(input);
    }
    return status;
}


/**
 * A command-line interface to the library.  Converts a single file from station.xml
 * to response format.
 */
int main(int argc, char *argv[]) {

    int status = X2R_OK;
    /*XXX x2r_log *log = NULL; */
    evalresp_log_t *log = NULL;
    x2r_fdsn_station_xml *root = NULL;
    FILE *in = stdin, *out = stdout;

    status = parse_opts(argc, argv, &log, &in, &out);
    if (!status)
    {
        status = x2r_station_service_load(log, in, &root);
    }
    if (!status)
    {
        status = x2r_resp_util_write(log, out, root);
    }

    if (status == X2R_ERR_USER) {
        fprintf(stderr, "\nUsage:\n");
        fprintf(stderr, "\n  Specifying input and output files\n");
        fprintf(stderr, "  %s [-v] -o OUT.resp IN.xml\n", argv[0]);
        fprintf(stderr, "\n  Using stdin and stdout\n");
        fprintf(stderr, "  %s [-v] < IN.xml > OUT.resp\n", argv[0]);
        fprintf(stderr, "\n  Logging goes to stderr (multiple -v gives more detail)\n");
        fprintf(stderr, "  %s -vvvv -o OUT.resp < IN.xml 2> LOG\n\n", argv[0]);
    }
    if (in && in != stdin)
    {
        fclose(in);
    }
    if (out && out != stdout)
    {
        fclose(out);
    }
    status = x2r_free_fdsn_station_xml(root, status);
    /*XXX status = x2r_free_log(log, status); */
    return status;
}
