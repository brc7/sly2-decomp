struct X { char pad[0x7C]; void *f7c; char pad2[0x88 - 0x80]; struct X *pNext; };
struct G { char pad[0x3B0C]; void *f3b0c; };
extern struct G D_003DD490;
extern struct X *D_003E0F90;
extern char D_002DE578[];
void HandleGameEvent(void *pv, int n, void *p);
void func_00154A90(void *pv, int n, void *p);
void SendTaskGameEvent(struct X *px, int n, void *p);
void HandleJobGameEvent(void *pv, int n, void *p);
void BroadcastGameEvent(void *pv, int n, void *p)
{
    struct X *px;
    HandleGameEvent(pv, n, p);
    func_00154A90(D_002DE578, n, p);
    for (px = D_003E0F90; px; px = px->pNext) {
        if (px->f7c == D_003DD490.f3b0c)
            SendTaskGameEvent(px, n, p);
    }
    if (D_003DD490.f3b0c)
        HandleJobGameEvent(D_003DD490.f3b0c, n, p);
}
