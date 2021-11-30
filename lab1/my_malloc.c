// This code is my own work, it was written without 
// consulting a tutor or code written by other students-Shuang Wu
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "my_malloc.h"


#define magic_number 2021551
FreeListNode first_node = NULL;
void* heap_cur_end = NULL;
void* heap_start = NULL;
int my_malloc_flag = 0;
//FreeListNode second_node  = NULL;
//FreeListNode third_node  = NULL;

//error signaling
//typedef enum {MYNOERROR, MYENOMEM, MYBADFREEPTR} MyErrorNo;
MyErrorNo my_errno = MYNOERROR;


void set_header(unsigned int size,void* ptr)
{
    *((unsigned int*)((char*)ptr + 4)) = magic_number;
    *((unsigned int*)((char*)ptr + 0)) = size;
}
void remove_node(FreeListNode ptr)
{
    // ptr IS THE FIRST ONE OR THE ONLY ONE
    if (ptr == free_list_begin())
    {
        first_node = ptr->flink;
        ptr->flink = NULL;
        return;
    }
    else
    {
        FreeListNode temp_ptr = free_list_begin();
        while (temp_ptr->flink != ptr)
        {
            temp_ptr = temp_ptr->flink;
        }
        temp_ptr->flink = temp_ptr->flink->flink;
        ptr->flink = NULL;
        return;
    }

}

void coalesce_free_list(void)
{
    FreeListNode temp_ptr = free_list_begin();
    if (temp_ptr == NULL || temp_ptr->flink == NULL)
    {
        return;
    }
    else
    {
        while (temp_ptr->flink != NULL)
        {
            if ((void *)temp_ptr + temp_ptr->size == temp_ptr->flink)
            {
                temp_ptr->size = temp_ptr->flink->size + temp_ptr->size;
                temp_ptr->flink = temp_ptr->flink->flink;
            }
            else
            {
                temp_ptr = temp_ptr->flink;
            }
        }
        return;
    }
}

unsigned int get_size_needed(unsigned int size)
{
    if (size <= 16)
    {
        return 16;
    }
    else
    {
        if (size%8 == 0)
        {
            return size;
        }
        else
        {
            return size + (CHUNKHEADERSIZE - (size % 8));
        }
    }
}

void* split(unsigned int rest_part_size, void* start_of_rest)
{
    FreeListNode rest_part;
    rest_part = (FreeListNode)start_of_rest;
    rest_part->size = rest_part_size;
    rest_part->flink = NULL;
    return rest_part;
}

void insert_node(FreeListNode inserted_node)
{

    FreeListNode head = free_list_begin();

    if (free_list_begin() == NULL)
    {
        first_node = inserted_node;
        free_list_begin()->flink = NULL;
        return;
    }

    else
    {
        if (free_list_begin()->flink == NULL)
        {
            if (head < inserted_node)
            {
                free_list_begin()->flink = inserted_node;

                inserted_node->flink = NULL;

                return;
            }
            else
            {
                inserted_node->flink = head;
                first_node = inserted_node;

                inserted_node->flink->flink = NULL;
                return;
            }
        }
        if (inserted_node < first_node)
        {
            inserted_node->flink = head;
            first_node = inserted_node;
            return;
        }

        while (head->flink != NULL)
        {
            if (head->flink < inserted_node)
            {
                head = head->flink;
                continue;
            }
            if (head->flink > inserted_node)
            {
                inserted_node->flink = head->flink;
                head->flink = inserted_node;
                return;
            }
        }
        if (head->flink == NULL)
        {
            head->flink = inserted_node;
            return;
        }
    }
}

void my_free(void* ptr)
{
    if(ptr > sbrk(0) || ptr - 8 <heap_start || ptr == NULL)
    {
        my_errno = MYBADFREEPTR;
        return;
    }

    ptr = ptr-8;
    int magic;
    memcpy(&magic,(int*)(ptr+4),4);
    int node_size;
    memcpy(&node_size,(int*)(ptr+0),4);

    if (ptr == NULL || magic != 2021551)
    {
        my_errno = MYBADFREEPTR;
        return;
    }
    FreeListNode free_node_inserted;
    free_node_inserted = (FreeListNode)ptr;
    free_node_inserted -> size = node_size;

    free_node_inserted->flink = NULL;
    insert_node(free_node_inserted);

}

FreeListNode free_list_begin(void)
{
    return first_node;
}

//my_malloc: returns a pointer to a chunk of heap allocated memory
void* my_malloc(size_t size) {
    if(heap_start == NULL) {
        heap_start = sbrk(0);
//        printf("sbrk(0) in my malloc: %p\n", heap_start);
    }

    unsigned int size_needed = get_size_needed(size + CHUNKHEADERSIZE);
    // IF FREE_LIST IS EMPTY
//    printf("%d\n",size_needed);
    if (free_list_begin() == NULL) {
        if (size_needed > 8192) {
            if (!sbrk(size_needed)) {
                my_errno = MYENOMEM;
                return NULL;
            }
            void * ptr = sbrk(size_needed);
            set_header(size_needed, ptr);

            FreeListNode node;
            node = (FreeListNode)ptr;
            return node;

        } else {
            void *chunk_ptr = sbrk(8192);
            if(chunk_ptr == NULL)
            {
                my_errno = MYENOMEM;
                return NULL;
            }

            // IF 8192 CHUNK NEED TO BE SPLIT
            if (8192 - size_needed > 16) {
                void *start_of_rest = chunk_ptr + size_needed;
                unsigned int rest_part_size = 8192 - size_needed;

                FreeListNode used_node;
                used_node = (FreeListNode)chunk_ptr;
                used_node->size = size_needed;
                used_node->flink = NULL;
                set_header(size_needed, chunk_ptr);

                FreeListNode rest_node;
                rest_node = (FreeListNode)start_of_rest;
                rest_node->size = rest_part_size;
                rest_node->flink = NULL;

                set_header(rest_part_size, start_of_rest);

                insert_node(rest_node);

//                printf("sbrk(0) in my malloc: %p\n", sbrk(0));

                return chunk_ptr+8;
            } else {
                set_header(size_needed, chunk_ptr);
                return chunk_ptr+8;
            }
        }
    }
        // IF FREE LIST IS NOT NULL
    else {
        // if there is only one node in the list
        FreeListNode ptr = free_list_begin();

        if (ptr->flink == NULL && ptr->size == size_needed) {
            void* temp_ptr = ptr;
            first_node = NULL;
            return temp_ptr+8;
        }
        if (ptr->flink == NULL && ptr->size >= size_needed) {
            void *temp_ptr = ptr;
            void *start_of_rest = temp_ptr+size_needed;

            unsigned int rest_part_size = ptr->size-size_needed;
            ptr->size = size_needed;

            ptr->flink = NULL;

            FreeListNode rest_part;
            FreeListNode used_node;

            rest_part = (FreeListNode)start_of_rest;
            rest_part->size = rest_part_size;
            rest_part->flink = NULL;

            first_node = rest_part;
            first_node->flink = NULL;

            void* p = ptr;

            set_header(size_needed, p);

            return p+8;
        }
        if (ptr->flink == NULL && ptr->size < size_needed) {
            void *chunk_ptr = sbrk(8192);
            if(chunk_ptr == NULL)
            {
                my_errno = MYENOMEM;
                return NULL;
            }
            set_header(size_needed, chunk_ptr);
            // IF 8192 CHUNK NEED TO BE SPLIT
            if (8192 - size_needed > 16) {
                void *start_of_rest = chunk_ptr + size_needed;
                unsigned int rest_part_size = 8192 - size_needed;
                //printf("split here!\n");

                FreeListNode rest_part = split(rest_part_size, start_of_rest);
                //printf("insert here!\n");
                insert_node(rest_part);

                return chunk_ptr+8;

            }
        }
        if (free_list_begin()->size >= size_needed)
        {
            if (ptr->size - size_needed <= 16)
            {
                FreeListNode used_node;
                used_node = free_list_begin();
                FreeListNode rest_node;
                rest_node = used_node->flink;

                used_node->flink = NULL;
                first_node = rest_node;

                void* temp_ptr= used_node;
                set_header(ptr->size, temp_ptr);

                return temp_ptr+8;
            }
            else
            {
                void* start = ptr;
                FreeListNode next_part = ptr->flink;
                unsigned int rest_part_size = ptr->size-size_needed;
                void* rest_begin = start+size_needed;
                FreeListNode rest_part = rest_begin;

                rest_part->flink = next_part;
                rest_part->size = rest_part_size;

//                set_header(rest_part_size, rest_begin);

                first_node = rest_part;

                set_header(size_needed, start);


                return start+8;
            }
        }

        while (ptr->flink != NULL)
        {
            if (ptr->flink->size < size_needed) {
                ptr = ptr->flink;
                continue;
            }
            if (ptr->flink->size >= size_needed)
            {
                if (ptr->flink->size - size_needed <= 16)
                {
                    void* start = ptr->flink;
                    void* next_part = ptr->flink->flink;
                    size_needed = ptr->flink->size;
                    ptr->flink = next_part;
                    set_header(size_needed, start);
                    return start+8;
                }
                else
                {
                    void* start = ptr->flink;
                    void* next_part = ptr->flink->flink;
                    unsigned int rest_part_size = ptr->flink->size-size_needed;
                    void* rest_begin = start+size_needed;
                    FreeListNode rest_part = rest_begin;
                    rest_part->flink = next_part;
                    rest_part->size = rest_part_size;
                    ptr->flink = rest_part;
                    set_header(size_needed, start);
                    return start+8;
                }
            }
        }
        if (ptr->flink == NULL)
        {
            if (size_needed > 8192)
            {
                void* temp_ptr = sbrk(size_needed);
                if(temp_ptr == NULL)
                {
                    my_errno = MYENOMEM;
                    return NULL;
                }
                set_header(size_needed, temp_ptr);

                return temp_ptr+8;
            }
        }
    }
}
//
//
//void print_free_list(FreeListNode fhead){
//
//    printf("freelist: ");
//
//    if (NULL == fhead){
//        printf("NULL\n");
//    }
//
//    else
//    {
//        FreeListNode fnode = fhead;
//        while (fnode) {
//            printf("%u ", fnode->size);
//            fnode = fnode->flink;
//        }
//        printf("\n");
//    }
//}
////
////
////
//int main(int argc, char* agrv[]){
//
//
//    int sizes[3] = {10, 50, 100};
//    int *p[3];
//    FreeListNode fhead = free_list_begin(); // fhead should be NULL here
//
//    for (int i = 0; i < 3; i++){
//        p[i] = (int*) my_malloc(sizeof(int) * sizes[i]); //chunk size is 4 * sizes[i]
//        fhead = free_list_begin();
//        print_free_list(fhead);
//        my_free(p[i]);
//
//        fhead = free_list_begin();
//        print_free_list(fhead);
//
//    }
//
//    /*
//    * If your implementation is correct, you should print the following information
//    * freelist: 8144
//    * freelist: 48 8144
//    * freelist: 48 7936
//    * freelist: 48 208 7936
//    * freelist: 48 208 7528
//    * freelist: 48 208 408 7528
//    */
//
//    return 0;
//
//}
