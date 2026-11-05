/* include/sly2/stackscope.h -- STACKSCOPE, the stack-heap scope (platform/memory/memory.cpp defines both members).
 *
 * A local STACKSCOPE marks the stack heap on construction (0x1AE478, InitStackImpl under the heap lock) and frees
 * everything allocated from it since then on destruction (0x1AE4A0, FreeStackImpl; g++ 2.95's in-charge flag, bit 0
 * deletes the object). Callers construct one around loads and searches that allocate scratch memory (BRX loads, path
 * search, BSP clipping, the job/task loaders...).
 *
 * The class name is a placeholder (HYPOTHESIS, from the annotations: "RAII scope"). Neither member reads the object
 * and no caller's frame needs it larger than g++'s empty-class byte, so it has no members.
 *
 * Symbols: the destructor is the real out-of-line destructor (_$_10STACKSCOPE, config's name for 0x1AE4A0). The
 * constructor keeps config's C name STACKSCOPE_ctor through an asm label (its mangled name would be __10STACKSCOPE).
 * Code that calls the destructor explicitly, as a C function, declares it with `__asm__("_$_10STACKSCOPE")`.
 * See docs/guides/tu-merging.md, "Representation decisions" and "Views unified". */
#ifndef SLY2_STACKSCOPE_H
#define SLY2_STACKSCOPE_H

struct STACKSCOPE
{
    STACKSCOPE() __asm__("STACKSCOPE_ctor");
    ~STACKSCOPE();
};

#endif
