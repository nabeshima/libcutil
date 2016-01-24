#
# $Id: Makefile 3 2013-05-20 13:07:23Z cota@upard.org $
# Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
# This file is subject to the MIT license available at,
# http://opensource.org/licenses/mit-license.php
#

SUBDIRS=`ls -F | grep /`

EVERYTHING=all doc test clean distclean
.PHONY: test doc

$(EVERYTHING):
	@for subdir in $(SUBDIRS); do ( cd $$subdir && $(MAKE) $@ ); done
