// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-114
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
// SLY2_SAVES: sd
typedef unsigned int size_t;
typedef unsigned int uword;
typedef int sword;
typedef unsigned char ubyte;
typedef struct dwarf_cie { uword length; sword CIE_id; ubyte version; char augmentation[0]; } __attribute__ ((packed, aligned (__alignof__ (void *)))) dwarf_cie;
typedef struct dwarf_fde { uword length; sword CIE_delta; void *pc_begin; uword pc_range; } __attribute__ ((packed, aligned (__alignof__ (void *)))) fde;
struct cie_info { char *augmentation; void *eh_ptr; int code_align; int data_align; unsigned ra_regno; };
union unaligned { void *p; unsigned b2 __attribute__ ((mode (HI))); unsigned b4 __attribute__ ((mode (SI))); unsigned b8 __attribute__ ((mode (DI))); } __attribute__ ((packed));
int func_002401D8(const char *, const char *);
#define strcmp func_002401D8
size_t func_0021ED28(const char *);
#define strlen func_0021ED28
void *func_0023F240(void *, unsigned *);
#define decode_uleb128 func_0023F240
void *func_0023F288(void *, int *);
#define decode_sleb128 func_0023F288

static inline dwarf_cie *
get_cie (fde *f)
{
  return ((void *)&f->CIE_delta) - f->CIE_delta;
}

static inline void *
read_pointer (void *p)
{ union unaligned *up = p; return up->p; }

void *
extract_cie_info (fde *f, struct cie_info *c)
{
  void *p;
  int i;
  int r;

  c->augmentation = get_cie (f)->augmentation;

  r = strcmp (c->augmentation, "");
  __asm__ __volatile__ (""); /* emulates Sony scheduler: nothing hoisted above the strcmp call (PRE-inserted lui stays after it) */
  if (r != 0
      && strcmp (c->augmentation, "eh") != 0
      && c->augmentation[0] != 'z')
    return 0;

  p = c->augmentation + strlen (c->augmentation) + 1;

  if (strcmp (c->augmentation, "eh") == 0)
    {
      c->eh_ptr = read_pointer (p);
      p += sizeof (void *);
    }
  else
    c->eh_ptr = 0;

  p = decode_uleb128 (p, &c->code_align);
  p = decode_sleb128 (p, &c->data_align);
  c->ra_regno = *(unsigned char *)p++;

  if (c->augmentation[0] == 'z')
    {
      p = decode_uleb128 (p, &i);
      p += i;
    }

  return p;
}
