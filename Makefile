CFLAGS = -Wall -Wextra -Wconversion -std=c11 -Isrc

run_tests: src/ccsds.c tests/test_ccsds.c
	gcc $(CFLAGS) $^ -o $@

test: run_tests
	./run_tests

clean:
	rm -f run_tests