// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-114
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
// SLY2_SAVES: sd
// MATCH (Claude agent): thread-context reset is one noreorder asm block (keeps GNU as from filling the EI jal slot).
typedef struct {
    int status; void *entry; char *stack; int stackSize;
    void *gpReg; int initPriority; int currentPriority; unsigned attr;
    unsigned option; int waitType; int waitId; int wakeupCount;
} ThreadParam;
int func_00235100(void);
int func_00239450(void);
int func_002394A0(void);
int func_00234920(int tid, ThreadParam *info);
int func_00234840(int tid, void *arg);
int StartThread_2355E0(int tid, void *arg)
{
    ThreadParam info;
    int r;
    char *p;

    if (func_00235100() != 0)
        return -1;
    if (func_00239450() == 0)
        return -1;
    r = func_00234920(tid, &info);
    if (r < 0) {
        func_002394A0();
        return r;
    }
    if (info.status == 0x10) {
    p = info.stack + info.stackSize - 0x2A0;
    __asm__ __volatile__(
        ".set noreorder\n\tsq $0, 0x0(%0)\n\tsq $0, 0x10(%0)\n\tsq $0, 0x20(%0)\n\tsq $0, 0x30(%0)\n\t"
        "sq $0, 0x40(%0)\n\tsq $0, 0x50(%0)\n\tsq $0, 0x60(%0)\n\tsq $0, 0x70(%0)\n\t"
        "sq $0, 0x80(%0)\n\tsq $0, 0x90(%0)\n\tsq $0, 0xA0(%0)\n\tsq $0, 0xB0(%0)\n\t"
        "sq $0, 0xC0(%0)\n\tsq $0, 0xD0(%0)\n\tsq $0, 0xE0(%0)\n\tsq $0, 0xF0(%0)\n\t"
        "sq $0, 0x100(%0)\n\tsq $0, 0x110(%0)\n\tsq $0, 0x120(%0)\n\tsq $0, 0x130(%0)\n\t"
        "sq $0, 0x140(%0)\n\tsq $0, 0x150(%0)\n\tsq $0, 0x160(%0)\n\tsq $0, 0x170(%0)\n\t"
        "sq $0, 0x180(%0)\n\tsq $0, 0x190(%0)\n\tsq $0, 0x1A0(%0)\n\tsq $0, 0x1B0(%0)\n\t"
        "sq $0, 0x200(%0)\n\tsq $0, 0x210(%0)\n\tsq $0, 0x220(%0)\n\tsq $0, 0x230(%0)\n\t"
        "sq $0, 0x240(%0)\n\tsq $0, 0x250(%0)\n\tsq $0, 0x260(%0)\n\tsq $0, 0x270(%0)\n\t"
        "sll $8, %1, 0\n\tsd $8, 0x40(%0)\n\t"
        "lw $8, 0x1C0(%0)\n\tsd $8, 0x1C0(%0)\n\tsd $0, 0x1C8(%0)\n\t"
        "lw $8, 0x1D0(%0)\n\tsd $8, 0x1D0(%0)\n\tsd $0, 0x1D8(%0)\n\t"
        "lw $8, 0x1E0(%0)\n\tsd $8, 0x1E0(%0)\n\tsd $0, 0x1E8(%0)\n\t"
        "lw $8, 0x1F0(%0)\n\tsd $8, 0x1F0(%0)\n\tsd $0, 0x1F8(%0)\n\t.set reorder"
        : : "r"(p), "r"(arg) : "$8", "memory");
    func_002394A0();
    return func_00234840(tid, arg);
    }
    func_002394A0();
    return -1;
}
