#include <Arduino.h>
#include <malloc.h>

extern "C" char *sbrk(int i);
extern char _estack; // NOLINT(*-reserved-identifier)

static char *ramend = &_estack;

#define NUM_BLOCKS 100
#define BLOCK_SIZE 4

uint16_t get_memory_usage() {
    const auto stack_ptr = reinterpret_cast<char *>(__get_MSP());
    const struct mallinfo mi = mallinfo();
    const auto heap = mi.uordblks;
    const auto stack = ramend - stack_ptr;

    return heap + stack;
}
