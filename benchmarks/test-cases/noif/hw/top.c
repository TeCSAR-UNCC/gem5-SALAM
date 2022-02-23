#include "../noif_clstr_hw_defines.h"

typedef unsigned array_t[256];

void vadd(unsigned* arr, unsigned* arrle, unsigned* arrge,unsigned* vshn, unsigned* ky, unsigned* ghj) {
	// #pragma clang loop vectorize(enable)
	// #pragma clang loop vectorize_width(2)
int i=0;
int searchkey=36;
int summ=0;
int orred=1;
int temp = 0;
// #pragma clang loop vectorize_width(2)
for(i=0;i<6;i++)
{
    arrle[i]=searchkey<=arr[i];
    arrge[i]=searchkey>=arr[i];
    vshn[i]=arrle[i]&&arrge[i];
    summ+=vshn[i];
    temp|=(vshn[i]==1);
    // orredres = orredres||orred;
    
        // ctr1=i;
        // ky=1;
        
    
}
ky[0]=temp;

ghj=0;
i=0;
    for(i=0;i<6-1;i++)
    {
        //find the last le tag
        *ghj=i*(arrge[i]==1&&arrge[i+1]==0&&ky[0]==0&&1);
        if(*ghj>0&&1)
        {
            
        }
            // ctr=i;
            
        
    }
    

	
	return;
}

void top() {
	void * arr    = (void *)TESTA;
    void * arrle    = (void *)TESTB;
    void * arrge    = (void *)TESTC;
    void * vshn    = (void *)TESTD;
    void * ky    = (void *)TESTH;
     void * ghj    = (void *)TESTL;

	vadd(arr,arrle,arrge,vshn,ky,ghj);

	return;
}