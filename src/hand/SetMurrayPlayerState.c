struct P { char pad[0x2C4]; int x2C4; char pad2[0x13F0 - 0x2C8]; void *x13F0; };
void SetMurrayState(struct P *p, int n);
void DropMurrayCarried(struct P *p);
void SetMurrayPlayerState(struct P *p, unsigned int k)
{
    switch (k) {
    case 0:
        if (p->x13F0) SetMurrayState(p, 5);
        else SetMurrayState(p, 0);
        break;
    case 1:
        switch (p->x2C4) { case 15: case 16: break; default: DropMurrayCarried(p); }
        SetMurrayState(p, 2);
        break;
    case 2: SetMurrayState(p, 0xD); break;
    case 3: DropMurrayCarried(p); SetMurrayState(p, 0xE); break;
    case 4: SetMurrayState(p, 0x10); break;
    case 5: DropMurrayCarried(p); SetMurrayState(p, 0x11); break;
    case 6: DropMurrayCarried(p); SetMurrayState(p, 0x12); break;
    case 7: DropMurrayCarried(p); SetMurrayState(p, 0x13); break;
    case 8: DropMurrayCarried(p); SetMurrayState(p, 0x14); break;
    }
}
