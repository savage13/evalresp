#include <syslog.h>
#include "../log.h"
#include <stdlib.h>
#include "to_syslog.h"

int evalresp_log_intialize_log_for_syslog(evalresp_log_t *log, evalresp_syslog_data_t *data)
{
    if (!log)
    {
        return EXIT_FAILURE;
    }
    log->log_func=evalresp_log_to_syslog;
    log->func_data = (void *) data;
    return EXIT_SUCCESS;
}
evalresp_syslog_t * evalresp_log_syslog_data_alloc(char *ident, int option, int facility)
{
    evalresp_syslog_t *log_opt = (evalresp_syslog_t *)calloc(1, sizeof(evalresp_syslog_t));

    if (!log_opt)
    {
        return NULL;
    }
    log_opt->ident = ident ;
    log_opt->option = option ;
    log_opt->facility = facility ;
    return log_opt;
}
void evalresp_log_syslog_data_free(evalresp_syslog_t *log_opt)
{
    if (!log_opt)
    {
        return;
    }
    free(log_opt);
}

int evalresp_log_to_syslog(evalresp_log_msg_t *msg, void *data)
{
    evalresp_syslog_data_t *log_opt = data;
    int option, facility, level;
    char *ident;
    if (!msg)
    {
        return -1;
    }
    if (log_opt)
    {
        ident = log_opt->ident;
        option = log_opt->option;
        facility = log_opt->facility;
    }
    else
    {
        ident = "evalresp";
        option =LOG_PERROR | LOG_PID;
        facility = LOG_USER;
    }
    openlog(ident, option, facility);
    switch (msg->log_level)
    {
        case ERROR:
            level = LOG_ERR;
            break;
        case WARN:
            level = LOG_WARNING;
            break;
        case DEBUG:
            level = LOG_DEBUG;
        default:
            level = LOG_INFO;
    }
    syslog(level, "%s", msg->msg);
    closelog();
    return EXIT_SUCCESS;
}
