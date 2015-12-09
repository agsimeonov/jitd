

TARGETS = btree
LIB_FILES = cog data util splay zipf policy
REWRITE_FILES = adaptive_merge cracker
FILES = \
  $(patsubst %, src/lib/%, $(LIB_FILES))\
  $(patsubst %, src/rewrites/%, $(REWRITE_FILES))


CC = gcc -std=c11 -Isrc/include -g
advanced: CC = gcc -D__ADVANCED -std=c11 -Isrc/include -g 
harvest: CC = gcc -D__HARVEST -std=c11 -Isrc/include -g 
build: CC = gcc -D__ADVANCED -std=c11 -Isrc/include -g 

build_test: $(TARGETS)
	@for i in $(TARGETS); do echo ====== Testing $$i ======;  done

test: $(TARGETS)
	@for i in $(TARGETS); do echo ====== Testing $$i ======; ./$$i; done

$(TARGETS) : % : $(patsubst %, %.o, $(FILES)) src/test/%_test.c
	$(CC) -o $@ $^ -lm

%.c : %.ds ../jitd 
	../jitd $< > $@

%.o : %.c
	$(CC) -c -o $@ $< -lm

build: build_test
	
advanced: test

harvest: test


clean:
	rm -f $(patsubst %, %.o, $(FILES))
	rm -rf *.{dSYM,o}
	rm btree

.PHONY: test advanced harvest clean
