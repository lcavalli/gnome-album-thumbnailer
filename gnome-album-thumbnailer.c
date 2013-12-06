/*
 * Copyright (C) 2013 Luca Cavalli <luca.cavalli@gmail.com>
 *
 * Authors: Luca Cavalli <luca.cavalli@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */

#include <string.h>
#include <glib.h>
#include <gio/gio.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "gnome-thumbnailer-skeleton.h"

static GdkPixbuf *
get_cover_from_image (GFile *folder, GError **error)
{
	GdkPixbuf *pixbuf = NULL;
	GFileEnumerator *enumerator;

	enumerator = g_file_enumerate_children (folder, G_FILE_ATTRIBUTE_STANDARD_NAME, \
	                                        G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, NULL, error);

	if (enumerator) {
		GFileInfo *file_info;
		GFileInputStream *stream;
		GFile *file;
		GRegex *regex;
		const gchar *file_name;
		gchar *path;
		gchar *cover_file_name;

		regex = g_regex_new (".*cover|album|folder.*\\.(jpg|jpeg|png)", G_REGEX_CASELESS, 0, NULL);

		while ((file_info = g_file_enumerator_next_file (enumerator, NULL, error)) != NULL) {
			file_name = g_file_info_get_name (file_info);

			if (g_regex_match (regex, file_name, 0, NULL)) {
				path = g_file_get_path (folder);
				cover_file_name = g_build_path (G_DIR_SEPARATOR_S, path, file_name, NULL);
				file = g_file_new_for_path (cover_file_name);

				g_free (cover_file_name);
				g_free (path);

				stream = g_file_read (file, NULL, error);

				g_object_unref (file);

				pixbuf = gdk_pixbuf_new_from_stream_at_scale (G_INPUT_STREAM (stream), \
				                                              256, 256, TRUE, NULL, error);

				g_object_unref (stream);
				g_object_unref (file_info);
				break;
			}

			g_object_unref (file_info);
		}

		g_regex_unref (regex);

		g_file_enumerator_close (enumerator, NULL, NULL);
		g_object_unref (enumerator);
	}

	/* In case of error set error message if not already done */
	if (pixbuf == NULL) {
		if (*error == NULL) {
			g_set_error_literal (error, G_IO_ERROR, G_IO_ERROR_FAILED, "Could not find cover");
		}
	}

	return pixbuf;
}

static GdkPixbuf *
get_cover_from_id3tag (GFile *folder, GError **error)
{
	GdkPixbuf *pixbuf = NULL;

	/* TODO: Implement cover extraction from MP3 ID3v2 tag */
	/* TODO: Check all covers. If they are the same, then use it, discard otherwise */

	/* In case of error set error message if not already done */
	if (pixbuf == NULL) {
		if (*error == NULL) {
			g_set_error (error, G_FILE_ERROR, 0, "Cover not found");
		}
	}

	return pixbuf;
}

GdkPixbuf *
file_to_pixbuf (const char *path, GError **error)
{
	GFile *input;
	GdkPixbuf *pixbuf;

	/* Open the file for reading */
	input = g_file_new_for_path (path);

	/* Look for cover image */
	pixbuf = get_cover_from_image (input, error);
	if (pixbuf != NULL) {
		goto CLEANUP;
	}

	/* Look for MP3 embedded cover */
	pixbuf = get_cover_from_id3tag (input, error);
	if (pixbuf != NULL) {
		goto CLEANUP;
	}

CLEANUP:
	g_object_unref (input);

	return pixbuf;
}

