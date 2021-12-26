#include <unistd.h>
#include <stdio.h>
#include <ar.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>

struct meta {
    char name[16]; //room for null
    int mode;
    int size;
    time_t mtime; // a time_t is a long
};


// int argc means the number of parameters
// argc[] means all parameters
void append()
{
    ;
}

void extract()
{
    ;
}


int fill_ar_hdr(char *filename, struct ar_hdr *hdr);
int fill_meta( struct ar_hdr hdr, struct meta *meta);

int main(int argc, char * argv[])
{
//    struct meta hehe;
//    strcpy(hehe.name, "123");
//    hehe.size = 345;
//    hehe.mode = 777;
//    printf("%s",hehe.name);

;
    int ch;
    while ((ch = getopt(argc, argv, "qxotvd:A:")) != -1)
    {
        printf("reading\n");
    }
    printf("%d\n", argc);

    if(argc<3)
    {
        printf("No enough parameters!\n");
        exit(1);
    }


    char* parameter = argv[1];
    char option = *(parameter+1);
    char* arname = argv[2];
    char* filename = argv[3];

    if (option != 'q' && option != 'x'&& option != 't' && option !='v' && option !='d' && option != 'A')
    {
        printf("Please use the correct parameters!\n");
        exit(1);
    }

    switch (option)
    {
        // Quickly append the files specified to the archive-file
        case 'q':
//            printf("qqqqqqqqqq\n");
//            ar_fd = open(arname, O_RDWR|O_CREAT, 0666);
//            printf("Archive file not found; creating new archive file\n");
//            write(ar_fd, ARMAG, 8);
//            append();
            break;
            // Extract the specified files from the archive-file
        case 'x':
            creat(arname, '0666');
            printf("xxxxxxxxxxx\n");
            break;
            // Used in combination with “x” restore the original permission and mtime
        case 'o':
            //printf("cccccccccc\n");
            break;
            // List the file names in the archive-file
        case 't':
            printf("tttttttt\n");
            struct stat *f_stat = malloc(sizeof(struct stat));
            struct ar_hdr * file_header = malloc(sizeof(struct ar_hdr));
            int fd = open(arname, O_RDWR);
//            On success, open(), openat(), and creat() return the new file
//            descriptor (a nonnegative integer).  On error, -1 is returned and
//            errno is set to indicate the error.
            if(fd == -1)
            {
                printf("open fail!\n");
                break;
            }

//            printf("%d\n",filesize);
            char * filesize;
            char buf[1024]={"\0"};
            int size = 0;
            while(read(fd, file_header, 60)!=0)
            {
                int j;
                for(j=16; j>=0; j--){
                    if(file_header->ar_name[j] == '/'){
                        file_header->ar_name[j] = '\0';
                    }
                }
                printf("------------------\n");
                printf("%s\n",file_header->ar_name);
                printf("------------------\n");
                for(j=12; j>=0; j--){
                    if(file_header->date[j] == '/'){
                        file_header->date[j] = '\0';
                    }
                }
                printf("------------------\n");
                printf("%s\n",file_header->ar_date);
                printf("------------------\n");
                break;
            }
//            printf("%s\n",file_header->ar_name);
//            printf("%s\n",file_header->ar_size);
//
//
//            printf("------------------\n");
//
//            printf("%s\n",buf);
//
//            printf("%s\n",file_header->ar_name);
//            printf("%s\n",file_header->ar_uid);
//            printf("%s\n",file_header->ar_mode);
//            printf("%s\n",file_header->ar_size);
//            printf("------------------\n");
//            int i = 0;
//            while(read(fd,file_header,sizeof(struct ar_hdr)) == sizeof(struct ar_hdr))
//            {
//                int j;
//                for (j = 0; j < 16; j++){
//                    if (file_header->ar_name[i] == '/') {
//                        file_header->ar_name[i] = '\0';
//                        break;
//                    }
//                }
//            }
//            printf("%s\n",file_header->ar_name);
//            printf("------------------\n");
//
//
//            printf("%s\n",file_header->ar_size);
//
//            printf("%d\n",fd);
//            fstat(fd,f_stat);
//            printf("%ld\n",f_stat->st_atime);
//            printf("%ld\n",f_stat->st_ctime);
//            printf("%ld\n",f_stat->st_ino);
//            printf("%ld\n",f_stat->st_size);

//            struct ar_hdr * file_header = malloc(sizeof(struct ar_hdr));



            break;
            // Used in combination with “t” print the verbose info as “ar” does.
        case 'v':
            //printf("eeeeeeeee\n");
            break;
            // Used in combination with “t” print the verbose info as “ar” does.
        case 'd':
            //printf("fffffffffffff\n");
            break;
            // Quickly append all regular files in the directory modified more than N days ago
        case 'A':
            //printf("ggggggggggggg\n");
            break;
        case '?':
            break;
    }
}
