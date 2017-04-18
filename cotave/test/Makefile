#
# $Id: Makefile 3 2013-05-20 13:07:23Z cota@upard.org $
# Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
# This file is subject to the MIT license available at,
# http://opensource.org/licenses/mit-license.php
#

include ../../Makefile.usrdef

# switch by OS
ifeq (${shell uname},Linux)
LAPACK_LDFLAGS=-llapack
endif

ifeq (${shell uname},Darwin)
LAPACK_LDFLAGS=-llapack
endif


TARGETS=\
	cotave_test \
	polynomial_test \
	butterworth_test
	

all: $(TARGETS)

$(TARGETS): $(TARGETS:%=%.cpp)
	$(CXX) $@.cpp -c -I.. $(CFLAGS_TEST)
	$(CXX) $@.o -o $@ $(LDFLAGS_TEST) $(LAPACK_LDFLAGS)

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
