#include "pbm.h"

#include <stdlib.h>
#include <stdio.h>



void sepia(const char* inputfname, const char* outputfname)
{
    read_ppmfile(inputfname);
    PPMImage* ppmsepia;
    ppmsepia = read_ppmfile(inputfname);
    unsigned int oldR, oldG, oldB;
    unsigned int newR, newG, newB;
    for (int h = 0; h < ppmsepia->height; h++)
    {
        for (int w = 0; w < ppmsepia->width; w++)
        {
            oldR = ppmsepia->pixmap[0][h][w];
            oldG = ppmsepia->pixmap[1][h][w];
            oldB = ppmsepia->pixmap[2][h][w];

            newR = 0.393 * (oldR)+0.769 * (oldG)+0.189 * (oldB);
            if (newR >= ppmsepia->max)
            {
                ppmsepia->pixmap[0][h][w] = ppmsepia->max;
            }
            else
            {
                ppmsepia->pixmap[0][h][w] = newR;
            }
            newG = 0.349 * (oldR)+0.686 * (oldG)+0.168 * (oldB);
            if (newG >= ppmsepia->max)
            {
                ppmsepia->pixmap[1][h][w] = ppmsepia->max;
            }
            else
            {
                ppmsepia->pixmap[0][h][w] = newR;
            }
            newB = 0.272 * (oldR)+0.534 * (oldG)+0.131 * (oldB);
            if (newB >= ppmsepia->max)
            {
                ppmsepia->pixmap[2][h][w] = ppmsepia->max;
            }
            else
            {
                ppmsepia->pixmap[2][h][w] = newB;
            }
        }
    }
    del_ppmimage(ppmsepia);
}

//Function to convert the ppm file to pbm file
void ppmtopbm(const char* inputfname, const char* outputfname)
{
    PPMImage* ppmimg;
    ppmimg = read_ppmfile(inputfname);
    PBMImage* pbmimg = new_pbmimage(ppmimg->width, ppmimg->height);

    for (int h = 0; h < ppmimg->height; h++)
    {
        for (int w = 0; w < ppmimg->width; w++)
        {
            unsigned int pR = ppmimg->pixmap[0][h][w];
            unsigned int pG = ppmimg->pixmap[1][h][w];
            unsigned int pB = ppmimg->pixmap[2][h][w];

            unsigned int pixel = (pR + pG + pB) / 3.0;

            if (pixel < (ppmimg->max / 2.0))
            {
                pbmimg->pixmap[h][w] = 1;
            }
            else
            {
                pbmimg->pixmap[h][w] = 0;
            }
        }
    }

    write_pbmfile(pbmimg, outputfname);

    del_ppmimage(ppmimg);
    del_pbmimage(pbmimg);
}

//Function to convert the ppm file to pgm file
void ppmtopgm(const char* inputfname, const char* outputfname, int pgmmax)
{
    PPMImage* ppmimg;
    ppmimg = read_ppmfile(inputfname);
    PGMImage* pgmimg = new_pgmimage(ppmimg->width, ppmimg->height, ppmimg->max);

    for (int h = 0; h < ppmimg->height; h++)
    {
        for (int w = 0; w < ppmimg->width; w++)
        {
            unsigned int pR = ppmimg->pixmap[0][h][w];
            unsigned int pG = ppmimg->pixmap[1][h][w];
            unsigned int pB = ppmimg->pixmap[2][h][w];

            pgmimg->pixmap[h][w] = ((pR + pG + pB) / ppmimg->max) * pgmmax;
        }
    }

    write_pgmfile(pgmimg, outputfname);

    del_ppmimage(ppmimg);
    del_pgmimage(pgmimg);

}

//Function to isolate a channel.
void isolateChannel(const char* inputfname, const char* outputfname, int channel)
{
    PPMImage* ppmimg;
    ppmimg = read_ppmfile(inputfname);

    for (int h = 0; h < ppmimg->height; h++)
    {
        for (int w = 0; w < ppmimg->width; w++)
        {
            if (channel == 0)
            {
                ppmimg->pixmap[1][h][w] = 0;
                ppmimg->pixmap[2][h][w] = 0;

            }
            else if (channel == 1)
            {
                ppmimg->pixmap[0][h][w] = 0;
                ppmimg->pixmap[2][h][w] = 0;

            }
            else
            {
                ppmimg->pixmap[0][h][w] = 0;
                ppmimg->pixmap[1][h][w] = 0;

            }

        }
    }

    write_ppmfile(ppmimg, outputfname);

    del_ppmimage(ppmimg);
}

//Function to remove a channel.
void removeChannel(const char* inputfname, const char* outputfname, int channel)
{
    PPMImage* ppmimg;
    ppmimg = read_ppmfile(inputfname);

    for (int h = 0; h < ppmimg->height; h++)
    {
        for (int w = 0; w < ppmimg->width; w++)
        {
            if (channel == 0)
            {
                ppmimg->pixmap[0][h][w] = 0;

            }
            else if (channel == 1)
            {
                ppmimg->pixmap[1][h][w] = 0;

            }
            else
            {
                ppmimg->pixmap[2][h][w] = 0;

            }

        }
    }

    write_ppmfile(ppmimg, outputfname);

    del_ppmimage(ppmimg);
}

//Mirroring an image.
void mirrorVertical(const char* inputfname, const char* outputfname)
{
    PPMImage* ppmimg;
    ppmimg = read_ppmfile(inputfname);
    int imgW, imgH, midVal;
    imgW = ppmimg->width, imgH = ppmimg->height;
    midVal = imgW / 2;

    PPMImage* new_ppmimg = new_ppmimage(imgW, imgH, ppmimg->max);

    for (int h = 0; h < imgH; ++h)
    {
        for (int w = 0; w < imgW; ++w)
        {
            if (w <= midVal)
            {
                new_ppmimg->pixmap[0][h][imgW - w] = ppmimg->pixmap[0][h][w];
                new_ppmimg->pixmap[1][h][imgW - w] = ppmimg->pixmap[1][h][w];
                new_ppmimg->pixmap[2][h][imgW - w] = ppmimg->pixmap[2][h][w];
            }
            else
            {
                new_ppmimg->pixmap[0][h][imgW - w] = ppmimg->pixmap[0][h][w];
                new_ppmimg->pixmap[1][h][imgW - w] = ppmimg->pixmap[1][h][w];
                new_ppmimg->pixmap[2][h][imgW - w] = ppmimg->pixmap[2][h][w];

            }
        }
    }
    write_ppmfile(new_ppmimg, outputfname);

    del_ppmimage(ppmimg);
    del_ppmimage(new_ppmimg);

}

//Thumbnail an image.
void thumbnail(const char* inputfname, const char* outputfname, int scale)
{
    PPMImage* ppmimg;
    ppmimg = read_ppmfile(inputfname);
    int imgW, imgH, midVal;
    imgW = ppmimg->width / scale, imgH = ppmimg->height / scale;

    PPMImage* new_ppmimg = new_ppmimage(imgW, imgH, ppmimg->max);

    for (int h = 0; h < imgH; ++h)
    {
        for (int w = 0; w < imgW; ++w)
        {
            new_ppmimg->pixmap[0][h][w] = ppmimg->pixmap[0][h * scale][w * scale];
            new_ppmimg->pixmap[1][h][w] = ppmimg->pixmap[1][h * scale][w * scale];
            new_ppmimg->pixmap[2][h][w] = ppmimg->pixmap[2][h * scale][w * scale];
        }
    }
    write_ppmfile(new_ppmimg, outputfname);

    del_ppmimage(ppmimg);
    del_ppmimage(new_ppmimg);

}

//Tiling a Thumbnail an image.
void tileThumbnail(const char* inputfname, const char* outputfname, int scale)
{
    PPMImage* ppmimg;
    ppmimg = read_ppmfile(inputfname);
    int imgW, imgH, midVal;
    PPMImage* thumbppmimg = new_ppmimage(ppmimg->width, ppmimg->height, ppmimg->max);

    imgW = ppmimg->width / scale, imgH = ppmimg->height / scale;

    PPMImage* new_ppmimg = new_ppmimage(imgW, imgH, ppmimg->max);

    for (int h = 0; h < imgH; ++h)
    {
        for (int w = 0; w < imgW; ++w)
        {
            new_ppmimg->pixmap[0][h][w] = ppmimg->pixmap[0][h * scale][w * scale];
            new_ppmimg->pixmap[1][h][w] = ppmimg->pixmap[1][h * scale][w * scale];
            new_ppmimg->pixmap[2][h][w] = ppmimg->pixmap[2][h * scale][w * scale];
        }
    }

    for (int h_h = 0; h_h < scale; h_h++)
    {
        for (int w_w = 0; w_w < scale; w_w++)
        {
            for (int h = 0; h < imgH; ++h)
            {
                for (int w = 0; w < imgW; ++w)
                {
                    thumbppmimg->pixmap[0][(h_h * imgH) + h][(w_w * imgW) + w] = new_ppmimg->pixmap[0][h][w];
                    thumbppmimg->pixmap[1][(h_h * imgH) + h][(w_w * imgW) + w] = new_ppmimg->pixmap[1][h][w];
                    thumbppmimg->pixmap[2][(h_h * imgH) + h][(w_w * imgW) + w] = new_ppmimg->pixmap[2][h][w];
                }
            }
        }
    }

    write_ppmfile(thumbppmimg, outputfname);

    del_ppmimage(ppmimg);
    del_ppmimage(new_ppmimg);
    del_ppmimage(thumbppmimg);
}


int main( int argc, char *argv[] )
{
    extern char* optarg;

 
    char* option_argv;
    char* inputfName;
    char* outputfName;
    int third_arg;
    int flag = 1;
    int o;
    const char* optstring = "bg:i:r:smt:n:o:";
    // when all operations are done, getopt() return -1
    while (-1 != (o = getopt(argc, argv, optstring))) {
        switch (o) 
        {
            case 'b':
                if (flag==0) {
                    printf("Reconversion error");
                    exit(1);
                }

                option_argv = "PBM";
                inputfName = argv[argc - 1];
                flag = 0;
                break;
                
            case 'g':
                if (flag==0) {
                    printf("Reconversion error");
                    exit(1);
                }
                if (third_arg >= 65536)
                {
                    printf("Error: Invalid max grayscale pixel value: %s; must be less than 65,536\n", optarg);
                    exit(-1);
                }
                option_argv = "PGM";
                inputfName = argv[argc - 1];
                char* ptr;
                third_arg = strtol(optarg, &ptr, 10);
                flag = 0;
                break;

            case 'i':
                if (flag==0) {
                    printf("Reconversion error");
                    exit(1);
                }
                option_argv = "isolate";
                inputfName = argv[argc - 1];
                if (strcmp(optarg, "red") == 0)
                {
                    third_arg = 0;
                }
                else if (strcmp(optarg, "green") == 0)
                {
                    third_arg = 1;
                }
                else if (strcmp(optarg, "blue") == 0)
                {
                    third_arg = 2;
                }
                else
                {
                    printf("Error: colorname\n");
                    exit(-1);
                }
                flag = 0;
                break;
            case 'r':
                if (flag==0) {
                    printf("Reconversion error");
                    exit(1);
                }
                option_argv = "remove";
                inputfName = argv[argc - 1];
                if (strcmp(optarg, "red") == 0)
                {
                    third_arg = 0;
                }
                else if (strcmp(optarg, "green") == 0)
                {
                    third_arg = 1;
                }
                else if (strcmp(optarg, "blue") == 0)
                {
                    third_arg = 2;
                }
                else
                {
                    printf("Error:colorname\n");
                    exit(-1);
                }
                flag = 0;
                break;

            case 's':
                if (flag==0) {
                    printf("Reconversion error");
                    exit(1);
                }
                option_argv = "sepia";
                inputfName = argv[argc - 1];
                flag = 0;
                break;

            case 'm':
                if (flag==0) {
                    printf("Reconversion error");
                    exit(1);
                }
                option_argv = "mirror";
                inputfName = argv[argc - 1];
                flag = 0;
                break;

            case 't':
                if (flag==0) {
                    printf("Reconversion error");
                    exit(1);
                }
                option_argv = "thumbnail";
                inputfName = argv[argc - 1];
                char* ptr1;
                third_arg = strtol(optarg, &ptr1, 10);
                if (third_arg > 8 || third_arg < 1)
                {
                    printf("Error: Invalid scale factor: %s; must be 1-8\n", optarg);
                    exit(-1);
                }
                flag = 0;
                break;

            case 'n':
                if (flag==0) {
                    printf("Reconversion error");
                    exit(1);
                }
                option_argv = "tile";
                inputfName = argv[argc - 1];
                char* ptr2;
                third_arg = strtol(optarg, &ptr2, 10);
                if (third_arg > 8 || third_arg < 1)
                {
                    printf("Error: Invalid scale factor: %s; must be 1-8\n", optarg);
                    exit(-1);
                }
                flag = 0;
                break;
            case 'o':      
                if (argc <= 4)
            {
                option_argv = "PBM";
                inputfName = argv[argc - 1];
                outputfName = optarg;
            }
                else
            {
                    if (optarg == NULL) {
                        printf("Error: No output file\n");
                        exit(1);
                }
                    else{ 
                            outputfName = optarg; 
                        }
            }
                break;
            // option not in optstring
            case '?':
                printf("Invalid optargv!");
                exit(1);
                break;
        }
    }





    if (strcmp(option_argv, "PBM") == 0)
    {
        PPMImage* ppm;
        ppm = read_ppmfile(inputfName);


        ppmtopbm(inputfName, outputfName);
    }
    else if (strcmp(option_argv, "PGM") == 0)
    {
        ppmtopgm(inputfName, outputfName, third_arg);
    }
    else if (strcmp(option_argv, "isolate") == 0)
    {
        isolateChannel(inputfName, outputfName, third_arg);
    }
    else if (strcmp(option_argv, "remove") == 0)
    {
        removeChannel(inputfName, outputfName, third_arg);
    }
    else if (strcmp(option_argv, "sepia") == 0)
    {
        sepia(inputfName, outputfName);
    }
    else if (strcmp(option_argv, "mirror") == 0)
    {
        mirrorVertical(inputfName, outputfName);
    }
    else if (strcmp(option_argv, "thumbnail") == 0)
    {
        thumbnail(inputfName, outputfName, third_arg);
    }
    else if (strcmp(option_argv, "tile") == 0)
    {
        tileThumbnail(inputfName, outputfName, third_arg);
    }
    return 0;
}



