#include "../search_clstr_hw_defines.h"

typedef struct BTree{
   int d[7];
   struct BTree * child_ptr[8];
   int l;
   int n;
} bTree;


//SEQUENTIAL

void top(unsigned * a, unsigned* b, unsigned *c) {

	bTree * cursor = (bTree* )*c;
	unsigned key = *a;
//    *b = cursor->child_ptr[1];
	for(int i = 0; i < cursor->n; i++){
		if(key<cursor->d[i])
		{
			cursor = (bTree* )cursor->child_ptr[0];
			
			break;
		}
		if(key == cursor->d[i])
		{
			*b=1;
			break;
		}
	}
    //   cursor = (bTree* )*c;
	  *b = cursor;
   // c = (bTree*)cursor;
   // cursor = (bTree *)c;
	// for(int i = 0; i < cursor->n; i++){
	// 	if(key<cursor->d[i])
	// 	{
	// 		cursor = cursor->child_ptr[i];
	// 		break;
	// 	}
	// 	if(key == cursor->d[i])
	// 	{
	// 		*b=1;
	// 		break;
	// 	}
	// }
   
   // *b = 1;
	// for(int i = 0; i < cursor->n; i++){
	// 	if(key<cursor->d[i])
	// 	{
	// 		temp = cursor->child_ptr[i];
	// 		cursor = temp;
	// 		break;
	// 	}
	// 	if(key == cursor->d[i])
	// 	{
	// 		*b=1;
	// 		break;
	// 	}
	// }

	return;

  
}



