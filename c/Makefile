

TARGETS = btree
LIB_FILES = cog data util splay zipf policy
REWRITE_FILES = adaptive_merge cracker
FILES = \
  $(patsubst %, src/lib/%, $(LIB_FILES))\
  $(patsubst %, src/rewrites/%, $(REWRITE_FILES))


CC = gcc -std=c11 -Isrc/include -g -O0
advanced: CC = gcc -D__ADVANCED -std=c11 -Isrc/include -g -O0
harvest: CC = gcc -D__HARVEST -std=c11 -Isrc/include -g -O0
build: CC = gcc -D__ADVANCED -std=c11 -Isrc/include -g -O0

build_test: $(TARGETS)
	@for i in $(TARGETS); do echo ====== Testing $$i ======;  done

test: $(TARGETS)
	@for i in $(TARGETS); do echo ====== Testing $$i ======; ./$$i; done

$(TARGETS) : % : $(patsubst %, %.o, $(FILES)) src/test/%_test.c
	$(CC) -o $@ $^ -lm -l json -O0

%.c : %.ds ../jitd 
	../jitd $< > $@

%.o : %.c
	$(CC) -c -o $@ $< -lm -l json -O0

build: build_test
	
advanced: test

harvest: test


clean:
	rm -f $(patsubst %, %.o, $(FILES))
	rm -rf *.{dSYM,o}
	rm btree

.PHONY: test advanced harvest clean
