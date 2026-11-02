// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
#define NULL ((void *)0)
typedef void * (*__eh_matcher) (void *, void *, void *);
typedef struct __eh_info { __eh_matcher match_function; short language; short version; } __eh_info;
typedef struct exception_table {
  void *start_region;
  void *end_region;
  void *exception_handler;
  void *match_info;
} exception_table;
typedef struct exception_descriptor {
  short lang;
  short version;
  void *runtime_id_field;
  exception_table table[1];
} exception_descriptor;

void *
find_exception_handler (void *pc, exception_descriptor *table,
                        __eh_info *eh_info, int rethrow, int *cleanup)
{
  void *retval = NULL;
  *cleanup = 1;
  if (table)
    {
      int pos = 0;
      exception_table *tab = &(table->table[0]);

      if (rethrow)
        {
          pos = (exception_table *) pc - tab;
          pc = ((exception_table *) pc)->end_region - 1;
          if (tab[pos].start_region != (void *) -1)
            pos++;
        }
      else
        pc--;

      for ( ; tab[pos].start_region != (void *) -1; pos++)
        {
          if (tab[pos].start_region <= pc && tab[pos].end_region > pc)
            {
              if (tab[pos].match_info)
                {
                  __eh_matcher matcher = eh_info->match_function;
                  if (matcher)
                    {
                      void *ret = (*matcher)((void *) eh_info,
                                             tab[pos].match_info, table);
                      if (ret)
                        {
                          if (retval == NULL)
                            retval = tab[pos].exception_handler;
                          *cleanup = 0;
                          break;
                        }
                    }
                }
              else
                {
                  if (retval == NULL)
                    retval = tab[pos].exception_handler;
                }
            }
        }
    }
  return retval;
}
