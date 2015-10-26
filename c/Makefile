

TARGETS = btree
LIB_FILES = cog data util splay zipfian_generator
REWRITE_FILES = adaptive_merge cracker
FILES = \
  $(patsubst %, src/lib/%, $(LIB_FILES))\
  $(patsubst %, src/rewrites/%, $(REWRITE_FILES))


CC = gcc -std=c11 -Isrc/include -g -lm

test: $(TARGETS)
	@for i in $(TARGETS); do echo ====== Testing $$i ======; ./$$i; done

$(TARGETS) : % : $(patsubst %, %.o, $(FILES)) src/test/%_test.c
	$(CC) -o $@ $^ -lm

%.c : %.ds ../jitd 
	../jitd $< > $@

%.o : %.c
	$(CC) -c -o $@ $<

clean:
	rm -f $(patsubst %, %.o, $(FILES))
	rm -rf *.{dSYM,o}

.PHONY: test clean
