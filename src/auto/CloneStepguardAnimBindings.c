#include "m2c_prelude.h"
M2C_UNK func_00208208(void *, void *);              /* extern */
M2C_UNK func_00209590(void *);                      /* extern */

void CloneStepguardAnimBindings(void *arg0) {
    s32 var_s2;
    void *var_s0;

    var_s2 = 0;
    if (M2C_FIELD(arg0, s32 *, 0x4C0) > 0) {
        var_s0 = arg0 + 0x4D0;
        do {
            var_s2 += 1;
            func_00209590(var_s0);
            var_s0 += 0x20;
        } while (var_s2 < M2C_FIELD(arg0, s32 *, 0x4C0));
    }
    func_00208208(arg0, arg0 + 0xC10);
    func_00208208(arg0, arg0 + 0x8D0);
    func_00208208(arg0, arg0 + 0xA70);
}
