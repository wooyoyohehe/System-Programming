// This code is my own work, it was written without
// consulting a tutor or code written by other students-Shuang Wu
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
int word_count = -1;

int empty_or_not(int sorter_number, int long_length, char temp_list[sorter_number][long_length + 2])
{
    for(int i=0; i<sorter_number; i++)
    {
        if(strcmp(temp_list[i],"{{")!=0) return 0;
    }
    return 1;
}

int find_smallest_index(int sorter_number, int long_length, char temp_list[sorter_number][long_length + 2])
{
    int smallest_index = -1;
    char cur_smallest_word[long_length + 2];
    strcpy(cur_smallest_word, "{{");
    for (int i = 0; i < sorter_number; i++)
    {
        if (strcmp(temp_list[i], cur_smallest_word) < 0)
        {
            strcpy(cur_smallest_word, temp_list[i]);
            smallest_index = i;
        }
    }
    return smallest_index;
}


void sorter(int pipe_parser_sorter[][2] ,int pipe_sorter_merger[][2], int sorter_number){
    for(int i = 0; i<sorter_number;i++){
        pipe(pipe_parser_sorter[i]);
        pipe(pipe_sorter_merger[i]);
        int p = fork();
        // child process
        if(!p){
            close(pipe_parser_sorter[i][1]);
            close(pipe_sorter_merger[i][0]);
            dup2(pipe_parser_sorter[i][0], STDIN_FILENO);
            dup2(pipe_sorter_merger[i][1], STDOUT_FILENO);
            for(int m=0;m<i;m++){
                close(pipe_parser_sorter[m][1]);
                close(pipe_sorter_merger[m][0]);
            }
            execlp("/usr/bin/sort", "sort", NULL);
        }
        else{
            close(pipe_parser_sorter[i][0]);
            close(pipe_sorter_merger[i][1]);
        }
    }
}

void put_words(int pipe_parser_sorter[][2] ,int pipe_sorter_merger[][2], int sorter_number, int short_length, int long_length){
    FILE *put_fp[sorter_number];
    char word[long_length + 1];
    char c;
    int pos = 0;

    for(int i = 0;i<sorter_number;i++){
        put_fp[i] = fdopen(pipe_parser_sorter[i][1], "w");
    }
    while (1)
    {
        c = fgetc(stdin);
        if(c == -1) break;

        // c is alphabet
        if ((c >= 97 && c <= 122) || (c >= 65 && c <= 90))
        {
            if (c >= 65 && c <= 90) c+=32;
            if (pos > long_length) continue;
            word[pos] = c;
            pos++;
            if (pos == long_length)
            {
                word[pos] = '\0';
                pos++;
                word_count++;
                char tmp[strlen(word) + 2];
                strncpy(tmp, word, strlen(word));
                tmp[strlen(word)] = '\n';
                tmp[strlen(word) + 1] = '\0';
                fputs(tmp, put_fp[word_count % sorter_number]);
            }
            continue;
        }
        // c is the illegal char
        else
        {
            if(pos <= short_length || pos > long_length)
            {
                memset(word, 0, long_length+1);
                pos = 0;
                continue;
            }
            if(pos > short_length && pos <= long_length)
            {
                word[pos] = '\0';
                word_count++;
                char tmp[strlen(word) + 2];
                strncpy(tmp, word, strlen(word));
                tmp[strlen(word)] = '\n';
                tmp[strlen(word) + 1] = '\0';
                fputs(tmp, put_fp[word_count % sorter_number]);
                memset(word, 0, long_length+1);
                pos = 0;
            }
        }
    }
    for(int i = 0;i<sorter_number;i++){
        fclose(put_fp[i]);
        close(pipe_parser_sorter[i][1]);
    }
}
void merger(int pipe_parser_sorter[][2] ,int pipe_sorter_merger[][2], int sorter_number, int long_length){
    int count = 0;
    int pid = fork();
    if (pid > 0)
    {
        for(int i = 0;i<sorter_number;i++){
            close(pipe_parser_sorter[i][0]);
        }
    }
    else
    {
        FILE *sorter2merger_fp[sorter_number];
        int frequency = 1;
        int smallest_index;
        int time = 0;
        char sorter2merfer_arr[sorter_number][long_length + 2];
        char cur_smallest_word[long_length + 2];
        char print_wait_list[long_length+2];
        memset(print_wait_list, 0, strlen(print_wait_list));

        for(int i = 0;i<sorter_number;i++)
        {
            sorter2merger_fp[i] = fdopen(pipe_sorter_merger[i][0], "r");
            fgets(sorter2merfer_arr[i], long_length + 2, sorter2merger_fp[i]);
        }

        while (!empty_or_not(sorter_number, long_length, sorter2merfer_arr))
        {
            smallest_index = find_smallest_index(sorter_number, long_length, sorter2merfer_arr);
            strcpy(cur_smallest_word, sorter2merfer_arr[smallest_index]);

            if(!strcmp(cur_smallest_word, print_wait_list)) frequency++;
            else{
                if(time != 0){
                    printf("%-10d%s", frequency, print_wait_list);
                    fflush(stdout);
                    frequency = 1;
                }
            }
            strcpy(print_wait_list,cur_smallest_word);
            time ++;
            if (fgets(sorter2merfer_arr[smallest_index], long_length + 2, sorter2merger_fp[smallest_index]) == NULL)
            {
                strcpy(sorter2merfer_arr[smallest_index], "{{");
            }
        }
        printf("%-10d%s", frequency, print_wait_list);
        fflush(stdout);

        for (int i = 0; i < sorter_number; i++)
        {
            fclose(sorter2merger_fp[i]);
            close(pipe_sorter_merger[i][0]);
        }
    }
}
int main(int argc, char **argv)
{
    int sorter_number = -1;
    int long_length = -1;
    int short_length = -1;
    int arg;

    while ((arg = getopt (argc, argv, "n:s:l:")) != -1) {
        switch (arg) {
            case 'n':
                sorter_number = atoi(optarg);
                break;
            case 's':
                short_length = atoi(optarg);
                break;
            case 'l':
                long_length = atoi(optarg);
                break;
            case '?':
                printf("wrong parameters inputted\n");
                exit(1);
            default:
                printf("nothing happened\n");
        }
    }
    if(sorter_number == -1 || long_length == -1 || short_length == -1) return 1;
    int pipe_parser_sorter[sorter_number][2];
    int pipe_sorter_merger[sorter_number][2];
    for(int i = 0; i<sorter_number;i++){
        pipe(pipe_parser_sorter[i]);
        pipe(pipe_sorter_merger[i]);
        int p = fork();
        // child process
        if(!p){
            close(pipe_parser_sorter[i][1]);
            close(pipe_sorter_merger[i][0]);
            dup2(pipe_parser_sorter[i][0], STDIN_FILENO);
            dup2(pipe_sorter_merger[i][1], STDOUT_FILENO);
            for(int m=0;m<i;m++){
                close(pipe_parser_sorter[m][1]);
                close(pipe_sorter_merger[m][0]);
            }
            execlp("/usr/bin/sort", "sort", NULL);
        }
        else{
            close(pipe_parser_sorter[i][0]);
            close(pipe_sorter_merger[i][1]);
        }
    }



//    sorter(pipe_parser_sorter,pipe_sorter_merger,sorter_number);
    put_words(pipe_parser_sorter,pipe_sorter_merger,sorter_number,short_length,long_length);
    merger(pipe_parser_sorter,pipe_sorter_merger,sorter_number,long_length);
    for(int i = 0;i<sorter_number+1;i++){
        wait(NULL);
    }
    return 0;
}