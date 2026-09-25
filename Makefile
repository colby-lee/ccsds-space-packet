CFLAGS = -Wall -Wextra -Wconversion -std=c11 -Isrc

run_tests: src/ccsds_spp.c src/ccsds_tm.c tests/test_ccsds.c
	gcc $(CFLAGS) $^ -o $@

test: run_tests
	./run_tests

clean:
	rm -f run_tests
