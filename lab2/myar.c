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
#include <errno.h>
#include <utime.h>
#include <dirent.h>


struct meta {
    char name[16]; //room for null
    int mode;
    int size;
    time_t mtime; // a time_t is a long
};

int D2O(int n)
{
    int octalnum = 0;
    int temp = 1;

    while (n != 0)
    {
        octalnum = octalnum + (n % 8) * temp;
        n = n / 8;
        temp = temp * 10;
    }

    return octalnum;
}

int get_stat_permission(struct stat* stat_info)
{
    int user_permission = 0;
    int group_permission = 0;
    int other_permission = 0;

    if((stat_info->st_mode & S_IRUSR)!=0)
    {
        user_permission += 4;
    }
    if((stat_info->st_mode & S_IRUSR)!=0)
    {
        user_permission += 2;
    }
    if((stat_info->st_mode & S_IXUSR)!=0)
    {
        user_permission += 1;
    }
    if((stat_info->st_mode & S_IRGRP)!=0)
    {
        group_permission += 4;
    }
    if((stat_info->st_mode & S_IWGRP)!=0)
    {
        group_permission += 2;
    }
    if((stat_info->st_mode & S_IXGRP)!=0)
    {
        group_permission += 1;
    }
    if((stat_info->st_mode & S_IROTH)!=0)
    {
        other_permission += 4;
    }
    if((stat_info->st_mode & S_IWOTH)!=0)
    {
        other_permission += 2;
    }
    if((stat_info->st_mode & S_IXOTH)!=0)
    {
        other_permission += 1;
    }
    return 100*user_permission+10*group_permission+other_permission;
}

void timestamp_to_date(long mtimestamp, char* buffer, long timestamp_size)
{
    struct tm * timeinfo;
//    printf("%ld\n", mtimestamp);
    timeinfo = localtime (&mtimestamp);
    strftime (buffer,timestamp_size,"%b %d %H:%M %Y",timeinfo);
}

void print_mode(int mode_num)
{
    char mode_str[3];
    sprintf(mode_str, "%d", mode_num);
    int digit;
    int temp = 100;
    for(int i = 0; i<3 ;i++)
        {
            digit = mode_num / temp;
            if(7 == digit)
            {
                printf("rwx");
            }
            if(6 == digit)
            {
                printf("rw-");
            }
            if(5 == digit)
            {
                printf("r-x");
            }
            if(4 == digit)
            {
                printf("r--");
            }
            if(3 == digit)
            {
                printf("-wx");
            }
            if(2 == digit)
            {
                printf("-w-");
            }
            if(1 == digit)
            {
                printf("--x");
            }
            if(0 == digit)
            {
                printf("---");
            }
            mode_num = mode_num - digit*temp;
            temp/=10;
        }
        printf(" ");
}


int fill_ar_hdr(char *filename, struct ar_hdr *hdr)
{
    struct stat* stat_info = malloc(sizeof(struct stat));
    int fd = open(filename, O_APPEND | O_RDWR, 0666);
    fstat(fd, stat_info);

    // write name to header
    char name_str[16];
    for(int i=0; i<16; i++)
    {
        hdr->ar_name[i] = '\0';
    }
    strcpy(hdr->ar_name, filename);
//    hdr->ar_name[sizeof(filename)+1] = '/';
    int flag = 0;
    for(int i=0; i<16; i++)
    {
        if(hdr->ar_name[i] == '\0' && flag == 0)
        {
            hdr->ar_name[i] = '/';
            flag = 1;
            continue;
        }
        if(hdr->ar_name[i] == '\0')
        {
            hdr->ar_name[i] = ' ';
            continue;
        }
    }
    // write date to header
    char date_str[12];
    sprintf(date_str, "%-12ld", stat_info->st_mtime);
    strcpy(hdr->ar_date, date_str);

    // write uid and gid to header
    char uid_str[6];
    char gid_str[6];
    sprintf(uid_str, "%-6d", stat_info->st_uid);
    sprintf(gid_str, "%-6d", stat_info->st_gid);
    strcpy(hdr->ar_uid, uid_str);
    strcpy(hdr->ar_gid, gid_str);

    // write mode to header
    char mode_str[8];

    sprintf(mode_str, "%-8d", D2O(stat_info->st_mode));
    strcpy(hdr->ar_mode, mode_str);

    // write the size to header
    char size_str[10];
    sprintf(size_str, "%-10ld", stat_info->st_size);
    strcpy(hdr->ar_size, size_str);
    // write ARFMAG to header end
    strcpy(hdr->ar_fmag, ARFMAG);

    free(stat_info);
    close(fd);
}
int fill_meta(struct ar_hdr hdr, struct meta *meta)
{
    // write the name to meta_data
    for(int i=0;i<16;i++)
    {
        meta->name[i] = '\0';
    }
    strcpy(meta->name, hdr.ar_name);
    int i = 0;
    while(meta->name[i] != '/')
    {
        i++;
    }
    meta->name[i]='\0';

    // write the date to meta_data
    time_t mtime;
    mtime = strtol(hdr.ar_date,NULL,10);
    meta->mtime = mtime;
//    timestamp_to_date(mtime,date,20);
    // write the mode to meta_data

    int a = hdr.ar_mode[3]-'0';
    int b = hdr.ar_mode[4]-'0';
    int c = hdr.ar_mode[5]-'0';
    meta->mode = a*100+b*10+c;

    // write the size to meta_data
    meta->size = (int)strtol(hdr.ar_size,NULL,10);

}

int t_and_v(char* filename, int verbose)
{
    struct stat *file_stat;
    struct ar_hdr * fileHeader;
    struct ar_hdr hdr;
    struct meta meta;

    int fd = open(filename, O_APPEND | O_RDWR, 0666);
    if(fd == -1)
    {
        return 1;
    }
    lseek(fd,8,SEEK_SET);
    char arname[16];
    int i = 0;
    int uid = 0;
    int gid = 0;
    char date[20];

    while(read(fd, &hdr, 60)!=0)
    {
        uid = atoi(hdr.ar_uid);
        gid = atoi(hdr.ar_gid);

        fill_meta(hdr, &meta);
        timestamp_to_date(meta.mtime,date,20);

        if(verbose == 0)
        {
            printf("%s\n",meta.name);
        }
        if(verbose == 1)
        {
            print_mode(meta.mode);
            printf("%4d",uid);
            printf("/");
            printf("%4d",gid);

            printf("%7d ",meta.size);
            printf("%s ",date);
            printf("%s\n",meta.name);
        }
        lseek(fd,meta.size,SEEK_CUR);
        if(meta.size%2 != 0)
        {
            lseek(fd,1,SEEK_CUR);
        }
    }
    close(fd);
    return 0;
}

int append(char* arname, char*filename, char ** argv, int argc, int from_outer)
{
    struct meta meta;
    struct ar_hdr hdr;
    if(from_outer == 1)
    {
        int file_descriptor_ = open(arname,2);
        if(file_descriptor_ == -1)
        {
            file_descriptor_ = creat(arname, 0666);
            write(file_descriptor_,ARMAG,8);
        }
        close(file_descriptor_);
        if(argc == 3)
        {
            return 0;
        }
    }

    int fd_arname = open(arname, O_APPEND | O_RDWR, 0666);

    for(int i = 3; i<argc; i++)
    {
        filename = argv[i];

        fill_ar_hdr(filename, &hdr);
        int fd_filename = open(filename, O_APPEND | O_RDWR, 0666);
        int content_size = (int)strtol(hdr.ar_size,NULL,10);
        char* content_buf[content_size];
        read(fd_filename, content_buf, content_size);

        lseek(fd_arname, 0,SEEK_END);
        write(fd_arname,&hdr, 60);
        lseek(fd_arname, 60,SEEK_CUR);
        write(fd_arname,content_buf, content_size);
        lseek(fd_arname, content_size,SEEK_CUR);
        if ((lseek(fd_arname,0,SEEK_END)%2))
        {
            write(fd_arname,"\n",1);
        }
        close(fd_filename);
    }
    close(fd_arname);
    return 0;
}

int extract(char* arname,char ** argv, int num_of_files, int fd, int xo)
{
    struct utimbuf timebuf;
    struct meta meta;
    struct ar_hdr hdr;
    char* file_name[16];
    int j;
    int mode = 0;
    int flag;
    int external_fd;
    char* content[200];
    int content_size;
    lseek(fd,8,SEEK_SET);
    while(read(fd, &hdr, 60)!=0)
    {
//        write(STDOUT_FILENO,&hdr,60);
        for(int i = 3; i < 3+num_of_files; i++)
        {
            j = 0;
            flag = 1;
            while(hdr.ar_name[j] != '/')
            {
                if(hdr.ar_name[j] != argv[i][j])
                {
                    flag = 0;
                    break;
                }
                j++;
            }
            if(flag==1)
            {
                external_fd = creat(argv[i], 0666);
                content_size = (int)strtol(hdr.ar_size,NULL,10);
                read(fd, content, content_size);
                write(external_fd, content, content_size);
                if(content_size%2==1)lseek(fd, 1, SEEK_CUR);
                close(external_fd);
                if(xo==1)
                {
                    // restore mode
                    int a = hdr.ar_mode[3]-'0';
                    int b = hdr.ar_mode[4]-'0';
                    int c = hdr.ar_mode[5]-'0';
                    mode = a*100+b*10+c;
                    chmod(argv[i], (mode_t) mode);

                    // restore mtime
                    fill_meta(hdr, &meta);
                    timebuf.modtime = meta.mtime;
                }
                break;
            }
            if(flag==0)continue;
        }
        if(flag==0)
        {
            int move = atoi(hdr.ar_size);
            if(move%2==1)move++;
            lseek(fd, move, SEEK_CUR);
        }
    }
    close(fd);
    return 0;
}

int check_file_time_N_days(char* filename, int N)
{
    float time_difference;
    long seconds;
    time_t present_time = time(0);
    struct stat *f_stat = malloc(sizeof(struct stat));
    stat(filename,f_stat);
    time_difference = difftime(present_time, f_stat->st_mtime);
    seconds = 60*60*24*N;
    if(time_difference >= seconds)
    {
        free(f_stat);
        return 1;
    }
    else
    {
        free(f_stat);
        return 0;
    }
}

int check_txt_file(char* filename)
{
    int flag = 0;
    if(filename[strlen(filename)-4] == '.')
    {
        flag++;
    }
    if(filename[strlen(filename)-3] == 't')
    {
        flag++;
    }
    if(filename[strlen(filename)-2] == 'x')
    {
        flag++;
    }
    if(filename[strlen(filename)-1] == 't')
    {
        flag++;
    }
    if(flag == 4)
    {
        return 1;
    }
    return 0;
}

int append_N_days_files(char* arname, int N, char ** argv, int num_of_files)
{
    int file_descriptor;
    struct utimbuf time_buf;
    char* filename = argv[3];
    DIR *dp;
    dp = opendir("./");
    struct dirent *dirent;
    char* argv_for_append[1000];
    int argc_for_append = 3;
    while (dirent = readdir(dp))
    {
        if (check_file_time_N_days(dirent->d_name, N)==1 && check_txt_file(dirent->d_name)==1)
        {
            argv_for_append[argc_for_append] = dirent->d_name;
            argc_for_append++;
        }
    }
    return append(arname, filename, argv_for_append, argc_for_append, 1);
}

int delete_file(char* arname,char ** argv, int num_of_files, int fd)
{
    struct meta meta;
    struct ar_hdr hdr;
    char* file_name[16];
    int j;
    int mode = 0;
    int flag;
    char* content[200];
    int content_size;
    char* argv_for_append[1000];
    int argc_for_append = 3;
    unlink(arname);

    lseek(fd,8,SEEK_SET);
    while(read(fd, &hdr, 60)!=0)
    {
//        write(STDOUT_FILENO,&hdr,60);
        for(int i = 3; i < 3+num_of_files; i++)
        {
            j = 0;
            flag = 1;
            while(hdr.ar_name[j] != '/')
            {
                if(hdr.ar_name[j] != argv[i][j])
                {
                    flag = 0;
                    break;
                }
                j++;
            }
            if(flag==1)
            {
                break;
            }
            if(flag==0)
            {
                fill_meta(hdr,&meta);
                argv_for_append[argc_for_append] = meta.name;
                argc_for_append++;
            }
        }

        int move = atoi(hdr.ar_size);
        if(move%2==1)move++;
        lseek(fd, move, SEEK_CUR);
    }
    return append(arname, file_name, argv_for_append, argc_for_append, 1);
}

int main(int argc, char * argv[])
{
    int ch;
    while ((ch = getopt(argc, argv, "qxotvd:A:")) != -1)
    {
//        printf("reading\n");
    }

    if(argc<3)
    {
        return 1;
    }

    struct utimbuf tbuf;
    int num_of_files = argc-3;
    char* parameter = argv[1];
    char option = *(parameter+1);
    char* arname = argv[2];
    char* filename = argv[3];
    int file_descriptor;
    char* test[100];
    int xo;
    int N;
    mode_t mask;

    if (option != 'q' && option != 'x'&& option != 't' && option !='v' && option !='d' && option != 'A')
    {
        return 1;
    }

    switch (option)
    {
        // Quickly append the files specified to the archive-file
        // padding is '\0'
        case 'q':
            file_descriptor = open(arname,2);
            if(file_descriptor == -1)
            {
                file_descriptor = creat(arname, 0666);
                write(file_descriptor,ARMAG,8);
            }
            return append(arname, filename, argv, argc, 0);
            // Extract the specified files from the archive-file
        case 'x':
            file_descriptor = open(arname,2);
            if(file_descriptor == -1)
            {
                return 1;
            }

            if(strcmp("-xo", parameter)==0)
            {
                xo = 1;
            }
            return extract(arname, argv, num_of_files, file_descriptor, xo);

            // Used in combination with “x” restore the original permission and mtime

            // List the file names in the archive-file
        case 't':
            if(strcmp("-t", parameter)==0)
                {
                    return t_and_v(arname, 0);;
                }
            if(strcmp("-tv", parameter)==0)
                {
                    return t_and_v(arname, 1);
                }
            return 1;

        case 'd':
            file_descriptor = open(arname,2);
            if(file_descriptor == -1)
            {
                return 1;
            }
            return delete_file(arname, argv, num_of_files, file_descriptor);

            break;
            // Quickly append all regular files in the directory modified more than N days ago
        case 'A':

            // usage: ./a.out -A N ar_name.a
            return append_N_days_files(argv[3], (int)strtol(argv[2],NULL,10), argv, num_of_files);
        case '?':
            break;
    }
}
