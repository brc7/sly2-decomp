#include "m2c_prelude.h"
extern M2C_UNK (*D_002C60BC)() __asm__("get_eh_context");

// Returns (*get_eh_context)(). (gcc 2.95 libgcc2.c exception support, same body.)
void __get_eh_context(void) {
    D_002C60BC();
}
