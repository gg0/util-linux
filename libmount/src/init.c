/*
 * Copyright (C) 2008 Karel Zak <kzak@redhat.com>
 *
 * This file may be redistributed under the terms of the
 * GNU Lesser General Public License.
 */

/**
 * SECTION: init
 * @title: Library initialization
 * @short_description: initialize debugging
 */

#include <stdarg.h>

#include "mountP.h"

UL_DEBUG_DEFINE_MASK(libmount);
UL_DEBUG_DEFINE_MASKNAMES(libmount) =
{
	{ "all", MNT_DEBUG_ALL,		"info about all subsystems" },
	{ "cache", MNT_DEBUG_CACHE,	"paths and tags cache" },
	{ "cxt", MNT_DEBUG_CXT,		"library context (handler)" },
	{ "diff", MNT_DEBUG_DIFF,	"mountinfo changes tracking" },
	{ "fs", MNT_DEBUG_FS,		"FS abstraction" },
	{ "help", MNT_DEBUG_HELP,	"this help" },
	{ "locks", MNT_DEBUG_LOCKS,	"mtab and utab locking" },
	{ "options", MNT_DEBUG_OPTIONS,	"mount options parsing" },
	{ "tab", MNT_DEBUG_TAB,		"fstab, mtab, moutninfo routines" },
	{ "update", MNT_DEBUG_UPDATE,	"mtab, utab updates" },
	{ "utils", MNT_DEBUG_UTILS,	"misc library utils" },
	{ NULL, 0 }
};

/**
 * mnt_init_debug:
 * @mask: debug mask (0xffff to enable full debugging)
 *
 * If the @mask is not specified, then this function reads
 * the LIBMOUNT_DEBUG environment variable to get the mask.
 *
 * Already initialized debugging stuff cannot be changed. Calling
 * this function twice has no effect.
 */
void mnt_init_debug(int mask)
{
	if (libmount_debug_mask)
		return;

	__UL_INIT_DEBUG(libmount, MNT_DEBUG_, mask, LIBMOUNT_DEBUG);

	if (libmount_debug_mask != MNT_DEBUG_INIT
	    && libmount_debug_mask != (MNT_DEBUG_HELP|MNT_DEBUG_INIT)) {
		const char *ver = NULL;
		const char **features = NULL, **p;

		mnt_get_library_version(&ver);
		mnt_get_library_features(&features);

		DBG(INIT, ul_debug("library debug mask: 0x%04x", libmount_debug_mask));
		DBG(INIT, ul_debug("library version: %s", ver));
		p = features;
		while (p && *p)
			DBG(INIT, ul_debug("    feature: %s", *p++));
	}

	ON_DBG(HELP, ul_debug_print_masks("LIBMOUNT_DEBUG",
				UL_DEBUG_MASKNAMES(libmount)));
}

#ifdef TEST_PROGRAM

#include <errno.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
	if (argc == 2) {
		int mask;

		errno = 0;
		mask = strtoul(argv[1], 0, 0);

		if (errno)
			return 1;

		mnt_init_debug(mask);
	}
	else if (argc == 1) {
		mnt_init_debug(0);
	} else
		return 1;

	return 0;
}
#endif /* TEST_PROGRAM */

