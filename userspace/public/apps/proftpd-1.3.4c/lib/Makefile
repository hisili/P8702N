

top_srcdir=..
srcdir=.


include ../Make.rules

LIB_DEPS=""
LIB_OBJS=pr_fnmatch.o sstrncpy.o strsep.o vsnprintf.o glibc-glob.o glibc-hstrerror.o glibc-mkstemp.o pr-syslog.o pwgrent.o tpl.o

"":

dummy:

all:
	@echo "Running make from top level directory."
	cd ../ && $(MAKE) all

#Makefile: Makefile.in ../config.status
#	cd ../ && ./config.status

.c.o:
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

libltdl: dummy
	cd libltdl/ && $(MAKE)

libsupp.a: $(LIB_OBJS)
	$(MAKE) -C libcap
	$(AR) rc libsupp.a $(LIB_OBJS)
	$(RANLIB) libsupp.a

lib: libsupp.a $(LIB_DEPS)

install:

clean:
	(cd libcap/ && $(MAKE) clean)
	rm -f *.o libsupp.a

depend:
	makedepend $(CPPFLAGS) -Y *.c 2>/dev/null
	makedepend $(CPPFLAGS) -Y -fMakefile.in *.c 2>/dev/null

distclean:
	test -z $(LIB_DEPS) || (cd libltdl/ && $(MAKE) distclean)
# DO NOT DELETE

getopt1.o: ../config.h getopt.h
getopt.o: ../config.h getopt.h
glibc-glob.o: ../config.h ../include/options.h ../include/glibc-glob.h
glibc-hstrerror.o: ../include/conf.h ../include/version.h ../config.h
glibc-hstrerror.o: ../include/default_paths.h ../include/options.h
glibc-hstrerror.o: ../include/libsupp.h ../include/glibc-glob.h
glibc-mkstemp.o: ../include/conf.h ../include/version.h ../config.h
glibc-mkstemp.o: ../include/default_paths.h ../include/options.h
glibc-mkstemp.o: ../include/libsupp.h ../include/glibc-glob.h
pr_fnmatch.o: ../config.h ../include/conf.h ../include/version.h
pr_fnmatch.o: ../include/default_paths.h ../include/options.h
pr_fnmatch.o: ../include/libsupp.h ../include/glibc-glob.h pr_fnmatch_loop.c
pr_fnmatch_loop.o: ../config.h
pr-syslog.o: ../include/conf.h ../include/version.h ../config.h
pr-syslog.o: ../include/default_paths.h ../include/options.h
pwgrent.o: ../include/conf.h ../include/version.h ../config.h
pwgrent.o: ../include/default_paths.h ../include/options.h
pwgrent.o: ../include/libsupp.h ../include/glibc-glob.h
sstrncpy.o: ../config.h ../include/libsupp.h ../include/glibc-glob.h
strsep.o: ../include/conf.h ../include/version.h ../config.h
strsep.o: ../include/default_paths.h ../include/options.h
strsep.o: ../include/libsupp.h ../include/glibc-glob.h
tpl.o: ../include/tpl.h
vsnprintf.o: ../include/conf.h ../include/version.h ../config.h
vsnprintf.o: ../include/default_paths.h ../include/options.h
vsnprintf.o: ../include/libsupp.h ../include/glibc-glob.h
