#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "my_malloc.h"


#define magic_number 2021551
FreeListNode first_node = NULL;
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
            if (temp_ptr->size + temp_ptr != temp_ptr->flink)
            {
                temp_ptr = temp_ptr->flink;
                continue;
            }
            else
            {
                void* ptr1 = temp_ptr;
                void* ptr1_next = temp_ptr->flink->flink;
                unsigned int new_size = temp_ptr->size + temp_ptr->flink->size;
                FreeListNode new_node = ptr1;
                new_node->size = new_size;
                new_node->flink = ptr1_next;
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

void insert_node(FreeListNode rest_part)
{

    FreeListNode head = free_list_begin();
    FreeListNode inserted_node;



    if (head == NULL)
    {
        first_node = rest_part;
        return;
    }

    else
    {
        if (head->flink == NULL)
        {
            if (head < rest_part)
            {
                rest_part = head->flink;
                return;
            }
            else
            {
                rest_part->flink = head;
                first_node = rest_part;
                return;
            }
        }

        if (rest_part < first_node)
        {
            rest_part->flink = head;
            first_node = rest_part;
            return;
        }

        while (head->flink != NULL)
        {
            if (head->flink < rest_part)
            {
                head = head->flink;
                continue;
            }
            if (head->flink > rest_part)
            {
                rest_part->flink = head->flink;
                head->flink = rest_part;
                return;
            }
        }
        if (head->flink == NULL)
        {
            head->flink = rest_part;
            return;
        }
    }
}

void my_free(void* ptr)
{
    if (ptr == NULL || *((int*)(ptr+4)) != 2021551)
    {
        my_errno = MYBADFREEPTR;
        return;
    }

    FreeListNode free_node_inserted = ptr;
    //printf("%u\n",*((int*)(ptr+0)));
    free_node_inserted -> size = *((int*)(ptr+0));
    free_node_inserted->flink = NULL;
    insert_node(free_node_inserted);
}

FreeListNode free_list_begin(void)
{
    return first_node;
}

//my_malloc: returns a pointer to a chunk of heap allocated memory
void* my_malloc(size_t size) {

    unsigned int size_needed = get_size_needed(size + CHUNKHEADERSIZE);

    // IF FREE_LIST IS EMPTY
    if (free_list_begin() == NULL) {
        if (size_needed > 8192) {
            if (sbrk(size_needed) == NULL) {
                my_errno = MYENOMEM;
                return NULL;
            }

            FreeListNode ptr = sbrk(size_needed);

            set_header(size_needed, ptr);


            return ptr + 8;
        } else {
            void *chunk_ptr = sbrk(8192);

            // IF 8192 CHUNK NEED TO BE SPLIT
            if (8192 - size_needed > 16) {
                void *start_of_rest = chunk_ptr + size_needed;
                unsigned int rest_part_size = 8192 - size_needed;

                //printf("split here!\n");

                FreeListNode rest_part = split(rest_part_size, start_of_rest);
                //printf("insert here!\n");
                insert_node(rest_part);
                set_header(size_needed, chunk_ptr);
                return chunk_ptr;
            } else {
                set_header(size_needed, chunk_ptr);
                return chunk_ptr;
            }
        }
    }
        // IF FREE LIST IS NOT NULL
    else {
        // if there is only one node in the list
        FreeListNode ptr = free_list_begin();
        if (ptr->flink == NULL && ptr->size >= size_needed) {
            void *start_of_rest = ptr + size_needed;
            unsigned int rest_part_size = ptr->size;
            FreeListNode rest_part = split(rest_part_size, start_of_rest);
            (ptr + size_needed)->flink = NULL;
            return ptr;
        }

        if (ptr->flink == NULL && ptr->size < size_needed) {
            void *chunk_ptr = sbrk(8192);
            set_header(size_needed, chunk_ptr);
            // IF 8192 CHUNK NEED TO BE SPLIT
            if (8192 - size_needed > 16) {
                void *start_of_rest = chunk_ptr + size_needed;
                unsigned int rest_part_size = 8192 - size_needed;

                //printf("split here!\n");

                FreeListNode rest_part = split(rest_part_size, start_of_rest);
                //printf("insert here!\n");
                insert_node(rest_part);

                return chunk_ptr;

            }
        }

        if (ptr->size >= size_needed)
        {
            if (ptr->size - size_needed <= 16)
            {
                first_node = ptr->flink;
                void* node = ptr;
                size_needed = ptr->size;
                set_header(size_needed, node);
                return node;
            }
            else
            {
                void* start = ptr;
                void* next_part = ptr->flink;
                unsigned int rest_part_size = ptr->size-size_needed;
                void* rest_begin = start+size_needed;
                FreeListNode rest_part = rest_begin;
                rest_part->flink = next_part;
                rest_part->size = rest_part_size;
                first_node = rest_part;
                set_header(size_needed, rest_begin);
                return start;
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
                if (ptr->size - size_needed <= 16)
                {
                    void* start = ptr->flink;
                    void* next_part = ptr->flink->flink;
                    size_needed = ptr->flink->size;
                    ptr->flink = next_part;
                    set_header(size_needed, start);
                    return start;
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
                    return start;
                }
            }
        }
        if (ptr->flink == NULL)
        {
            if (size_needed > 8192)
            {
                if (sbrk(size_needed) == NULL)
                {
                    my_errno = MYENOMEM;
                    return NULL;
                }
                void* last_node = sbrk(size_needed);
                set_header(size_needed, last_node);
                return last_node;
            }
        }
    }
}
//
//
//void print_free_list(FreeListNode fhead){
////    printf("55555555\n");
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
//            printf("%lu ", fnode->size);
//            fnode = fnode->flink;
//        }
//        printf("\n");
//    }
//}
//
//
//
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
