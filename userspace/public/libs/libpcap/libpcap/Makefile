# Generated automatically from Makefile.in by configure.
#  Copyright (c) 1993, 1994, 1995, 1996
# 	The Regents of the University of California.  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that: (1) source code distributions
#  retain the above copyright notice and this paragraph in its entirety, (2)
#  distributions including binary code include the above copyright notice and
#  this paragraph in its entirety in the documentation or other materials
#  provided with the distribution, and (3) all advertising materials mentioning
#  features or use of this software display the following acknowledgement:
#  ``This product includes software developed by the University of California,
#  Lawrence Berkeley Laboratory and its contributors.'' Neither the name of
#  the University nor the names of its contributors may be used to endorse
#  or promote products derived from this software without specific prior
#  written permission.
#  THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
#  WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
#  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
#
# @(#) $Header: /tcpdump/master/libpcap/Makefile.in,v 1.89 2002/07/27 18:45:34 guy Exp $ (LBL)

#
# Various configurable paths (remember to edit Makefile.in, not Makefile)
#

#
# Set our CommEngine directory (by splitting the pwd into two words
# at /userspace and taking the first word only).
# Then include the common defines under CommEngine.
#
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/make.common


LIB=libpcap.so.1.1.1

# Top level hierarchy
prefix = /usr/local
exec_prefix = ${prefix}
datarootdir = ${prefix}/share
# Pathname of directory to install the configure program
bindir = ${exec_prefix}/bin
# Pathname of directory to install the include files
includedir = ${prefix}/include
# Pathname of directory to install the library
libdir =  ${exec_prefix}/lib
# Pathname of directory to install the man pages
mandir = ${datarootdir}/man

# VPATH
srcdir = .


#
# You shouldn't need to edit anything below.
#

# LD = /usr/bin/ld
# CC = /opt/toolchains/uclibc-crosstools-gcc-4.4.2-1/usr/bin/mips-linux-uclibc-gcc
CCOPT = -O2 -fpic
INCLS = -I. 
DEFS = -DHAVE_CONFIG_H -DREMOTE -DHAVE_STRERROR -DHAVE_PCAPREADEX

# Standard CFLAGS
CFLAGS = $(CCOPT) $(INCLS) $(DEFS)

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644
# RANLIB = /opt/toolchains/uclibc-crosstools-gcc-4.4.2-1/usr/bin/mips-linux-uclibc-ranlib

#
# Flex and bison allow you to specify the prefixes of the global symbols
# used by the generated parser.  This allows programs to use lex/yacc
# and link against libpcap.  If you don't have flex or bison, get them.
#
LEX = flex -Ppcap_
YACC = bison -y -p pcap_

# Explicitly define compilation rule since SunOS 4's make doesn't like gcc.
# Also, gcc does not remove the .o before forking 'as', which can be a
# problem if you don't own the file but can write to the directory.
.c.o:
	@rm -f $@
	$(CC) $(CFLAGS) -c $(srcdir)/$*.c

PSRC =	pcap-linux.c
FSRC =  fad-getad.c
FSRC =  fad-gifc.c
CSRC =	pcap.c inet.c gencode.c optimize.c nametoaddr.c \
	etherent.c savefile.c bpf_filter.c bpf_image.c bpf_dump.c \
	pcap-remote.c pcap-new.c sockutils.c 

GENSRC = scanner.c grammar.c version.c
LIBOBJS = @LIBOBJS@

SRC =	$(PSRC) $(FSRC) $(CSRC) $(GENSRC)

# We would like to say "OBJ = $(SRC:.c=.o)" but Ultrix's make cannot
# hack the extra indirection
OBJ =	$(PSRC:.c=.o) $(FSRC:.c=.o) $(CSRC:.c=.o) $(GENSRC:.c=.o) # $(LIBOBJS)
HDR =	pcap.h pcap-int.h pcap-namedb.h pcap-nit.h pcap-pf.h \
	ethertype.h gencode.h gnuc.h
GENHDR = \
	tokdefs.h

TAGHDR = \
	bpf/net/bpf.h

TAGFILES = \
	$(SRC) $(HDR) $(TAGHDR)

CLEANFILES = $(OBJ) libpcap.a $(GENSRC) $(GENHDR) lex.yy.c

all: libpcap.a

libpcap.a: $(OBJ)
	@rm -f $@
	ar rc $@ $(OBJ)
	$(RANLIB) $@

scanner.c: $(srcdir)/scanner.l
	@rm -f $@
	$(LEX) -t $< > $$$$.$@; mv $$$$.$@ $@

scanner.o: scanner.c tokdefs.h
	$(CC) $(CFLAGS) -c scanner.c

tokdefs.h: grammar.c
grammar.c: $(srcdir)/grammar.y
	@rm -f grammar.c tokdefs.h
	$(YACC) -d $<
	mv y.tab.c grammar.c
	mv y.tab.h tokdefs.h

grammar.o: grammar.c
	@rm -f $@
	$(CC) $(CFLAGS) -Dyylval=pcap_lval -c grammar.c

version.o: version.c
	$(CC) $(CFLAGS) -c version.c

snprintf.o: $(srcdir)/../tcpdump/missing/snprintf.c
	$(CC) $(CFLAGS) -o $@ -c $(srcdir)/../tcpdump/missing/snprintf.c

version.c: $(srcdir)/VERSION
	@rm -f $@
	sed -e 's/.*/char pcap_version[] = "&";/' $(srcdir)/VERSION > $@

bpf_filter.c: $(srcdir)/bpf/net/bpf_filter.c
	rm -f bpf_filter.c
	ln -s $(srcdir)/bpf/net/bpf_filter.c bpf_filter.c

bpf_filter.o: bpf_filter.c
	$(CC) $(CFLAGS) -c bpf_filter.c

install:
	[ -d $(DESTDIR)$(libdir) ] || \
	    (mkdir -p $(DESTDIR)$(libdir); chmod 755 $(DESTDIR)$(libdir))
	$(INSTALL_DATA) libpcap.a $(DESTDIR)$(libdir)/libpcap.a
	$(RANLIB) $(DESTDIR)$(libdir)/libpcap.a
	[ -d $(DESTDIR)$(includedir) ] || \
	    (mkdir -p $(DESTDIR)$(includedir); chmod 755 $(DESTDIR)$(includedir))
	$(INSTALL_DATA) $(srcdir)/pcap.h $(DESTDIR)$(includedir)/pcap.h
	$(INSTALL_DATA) $(srcdir)/pcap-namedb.h \
	    $(DESTDIR)$(includedir)/pcap-namedb.h
	[ -d $(DESTDIR)$(includedir)/net ] || \
	    (mkdir -p $(DESTDIR)$(includedir)/net; chmod 755 $(DESTDIR)$(includedir)/net)
	$(INSTALL_DATA) $(srcdir)/bpf/net/bpf.h \
	    $(DESTDIR)$(includedir)/net/bpf.h
	[ -d $(DESTDIR)$(mandir)/man3 ] || \
		(mkdir -p $(DESTDIR)$(mandir)/man3; chmod 755 $(DESTDIR)$(mandir)/man3)
	$(INSTALL_DATA) $(srcdir)/pcap.3 \
	    $(DESTDIR)$(mandir)/man3/pcap.3

uninstall:
	rm -f $(DESTDIR)$(libdir)/libpcap.a
	rm -f $(DESTDIR)$(includedir)/pcap.h
	rm -f $(DESTDIR)$(includedir)/pcap-namedb.h
	rm -f $(DESTDIR)$(includedir)/net/bpf.h
	rm -f $(DESTDIR)$(mandir)/man3/pcap.3

clean:
	rm -f $(CLEANFILES)

distclean:
	rm -rf $(CLEANFILES) Makefile config.cache config.log config.status \
	    config.h gnuc.h os-proto.h net bpf_filter.c stamp-h stamp-h.in

tags: $(TAGFILES)
	ctags -wtd $(TAGFILES)

tar:
	@cwd=`pwd` ; dir=`basename $$cwd` ; name=libpcap-`cat VERSION` ; \
	    list="" ; tar="tar chf" ; \
	    for i in `cat FILES` ; do list="$$list $$name/$$i" ; done; \
	    echo \
	    "rm -f ../$$name; ln -s $$dir ../$$name" ; \
	     rm -f ../$$name; ln -s $$dir ../$$name ; \
	    echo \
	    "(cd .. ; $$tar - [lots of files]) | compress > /tmp/$$name.tar.Z" ; \
	     (cd .. ; $$tar - $$list) | compress > /tmp/$$name.tar.Z ; \
	    echo \
	    "rm -f ../$$name" ; \
	     rm -f ../$$name

depend:	$(GENSRC) bpf_filter.c
	./mkdep -c $(CC) $(DEFS) $(INCLS) $(SRC)
