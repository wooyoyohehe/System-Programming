#include<string.h>
#include <assert.h>
#include <stdio.h>
#include "my_malloc.h"
#include <unistd.h>
#include <stdlib.h>

int main(){

    printf("Test 1 in here: just allocate the 12 byte which should get 24 byte chunk because header\n");
    int* p = (int*)my_malloc(sizeof(int)*3);
    int i ;
    printf("-----------------\n");
    memcpy(&i,(char*)p -8,4);
    assert(i == 24);
    p[0] = 1;
    p[1] = 2;
    p[2] = 3;
    assert(p[0] == 1);
    assert(p[1] == 2);
    assert(p[2] == 3);
    my_free(p);
    printf("After free1\n");
    printf("You need to check every freelistnode's address is chunked in right size\n");
    printf("In here you have have 2 nodes and the size is 24 and 8168\n");
    FreeListNode n = free_list_begin();
    while(n!=NULL){
        printf("free1 %p\n",n);
        void * ppt = n;
        printf("free1 %zu\n",n->size);
        n = n -> flink;
    }


    printf("-----------test1 end---------------\n");

    // error case
//    int* u = (int*)sbrk(103);
//    my_free(u);
//    assert(my_errno == MYBADFREEPTR);

    printf("Test 2 in here: just allocate the 16 byte which should get 24 byte chunk because header\n");
    int* q = (int*)my_malloc(sizeof(int)*4);
    printf("42 line in rest %d\n", free_list_begin()->size);
    printf("address in 41th %p", q);
    memcpy(&i,(char*)p -8,4);
    assert(i==24);
    q[0] = 1;
    q[1] = 2;
    q[2] = 3;
    q[3] = 4;
    assert(q[0] == 1);
    assert(q[1] == 2);
    assert(q[2] == 3);
    assert(q[3] == 4);
    my_free(q);
    printf("After free2\n");
    printf("You need to check every freelistnode's address is chunked in right size\n");
    printf("In here you have have 2 node and the size is 24 and 8168\n");
    n = free_list_begin();
    while(n!=NULL){
        printf("free2 %p\n",n);
        printf("free2 %zu\n",n->size);
        n = n -> flink;
    }


    printf("Test 3 in here: just allocate the 16 byte which should get 24 byte chunk because header\n");
    printf("In here you have have 2 node and the size is 24 and 8168\n");
    int* b = (int*)my_malloc(sizeof(int)*4);
    memcpy(&i,(char*)b -8,4);
    assert(i ==24);
    my_free(b);
    printf("After free3\n");
    n = free_list_begin();
    printf("You need to check every freelistnode's address is chunked in right size\n");
    printf("In here you have have 2 node and the size is 24 and 8168\n");
    while(n!=NULL){
        printf("free3 %p\n",n);
        printf("free3 %zu\n",n->size);
        n = n -> flink;
    }


    printf("Test 4 in here: just allocate the 20 byte which should get 32 byte chunk because header\n");
    int* w = (int*)my_malloc(sizeof(int)*5);
    memcpy(&i,(char*)w-8,4);
    printf("%d\n", i);
    assert(i == 32);
    my_free(w);
    printf("After free4\n");
    printf("You need to check every freelistnode's address is chunked in right size\n");
    printf("In here you have have 3 node and the size is 24, 32 and 8136\n");
    n = free_list_begin();
    while(n!=NULL){
        printf("free4 %p\n",n);
        printf("free4 %zu\n",n->size);
        n = n -> flink;
    }


    printf("Test5 we will coalesce the free list node in here\n");
    coalesce_free_list();
    printf("After coalesce, just one node and size is 8192\n");
    n = free_list_begin();
    while(n!=NULL){
        printf("Coalesce %p\n",n);
        printf("Coalesce %zu\n",n->size);
        n = n -> flink;
    }


    printf("Test 6 in here: just allocate the 20 byte which should get 32 byte chunk because header\n");
    int* a= (int*)my_malloc(sizeof(int)*5);

    memcpy(&i,(char*)a -8,4);
    printf("108th %d\n", i);

    assert(i == 32);
    my_free(a);
    printf("After free6\n");
    n = free_list_begin();
    printf("In here you have 2 nodes and the size is 32 and 8160\n");
    while(n!=NULL){
        printf("free6 %p\n",n);
        printf("free6 %zu\n",n->size);
        n = n -> flink;
    }


    printf("Test 7 in here: just allocate the 8192 byte which should get 8200 byte chunk because header\n");
    char* s = (char*)my_malloc(sizeof(char)*8192);
    memcpy(&i,(char*)s -8,4);



    assert(i == 8200);
    my_free(s);
    printf("After free7\n");
    n = free_list_begin();
    printf("In here you have 3 nodes and the size is 32, 8160 and 8200\n");
    while(n!=NULL){
        printf("free7 %p\n",n);
        printf("free7 %zu\n",n->size);
        n = n -> flink;
    }




    printf("Test 8 in here: just allocate the 63 byte which should get 72 byte chunk because header\n");
    s = (char*)my_malloc(sizeof(char)*63);
    memcpy(&i,(char*)s -8,4);
    assert(i == 72);
    my_free(s);
    printf("After free8\n");
    n = free_list_begin();
    printf("In here you have 4 nodes and the size is 32, 72, 8088 and 8200\n");
    while(n!=NULL){
        printf("free8 %p\n",n);
        printf("free8 %zu\n",n->size);
        n = n -> flink;
    }


    printf("Test 9 in here: allocate the 20, 63, 8080, and 8192 bytes and because  header you should get 32, 72, 8088 and 8200 bytes \n");
    char* s1 = (char*)my_malloc(sizeof(char)*20);
    char* s2 = (char*)my_malloc(sizeof(char)*63);
    char* s3 = (char*)my_malloc(sizeof(char)*8080);
    char* s4 = (char*)my_malloc(sizeof(char)*8192);
    assert(s2 - s1 == 32);
    assert(s3 - s2 == 72);
    assert(s4 - s3 == 8088);
    s3[0] = 'a';
    s3[1] = 'b';
    s3[2] = 'c';
    s3[3] = '\0';
    assert(s3[0]=='a');
    assert(s3[1] == 'b');
    assert(s3[2] == 'c');
    n = free_list_begin();
    printf("In here you have 0 node\n");
    while(n!=NULL){
        printf("free9 %p\n",n);
        printf("free9 %zu\n",n->size);
        n = n -> flink;
    }


    printf("Test 10 in here: allocate 55 bytes and get 72 bytes because of potential wastage\n");
    my_free(s2);

    my_free(s3);
    n = free_list_begin();
    printf("In here you have 2 nodes and size is 72 and 8088\n");
    while(n!=NULL){
        printf("free10 %p\n",n);
        printf("free10 %zu\n",n->size);
        n = n -> flink;
    }


    char* s5 = (char*)my_malloc(sizeof(char)*55);
    assert(s2 == s5);
    printf("%p %p\n",s2,s5);
    my_free(s5);


    printf("Test 11 in here will split it 10 times");
    coalesce_free_list();
    n = free_list_begin();
    printf("In here you have 1 nodes and size is 8160\n");
    while(n!=NULL){
        printf("free11 %p\n",n);
        printf("free11 %zu\n",n->size);
        n = n -> flink;
    }



    printf("Test 12 Now we are going to allocate 7 chunks which has size 7 15 23 31 39 47 55 63 71 79 \n");
    char* a1 = (char*)my_malloc(sizeof(char)*7);
    char* a2 = (char*)my_malloc(sizeof(char)*15);
    char* a3 = (char*)my_malloc(sizeof(char)*23);
    char* a4 = (char*)my_malloc(sizeof(char)*31);
    char* a5 = (char*)my_malloc(sizeof(char)*39);
    char* a6 = (char*)my_malloc(sizeof(char)*47);
    char* a7 = (char*)my_malloc(sizeof(char)*55);
    char* a8 = (char*)my_malloc(sizeof(char)*63);
    char* a9 = (char*)my_malloc(sizeof(char)*71);
    char* a10 = (char*)my_malloc(sizeof(char)*79);
    memcpy(&i,(char*)a1-8,4);
    assert(i == 16);
    memcpy(&i,(char*)a2-8,4);
    assert(i == 24);
    memcpy(&i,(char*)a3-8,4);
    printf("%d\n",i);
    assert(i == 32);
    memcpy(&i,(char*)a4-8,4);
    assert(i == 40);
    memcpy(&i,(char*)a5-8,4);
    assert(i == 48);
    memcpy(&i,(char*)a6-8,4);
    assert(i == 56);
    memcpy(&i,(char*)a7-8,4);
    assert(i == 64);
    memcpy(&i,(char*)a8-8,4);
    assert(i == 72);
    memcpy(&i,(char*)a9-8,4);
    assert(i == 80);
    memcpy(&i,(char*)a10-8,4);
    assert(i == 88);


    assert(a2 - a1 == 16);
    assert(a3 - a2 == 24);
    assert(a4 -a3 == 32);
    assert(a5-a4 == 40);
    assert(a6 - a5 == 48);
    assert(a7 - a6 == 56 );
    assert(a8 - a7 == 64);
    assert(a9 - a8 == 72);
    assert(a10 - a9 == 80);

    my_free(a1);
    my_free(a2);
    my_free(a3);
    my_free(a4);
    my_free(a5);
    my_free(a6);
    my_free(a7);
    my_free(a8);
    my_free(a9);
    my_free(a10);
    n = free_list_begin();
    printf("In here you have 11 nodes and size is 16 24 32 40 48 56 64 72 80 88 7640 \n");
    while(n!=NULL){
        printf("free12 %p\n",n);
        printf("free12 %zu\n",n->size);
        n = n -> flink;
    }



    coalesce_free_list();
    n = free_list_begin();
    printf("In here you have 1 nodes and size is 8160\n");
    while(n!=NULL){
        printf("free12 %p\n",n);
        printf("free12 %zu\n",n->size);
        n = n -> flink;
    }


    char* k =  (char*)my_malloc(sizeof(char)*80);
    char* k1 = (char*)my_malloc(sizeof(char)*85);
    char* k2 = (char*)my_malloc(sizeof(char)*95);
    my_free(k);
    my_free(k2);
    n = free_list_begin();
    while(n!=NULL){
        printf("262th %p %d\n",n,n->size);
        n = n -> flink;
    }
    coalesce_free_list();
    n = free_list_begin();
    while(n!=NULL){
        printf("268th %p %d\n",n,n->size);
        n = n -> flink;
    }
    exit(0);
    return 0;
}