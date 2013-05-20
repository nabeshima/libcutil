#
# $Id$
# Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
# This file is subject to the MIT license available at,
# http://opensource.org/licenses/mit-license.php
#

include ../../../Makefile.usrdef

ifneq (${shell if pkg-config --exists directfb; then echo DUMMY; fi},)
TARGETS=\
	dfbwidget_test
DFB_CFLAGS=`pkg-config --cflags directfb`
DFB_LDFLAGS=`pkg-config --libs directfb`
endif

all: $(TARGETS)

$(TARGETS): $(TARGETS:%=%.cpp)
	$(CXX) $@.cpp -c -I../../../core -I.. $(CFLAGS_TEST) $(DFB_CFLAGS)
	$(CXX) $@.o -o $@ $(LDFLAGS_TEST) $(DFB_LDFLAGS)

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
