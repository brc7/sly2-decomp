#include "common.h"
struct S { char pad[0xD0]; ulong fl; };
int GetSoIgnoreLocked(struct S *p) { return (int)(p->fl >> 34) & 1; }
