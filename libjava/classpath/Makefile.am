## Input file for automake to generate the Makefile.in used by configure

# lib first, to compile .class files before native code, last examples
## GCJ LOCAL: we remove 'tools' for the time being.
## We don't build any of them into actual tools yet.
SUBDIRS = lib doc external include native resource scripts $(EXAMPLESDIR)
DIST_SUBDIRS = lib doc external include native resource scripts examples

## GCJ LOCAL: we need an extra -I here.
ACLOCAL_AMFLAGS = -I m4 -I ../.. -I ../../config

native: lib

EXTRA_DIST = HACKING BUGS THANKYOU mauve-classpath LICENSE \
             ChangeLog-2003 ChangeLog-2004 ChangeLog-2005 \
             ChangeLog.gnujaxp.1 ChangeLog.gnujaxp.2 ChangeLog.libxmlj \
	     autogen.sh

#DISTCHECK_CONFIGURE_FLAGS = --enable-gjdoc

dist-hook:
	-$(mkinstalldirs) $(distdir)/include
	-cp -f $(top_srcdir)/include/*.in $(distdir)/include
	-cp -f $(top_srcdir)/include/*.h $(distdir)/include

ChangeLog:
	cvs2cl.pl -b --gmt --log-opts "-d'1998-06-30< now'" --window 7200 --usermap ChangeLog.usermap --ignore ChangeLog --fsf
