/*
 * Copyright 1987, 1988 by MIT Student Information Processing Board
 *
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose is hereby granted, provided that
 * the names of M.I.T. and the M.I.T. S.I.P.B. not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  M.I.T. and the
 * M.I.T. S.I.P.B. make no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 */
#include "config.h"
#include "ss_internal.h"
#include <signal.h>
#include <sys/types.h>
#include <setjmp.h>

#if defined(SVB_WIN32) && defined(SVB_MINGW)
#define NO_FORK
#endif

#ifndef NO_FORK
#include <sys/wait.h>
#endif

typedef void sigret_t;

void ss_list_requests(int argc __SS_ATTR((unused)),
		      const char * const *argv __SS_ATTR((unused)),
		      int sci_idx, void *infop __SS_ATTR((unused)))
{
    ss_request_entry *entry;
    char const * const *name;
    int i, spacing;
    ss_request_table **table;

    FILE *output;
    int fd;
#ifndef SVB_WIN32
    sigset_t omask, igmask;
#endif
    sigret_t (*func)(int);
#ifndef NO_FORK
    int waitb;
#endif
#ifndef SVB_WIN32
    sigemptyset(&igmask);
    sigaddset(&igmask, SIGINT);
    sigprocmask(SIG_BLOCK, &igmask, &omask);
#endif
    func = signal(SIGINT, SIG_IGN);
    fd = ss_pager_create();
    if (fd < 0) {
        perror("ss_pager_create");
        (void) signal(SIGINT, func);
        return;
    }
    output = fdopen(fd, "w");
#ifndef SVB_WIN32 // TODO
    sigprocmask(SIG_SETMASK, &omask, (sigset_t *) 0);
#endif
    fprintf (output, "Available %s requests:\n\n",
	     ss_info (sci_idx) -> subsystem_name);

    for (table = ss_info(sci_idx)->rqt_tables; *table; table++) {
        entry = (*table)->requests;
        for (; entry->command_names; entry++) {
            spacing = -2;
            if (entry->flags & SS_OPT_DONT_LIST)
                continue;
            for (name = entry->command_names; *name; name++) {
                int len = strlen(*name);
                fputs(*name, output);
                spacing += len + 2;
                if (name[1]) {
                    fputs(", ", output);
                }
            }
            if (spacing > 23) {
                fputc('\n', output);
                spacing = 0;
            }
            for (i = 0; i < 25 - spacing; i++)
                fputc(' ', output);
            fputs(entry->info_string, output);
            fputc('\n', output);
        }
    }
    fclose(output);
#ifndef NO_FORK
    wait(&waitb);
#endif
    (void) signal(SIGINT, func);
}
