CC       = g++ -Wall -Werror -Weffc++ -O3
CC_DEBUG = g++ -Wall -Werror -Weffc++ -D_GLIBCXX_DEBUG -g -fprofile-arcs -ftest-coverage -pg
C11      = -std=c++11
CPPCHECK = ../cppcheck-1.58/cppcheck

PROBLEMS = $(basename $(wildcard srm*.c++))

.PHONY: all
all: $(PROBLEMS)

.PHONY: clean
clean:
	rm -f perf.data perf.data.old
	rm -f *.gcov *.gcda *.gcno *.gprof \#*# gmon.out
	rm -f a.out $(PROBLEMS)

srm_144_div1_300: srm_144_div1_300.c++
	$(CPPCHECK) $@.c++
	$(CC_DEBUG) $^ -o $@

srm_144_div1_550: srm_144_div1_550.c++
	$(CPPCHECK) $@.c++
	$(CC_DEBUG) $(C11) $^ -o $@
