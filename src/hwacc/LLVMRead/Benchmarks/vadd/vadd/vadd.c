void vadd(int* a, int* b, int* c) {
    int i;
    #pragma clang loop unroll(disable)
    for (i = 0; i < 256; i++) {
        c[i] = a[i] + b[i];
    }
}
