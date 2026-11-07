#include "m2c_prelude.h"
extern M2C_UNK (*D_002C60BC)() __asm__("get_eh_context");

// Returns (*get_eh_context)(). (libgcc exception) SDK name __get_eh_context.
void func_0023E398(void) {
    D_002C60BC();
}
