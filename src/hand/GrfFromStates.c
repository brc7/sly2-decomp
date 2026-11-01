int GrfFromState(int i);
int GrfFromStates(int c, int *ai)
{
    int grf = 0;
    int i;
    for (i = 0; i < c; i++)
        grf |= GrfFromState(ai[i]);
    return grf;
}
