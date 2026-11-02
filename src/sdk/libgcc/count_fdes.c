// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0 -fexceptions
// SLY2_ASM: gnu-reorder-db
typedef unsigned int uword;
typedef int sword;
typedef unsigned int size_t;
typedef struct dwarf_fde {
    uword length;
    sword CIE_delta;
    void *pc_begin;
    uword pc_range;
} fde;

static inline fde *next_fde(fde *p)
{
    return (fde *)((char *)p + p->length + sizeof(p->length));
}

size_t count_fdes(fde *this_fde)
{
    size_t count;
    for (count = 0; this_fde->length != 0; this_fde = next_fde(this_fde))
    {
        if (this_fde->CIE_delta == 0 || this_fde->pc_begin == 0)
            continue;
        ++count;
    }
    return count;
}
