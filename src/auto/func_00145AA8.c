#include "m2c_prelude.h"
s32 FIsBasicDerivedFrom__FP5BASIC3CID(void *, M2C_UNK);                 /* extern */
s128 func_001A62E8(void *);                      /* extern */
s32 PpathzoneFromPos(s128, M2C_UNK);                /* extern */

s32 func_00145AA8(void *arg0) {
    s32 var_v0;
    void *temp_a0;
    void *var_v1;

    if (FIsBasicDerivedFrom__FP5BASIC3CID(M2C_FIELD(arg0, void **, 0x30), 0x30) != 0) {
        temp_a0 = M2C_FIELD(M2C_FIELD(arg0, void **, 0x30), void **, 0xE8);
        var_v1 = NULL;
        if (temp_a0 != NULL) {
            var_v1 = M2C_FIELD(temp_a0, void **, 0xDC);
        }
        var_v0 = 0;
        if (var_v1 != NULL) {
            return M2C_FIELD(var_v1, s32 *, 0x10);
        }
        /* Duplicate return node #6. Try simplifying control flow for better match */
        return var_v0;
    }
    var_v0 = PpathzoneFromPos(func_001A62E8(M2C_FIELD(arg0, void **, 0x30)), 1);
    return var_v0;
}
