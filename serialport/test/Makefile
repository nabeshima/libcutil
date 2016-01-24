#
# $Id: Makefile 4 2013-09-11 05:53:28Z cota@upard.org $
# Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
# This file is subject to the MIT license available at,
# http://opensource.org/licenses/mit-license.php
#

include ../../Makefile.usrdef

TARGETS=\
	serialport_test


all: $(TARGETS)

$(TARGETS): $(TARGETS:%=%.cpp)
	$(CXX) $@.cpp -c -I../../core -I.. $(CFLAGS_TEST)
	$(CXX) $@.o -o $@ $(LDFLAGS_TEST)

test: all
	@for i in $(TARGETS); do \
		echo ""; \
		echo "####"; \
		echo "## Running " $$i " for testing..."; \
		echo "##"; \
		./$$i;\
	done

clean distclean:
	rm -rf *~ .#* $(TARGETS) $(TARGETS:%=%.o)
