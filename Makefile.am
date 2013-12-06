bin_PROGRAMS = gnome-album-thumbnailer

gnome_album_thumbnailer_SOURCES = gnome-album-thumbnailer.c gnome-thumbnailer-skeleton.h gnome-thumbnailer-skeleton.c
gnome_album_thumbnailer_CPPFLAGS = \
	$(THUMBNAILER_CFLAGS) \
	-DTHUMBNAILER_RETURNS_PIXBUF \
	-DTHUMBNAILER_USAGE="\"Thumbnail album folders\"" \
	$(WARN_CFLAGS)
gnome_album_thumbnailer_LDADD = $(THUMBNAILER_LIBS)

thumbnailerdir = $(datadir)/thumbnailers/
thumbnailer_DATA = gnome-album-thumbnailer.thumbnailer
gnome-album-thumbnailer.thumbnailer: gnome-album-thumbnailer.thumbnailer.in Makefile
	$(AM_V_GEN) $(SED) -e "s|\@bindir\@|$(bindir)|" $< > $@

EXTRA_DIST = gnome-album-thumbnailer.thumbnailer.in AUTHORS COPYING README

CLEANFILES = gnome-album-thumbnailer.thumbnailer

