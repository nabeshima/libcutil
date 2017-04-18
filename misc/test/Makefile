#
# $Id: Makefile 3 2013-05-20 13:07:23Z cota@upard.org $
# Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
# This file is subject to the MIT license available at,
# http://opensource.org/licenses/mit-license.php
#

include ../../Makefile.usrdef

TARGETS=\
	lexical_test \
	stl_test \
	ppm_test \
	config_test \
	token_test \
	crc_test \
	rsa_test \
	sha_test \
	cipher_test \
	intx_test \
	hash_test \
	octree_test \
	options_test \
	sharedptr_test \
	byte_test
	
all: $(TARGETS)

$(TARGETS): $(TARGETS:%=%.cpp)
	$(CXX) $@.cpp -c -I.. $(CFLAGS_TEST)
	$(CXX) $@.o -o $@ $(LDFLAGS_TEST)

test: all
	@for i in $(TARGETS); do \
		echo ""; \
		echo "####"; \
		echo "## Running " $$i " for testing..."; \
		echo "##"; \
		./$$i;\
	done
	./options_test --debug=true --port=1214444111111111 --help --log=fffggg112,222

clean distclean:
	rm -rf *~ .#* $(TARGETS) $(TARGETS:%=%.o)
