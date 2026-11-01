#include "common.h"
struct S { char pad[0x148]; ulong fl; };
int GetSoIceable(struct S *p) { return (int)(p->fl >> 33) & 1; }
