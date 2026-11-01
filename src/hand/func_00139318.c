struct G { char pad[0x4070]; int f4070; };
extern struct G D_003DD490;
void *PdlFromSwCid(struct G *, int);
void RemoveSlEntry(void *, void *);
void func_0011CD98(void *, void *);
void func_00139318(void *p) {
    RemoveSlEntry(PdlFromSwCid(&D_003DD490, 0x1c), p);
    func_0011CD98(&D_003DD490.f4070, p);
}
