#include "sys/exit.h"
__attribute__((noreturn,used)) void __stack_chk_fail(void) {
    exit(1);
}
