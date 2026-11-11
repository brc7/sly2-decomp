#include "m2c_prelude.h"
// Returns table->lang.version (+6). (gcc 2.95 libgcc2.c exception support, a static with this body.)
s16 __get_eh_table_version(void *arg0) {
    return M2C_FIELD(arg0, s16 *, 6);
}
