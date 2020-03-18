#include "../defines.h"

void vadd() {
	TYPE *a = (TYPE *)(0x2f100001);
	TYPE *b = (TYPE *)(0x2f100041);
	TYPE *c = (TYPE *)(0x2f100081);

	int i;
	for (i=0; i<16; i++) {
		*b = *b + (*a * c[i]);
	}
}
