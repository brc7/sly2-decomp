/* include/sly2/stackscope.h -- STACKSCOPE, the stack-heap scope (platform/memory/memory.cpp).
 *
 * A local STACKSCOPE marks the stack heap on construction (0x1AE478, InitStackImpl under the heap lock) and frees
 * everything allocated from it since then on destruction (0x1AE4A0, FreeStackImpl; the destructor takes g++ 2.95's
 * in-charge flag and deletes the object when bit 0 is set). Callers construct one around loads and searches that
 * allocate scratch memory (BRX loads, path search, BSP clipping, the job/task loaders...).
 *
 * The class name and the member layout are placeholders (HYPOTHESIS, from the annotations: "RAII scope"; the
 * mangled names would be __10STACKSCOPE / _$_10STACKSCOPE). Neither function reads the object, and no caller's
 * frame needs it larger than g++'s empty-class byte, so it has no members.
 *
 * Representation (docs/guides/tu-merging.md, "Representation decisions"): config names the two functions with C
 * names, so the constructor is bound to STACKSCOPE_ctor with an asm label, and the destructor is an inline wrapper
 * that calls the out-of-line destructor 0x1AE4A0 with in-charge 2, which is the call g++ emits for a non-inline
 * destructor (an asm label on a destructor makes g++ drop the call). No caller references a destructor symbol, so
 * no link alias is needed. When config takes the mangled names, the wrapper becomes a plain `~STACKSCOPE();` and
 * memory.cpp defines the two members. */
#ifndef SLY2_STACKSCOPE_H
#define SLY2_STACKSCOPE_H

struct STACKSCOPE
{
    STACKSCOPE() __asm__("STACKSCOPE_ctor");
    ~STACKSCOPE();
    /* the out-of-line destructor body (0x1AE4A0), grf = g++'s in-charge flag */
    void Destruct(int grf) __asm__("STACKSCOPE_ctor_1AE4A0");
};

#endif
