/*
 * This file is part of usysconf.
 *
 * Copyright © 2017 Solus Project
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#define _GNU_SOURCE

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "context.h"
#include "files.h"
#include "util.h"

static const char *library_paths[] = {
        /* Match all library paths directories and run ldconfig for them */
        "/usr/lib64", "/usr/lib32", "/usr/lib", "/lib32", "/lib64", "/lib",
};

/**
 * Update the icon cache on disk for every icon them found in the given directory
 */
static UscHandlerStatus usc_handler_ldconfig_exec(__usc_unused__ UscContext *ctx, const char *path)
{
        char *command[] = {
                "/sbin/ldconfig",
                "-X", /* don't update symlinks */
                NULL, /* Terminator */
        };

        if (!usc_file_is_dir(path)) {
                return USC_HANDLER_SKIP;
        }

        fprintf(stderr, "Checking %s\n", path);
        int ret = usc_exec_command(command);
        if (ret != 0) {
                fprintf(stderr, "Ohnoes\n");
                return USC_HANDLER_FAIL | USC_HANDLER_BREAK;
        }
        /* Only want to run once for all of our globs */
        return USC_HANDLER_SUCCESS | USC_HANDLER_BREAK;
}

const UscHandler usc_handler_ldconfig = {
        .name = "ldconfig",
        .exec = usc_handler_ldconfig_exec,
        .paths = library_paths,
        .n_paths = ARRAY_SIZE(library_paths),
};

/*
 * Editor modelines  -  https://www.wireshark.org/tools/modelines.html
 *
 * Local variables:
 * c-basic-offset: 8
 * tab-width: 8
 * indent-tabs-mode: nil
 * End:
 *
 * vi: set shiftwidth=8 tabstop=8 expandtab:
 * :indentSize=8:tabSize=8:noTabs=true:
 */
