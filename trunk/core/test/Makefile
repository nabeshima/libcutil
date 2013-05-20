#
# $Id$
# Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
# This file is subject to the MIT license available at,
# http://opensource.org/licenses/mit-license.php
#

include ../../Makefile.usrdef

# switch by OS
ifeq (${shell uname},Linux)
PTHREAD_LDFLAGS=-pthread
endif

TARGETS=\
	period_test \
	proc_test \
	thread_test \
	stream_test


all: $(TARGETS)

$(TARGETS): $(TARGETS:%=%.cpp)
	$(CXX) $@.cpp -c -I.. $(CFLAGS_TEST)
	$(CXX) $@.o -o $@ $(LDFLAGS_TEST) $(PTHREAD_LDFLAGS)

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
