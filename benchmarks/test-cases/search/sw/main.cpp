#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "bench.h"
#include "../../../common/m5ops.h"

// volatile uint32_t *arg4 = (uint32_t *)0x2f000019;


static unsigned char base[1024];
static uint offset = 0;

void * my_malloc(int size){
  void * mem;
  mem = (void*)(base + offset);
  offset += size;

  return mem;
}

// using namespace std;
struct BTree{
   int d[7];
   struct BTree * child_ptr[8];
   int l;
   int n;
}*r = NULL, *np = NULL, *x = NULL;

volatile uint8_t * top  = (uint8_t *)TOP;
volatile uint32_t *arg1 = (uint32_t *)(TOP + 1);
volatile uint32_t *arg2 = (uint32_t *)(TOP + 9);
volatile uint32_t *arg3 = (uint32_t *)(TOP + 17);

struct BTree* init(){
   int i;
   np = (struct BTree*)my_malloc(sizeof(struct BTree));
   np->l = true;
   np->n = 0;
   for (i = 0; i < 7; i++) {
      np->child_ptr[i] = NULL;
   }
   return np;
}

void traverse(struct BTree *p){
   // cout<<endl;
   printf("\n");
   int i;
   for (i = 0; i < p->n; i++) {
      if (p->l == false) {
         traverse(p->child_ptr[i]);
      }
      printf(" %d", p->d[i]);
   }
   if (p->l == false) {
      traverse(p->child_ptr[i]);
   }
   // cout<<endl;
   printf("\n");
}

void splitChild(int i, struct BTree *y, struct BTree* s) {
//    printf("CALLLLLL SC!!!\n");
   struct BTree* z = (struct BTree*)my_malloc(sizeof(struct BTree));
   z->l=y->l;
   z->n=3;
    // printf("HERERREERERRER!!!\n");
    for(int j=0;j<3;j++)
    {
        z->d[j]=y->d[j+4];//CHECK
    }
    // printf("First For LOOP\n");
    if(y->l==false)
    {
        for(int j=0;j<4;j++)
        {
            z->child_ptr[j]=y->child_ptr[j+4];
        }
    }
    // printf("After IF\n");
        y->n=3;
        for(int j=s->n;j>=i+1;j--)
        {
            s->child_ptr[j+1]=s->child_ptr[j];
        }
    // printf("After second FOR LOOP\n");
        s->child_ptr[i+1]= z;
        for(int j=((s->n)-1);j>=i;j--)
        {
            s->d[j+1]=s->d[j];
        }
        // printf("After third FOR LOOP\n");
        s->d[i]=y->d[3];
        s->n=s->n+1;
        // printf("END of SplitChild\n");
    
}

void insertNonFull(int k, struct BTree* s)
{
    int i=s->n-1;
   //  printf("HERERREERERRER INF!!!\n");
    if(s->l==true)
    {
        while(i>=0 && s->d[i]>k)
        {
            s->d[i+1]=s->d[i];
            i--;
        }

        s->d[i+1]=k;
        s->n=s->n+1;
      //   printf("HERERREERERRER INF 1!!!\n");
    }
    else
    {
        while(i>=0 && s->d[i]>k)
        {
            i--;
        }
        struct BTree* temp = s->child_ptr[i+1];
        struct BTree* temp1 = temp;

        if(temp1->n == 7)
        {
           
            splitChild(i+1, s->child_ptr[i+1], s);
        
        if(s->d[i+1]<k)
        {
            i++;
        }
        }
      //   printf("HERERREERERRER INF 2!!!\n");
        insertNonFull(k, s->child_ptr[i + 1]);
      //   printf("HERERREERERRER INF 3!!!\n");
    }
   //  printf("DONE INF\n");
}

void insert(int a) {
    // printf("CALLLLLL!!!\n");
   if(r==NULL)
   {
       r = (struct BTree*)my_malloc(sizeof(struct BTree));
       r->d[0]=a;
       r->n=1;
       r->l=true;

   }
   else
   {
    //    printf("Entered ELSE!!!\n");
       if(r->n==7)
       {
        //    printf("HERERERERE INSERT1!!!\n");
           struct BTree* s=(struct BTree*)my_malloc(sizeof(struct BTree));
           s->l=false;
           s->child_ptr[0]=r;

           splitChild(0,r,s);
// printf("After SC\n");
           int i=0;
           if(s->d[0]<a)
           {
               i++;
           }
// printf("Insert FUll up next\n");
           insertNonFull(a, s->child_ptr[i]);
// printf("Insert FUll complete\n");
           r=s;
       }
       else
       {
         //   printf("HERERREERERRER!!!\n");
           insertNonFull(a, r);
        //    printf("After second NOT FULL\n");
       }
   }
//    printf("Complete Insert\n");
}


  

int main(void) {

   int val, ch;
   int i, n, t;
   for(i = 1; i <= 10; i++)
      insert(i);

   printf("traversal of constructed B tree %d, %p\n", r->n, ((char*)r + 56));
   printf("traversal of constructed B tree %p, %d\n", r, offset);
   printf("offset of d: %ld\n", offsetof(struct BTree, d));
   printf("offset of child_ptr: %ld\n", offsetof(struct BTree, child_ptr));
   printf("offset of l: %ld\n", offsetof(struct BTree, l));
   printf("offset of n: %ld\n", offsetof(struct BTree, n));

    m5_reset_stats();

    int *flag;
    *flag = 0;



    uint32_t base = 0x80c00000;
    TYPE *m1 = (TYPE *)base;
    TYPE *m2 = (TYPE *)(base + sizeof(TYPE) * N);
    TYPE *m3 = (TYPE *)(base + 2 * sizeof(TYPE) * N);


    *m1 = 1;
    *m2 = 0;
    *m3 = (uint64_t)r;


    *arg1 = (uint64_t)(void *)m1;
    *arg2 = (uint64_t)(void *)m2;
    *arg3 = (uint64_t)(void *)m3;

    // printf("Job complete : %p %p\n", m1, (m1 + 1));
    *top = 0x01;
    int count;
    while (*top != 0)
    count++;

    printf("Done\n");

    printf("Job complete : %d %p %p\n",m1[0], m2[0], r->child_ptr[0]);

    m5_dump_stats();
    m5_exit();

}