#
# $Id: Makefile 3 2013-05-20 13:07:23Z cota@upard.org $
# Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
# This file is subject to the MIT license available at,
# http://opensource.org/licenses/mit-license.php
#

include Makefile.usrdef

LIBNAME=cutil

SUBDIRS=`ls -F | grep /`

HEADERS=\
	`find . -name *.h` \
	`find . -name *.ipp`

EVERYTHING=all test
.PHONY: test doc

$(EVERYTHING):
	@for subdir in $(SUBDIRS); do ( cd $$subdir && $(MAKE) $@ ); done

doc:
	( cd doc && $(MAKE) $@ )

install:
	mkdir -p $(INSTALLPATH)/lib $(INSTALLPATH)/include/$(LIBNAME)
	cp $(HEADERS) $(INSTALLPATH)/include/$(LIBNAME)

uninstall:
	rm -rf $(INSTALLPATH)/include/$(LIBNAME)

clean distclean:
	find . -name "*~" -exec rm {} \;
	find . -name ".#*" -exec rm {} \;
	@for subdir in $(SUBDIRS); do ( cd $$subdir && $(MAKE) $@ ); done
	( cd doc && $(MAKE) $@ )
