#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

// This program intentionally leaks memory.  Its intended use
// case -- testing/tuning of monitoring/alerting systems.

static size_t min_chunk_size = 1024 * 1024;

static size_t round_chunk_size(size_t mem_chunk);
static size_t get_chunk_size(char * arg1);
static void * allocate_memory(size_t mem_chunk);

int main(int argc, char * argv[]) {
    size_t mem_chunk = min_chunk_size;
    size_t mem_total = 0;

    printf("argc: %d\n", argc);

    if (argc > 1) {
        mem_chunk = get_chunk_size(argv[1]);
        mem_chunk = round_chunk_size(mem_chunk);
    }

    printf("mem_chunk(%ld): %ld\n", mem_chunk, round_chunk_size(mem_chunk));

    for (;;) {
        double mem_total_g = (double)mem_total / (1024.0 * 1024.0 * 1024.0);
        fprintf(stderr, "Allocating %ld bytes of memory (%f GB in total)... ",
            mem_chunk, mem_total_g);

        void * memptr = allocate_memory(mem_chunk);
        if (memptr == NULL) {
            fprintf(stderr, "\n*** malloc() failed. ***\n");
            return 1;
        }

        mem_total += mem_chunk;

        fprintf(stderr, "Done.\n");

        sleep(1);
    }
    return 0;
}

static size_t
round_chunk_size(size_t mem_chunk) {
    return (mem_chunk / 4096U) * 4096U;
}

static size_t
get_chunk_size(char * arg1) {
    long int val;

    errno = 0;
    val = strtol(arg1, NULL, 10);

    if (errno == ERANGE) {
        fprintf(stderr, "*** Cannot convert \"%s\" to integer ***\n", arg1);
    }
    if (val < min_chunk_size) {
        val = min_chunk_size;
    }

    return val;
}

static void *
allocate_memory(size_t mem_chunk) {
    void * memptr = malloc(mem_chunk);
    int8_t * ptr = (int8_t *)memptr;
    for (size_t i = 0; i < mem_chunk; i += 4096U) {
        ptr[i] = 1;
    }
    return memptr;
}
