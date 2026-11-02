// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
typedef struct old_exception_table {
    void *start_region;
    void *end_region;
    void *exception_handler;
} old_exception_table;

void *old_find_exception_handler(void *pc, old_exception_table *table)
{
    if (table)
    {
        int pos;
        int best = -1;

        for (pos = 0; table[pos].start_region != (void *)-1; ++pos)
        {
            if (table[pos].start_region <= pc && table[pos].end_region > pc)
            {
                if (best == -1 || (table[pos].end_region <= table[best].end_region
                                   && table[pos].start_region >= table[best].start_region))
                    best = pos;
            }
            else if (best >= 0 && table[pos].start_region > pc)
                break;
        }
        if (best != -1)
            return table[best].exception_handler;
    }

    return (void *)0;
}
