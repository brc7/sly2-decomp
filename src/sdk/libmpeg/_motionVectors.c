// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
int _getBits(void *ctx, int n);
void _motionVector(void *ctx, int *PMV, int *dmvector, int h_r_size, int v_r_size, int dmv, int mvscale, int full_pel_vector);
void _motionVectors(void *ctx, int PMV[2][2][2], int *dmvector, int motion_vertical_field_select[2][2], int s,
                    int motion_vector_count, int mv_format, int h_r_size, int v_r_size, int dmv, int mvscale)
{
    if (motion_vector_count == 1) {
        if (mv_format == 0 && !dmv) {
            motion_vertical_field_select[1][s] = motion_vertical_field_select[0][s] = _getBits(ctx, 1);
        }
        _motionVector(ctx, PMV[0][s], dmvector, h_r_size, v_r_size, dmv, mvscale, 0);
        PMV[1][s][0] = PMV[0][s][0];
        PMV[1][s][1] = PMV[0][s][1];
    } else {
        motion_vertical_field_select[0][s] = _getBits(ctx, 1);
        _motionVector(ctx, PMV[0][s], dmvector, h_r_size, v_r_size, dmv, mvscale, 0);
        motion_vertical_field_select[1][s] = _getBits(ctx, 1);
        _motionVector(ctx, PMV[1][s], dmvector, h_r_size, v_r_size, dmv, mvscale, 0);
    }
}
