#
# $Id$
# Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
# This file is subject to the MIT license available at,
# http://opensource.org/licenses/mit-license.php
#

include Makefile.usrdef

LIBNAME=cutil

SUBDIRS=`ls -F | grep /`

HEADERS=\
	`find . -name *.h` \
	`find . -name *.ipp`

EVERYTHING=all doc test
.PHONY: test doc

$(EVERYTHING):
	@for subdir in $(SUBDIRS); do ( cd $$subdir && $(MAKE) $@ ); done

install:
	mkdir -p $(INSTALLPATH)/lib $(INSTALLPATH)/include/$(LIBNAME)
	cp $(HEADERS) $(INSTALLPATH)/include/$(LIBNAME)

uninstall:
	rm -rf $(INSTALLPATH)/include/$(LIBNAME)

clean distclean:
	find . -name "*~" -exec rm {} \;
	find . -name ".#*" -exec rm {} \;
	@for subdir in $(SUBDIRS); do ( cd $$subdir && $(MAKE) $@ ); done
