// This code is my own work, it was written without 
// consulting a tutor or code written by other students-Shuang Wu
#include "pbm.h"
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void sepia(PPMImage* ppmimg)
{  
    for (int h = 0; h < ppmimg->height; h++)
    {
        for (int w = 0; w < ppmimg->width; w++)
        {
        
	    unsigned int R;
            R = 0.393 * ppmimg->pixmap[0][h][w]+0.769 * ppmimg->pixmap[1][h][w]+0.189 * ppmimg->pixmap[2][h][w];
            if (R >= ppmimg->max)
            {
                ppmimg->pixmap[0][h][w] = ppmimg->max;
            }
            else
            {
                ppmimg->pixmap[0][h][w] = R;
            }
	    unsigned int G;
            G = 0.349 * ppmimg->pixmap[0][h][w]+0.686 * ppmimg->pixmap[1][h][w]+0.168 * ppmimg->pixmap[2][h][w];
            if (G >= ppmimg->max)
            {
                ppmimg->pixmap[1][h][w] = ppmimg->max;
            }
            else
            {
                ppmimg->pixmap[0][h][w] = G;
            }
	    unsigned int B;
            B = 0.272 * ppmimg->pixmap[0][h][w]+0.534 * ppmimg->pixmap[1][h][w]+0.131 * ppmimg->pixmap[2][h][w];
            if (B >= ppmimg->max)
            {
                ppmimg->pixmap[2][h][w] = ppmimg->max;
            }
            else
            {
                ppmimg->pixmap[2][h][w] = B;
            }
        }
    }
    
}


void ppm2pbm(PPMImage* ppmimg, PBMImage* pbmimg, const char* outputfName)
{
    for (int h = 0; h < ppmimg->height; h++)
    {
        for (int w = 0; w < ppmimg->width; w++)
        {
            if ((ppmimg->pixmap[0][h][w]+ ppmimg->pixmap[1][h][w]+ ppmimg->pixmap[2][h][w])/3.0 < (ppmimg->max / 2.0))
            {
                pbmimg->pixmap[h][w] = 1;
            }
            else
            {
                pbmimg->pixmap[h][w] = 0;
            }
        }
    }
    write_pbmfile(pbmimg, outputfName);
    del_ppmimage(ppmimg);
    del_pbmimage(pbmimg);
}


void ppm2pgm(PPMImage* ppmimg, PGMImage* pgmimg, int pixelmax, const char* outputfName)
{	
    
    // double will ensure the precision
    double mean;
    double newhw;
    for (int h = 0; h < ppmimg->height; h++)
    {
        for (int w = 0; w < ppmimg->width; w++)
        {
            mean = (ppmimg->pixmap[0][h][w] + ppmimg->pixmap[1][h][w] + ppmimg->pixmap[2][h][w]) / 3.0;
            newhw =  mean * ((double)(pixelmax) / (double)(ppmimg->max));
            pgmimg->pixmap[h][w] = newhw;
        }
    }
	write_pgmfile(pgmimg, outputfName);
    del_ppmimage(ppmimg);
    del_pgmimage(pgmimg);
}


void isolate(PPMImage* ppmimg, int channel)
{
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
            else if (channel == 2)
            {
                ppmimg->pixmap[0][h][w] = 0;
                ppmimg->pixmap[1][h][w] = 0;

            }

        }
    }
}


void remove_channel(PPMImage* ppmimg, int channel)
{
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
}


void mirror(PPMImage* ppmimg)
{     
      for (int rgb=0; rgb<3; rgb++){
            for (int h=0; h<ppmimg->height; h++) {
                for (int w=0; w<ppmimg->width/2; w++) {
                    ppmimg->pixmap[rgb][h][ppmimg->width-w-1] = ppmimg->pixmap[rgb][h][w];
            }
        }
    }
}


void thumbnail(PPMImage* ppmimg, int scale, const char* outputfName)
{


    unsigned int width_1;
    unsigned int height_1;
    
    if (ppmimg->width % scale != 0) 
    {
        int temp_1 = ppmimg->width/scale;
        width_1 = temp_1 + 1;
    }
    else
    {  
        width_1 = ppmimg->width/scale;
    }
    if (ppmimg->height % scale != 0)
    {
        int temp_2 = ppmimg->height/scale;
        height_1 = temp_2 + 1;
    }
    else
    {
        height_1 = ppmimg->height/scale;
    }
    
    PPMImage * ppmimg1 = new_ppmimage(width_1, height_1, ppmimg->max);
    for(int rgb = 0;rgb<3;rgb++)
    {
    for (int h=0; h<ppmimg1->height; h++) {
        for (int w=0; w<ppmimg1->width; w++) {
            ppmimg1->pixmap[rgb][h][w] = ppmimg->pixmap[rgb][h*scale][w*scale];
        }
    }
    }
    write_ppmfile(ppmimg1, outputfName);
    del_ppmimage(ppmimg1);
    del_ppmimage(ppmimg);


}


void nup(PPMImage* ppmimg, int scale, const char* outputfName)
{
    // PPMImage* p = new_ppmimg(ppmimg->width, ppmimg->height, ppmimg->max);
    

    unsigned int width_1;
    unsigned int height_1;
    if (ppmimg->width % scale != 0)  
    {
        int temp_1 = ppmimg->width/scale;
        width_1 = temp_1 + 1;
    }
    else
    {
        width_1 = ppmimg->width / scale;
    }
    if (ppmimg->height % scale != 0)
    { 
        int temp_2 = ppmimg->height/scale;
        height_1 = temp_2 + 1;
    }
    else
    {
        height_1 = ppmimg->height / scale;
    }
    

    for (int rgb=0; rgb<3; rgb++)
    {
        for (int h = 0; h < height_1; h++)
          {
            for (int w = 0; w < width_1; w++)
            {
                ppmimg->pixmap[rgb][h][w] = ppmimg->pixmap[rgb][h*scale][w*scale];
            }
          }
    }
    
    for (int rgb=0; rgb<3; rgb++)
    {
    for (int h=0; h<ppmimg->height; h++) {
        for (int w=0; w<ppmimg->width; w++) {
                ppmimg->pixmap[rgb][h][w] = ppmimg->pixmap[rgb][h%height_1][w%width_1];   
        }
     }
    }
    write_ppmfile(ppmimg, outputfName);
    del_ppmimage(ppmimg);
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
		if(flag==0){
			printf("Error: Multiple transformations specified\n");
			exit(1);
		}
                option_argv = "b";
                inputfName = argv[argc - 1];
		flag=0;
                break;
                
            case 'g':
		if(flag==0){
			printf("Error: Multiple transformations specified\n");
			exit(1);
		}
		char* ptr_g;
		third_arg = strtol(optarg, &ptr_g, 10);
                if (third_arg > 65535 || third_arg < 1)
                {
                    printf("Error: Invalid max grayscale pixel value: %s; must be less than 65,536\n", optarg);
                    exit(1);
                }
                option_argv = "g";
                inputfName = argv[argc - 1];
   
		flag=0;
                break;

            case 'i':
		if(flag==0){
			printf("Error: Multiple transformations specified\n");
			exit(1);

		}
               if (strcmp(optarg, "red") == 0 || strcmp(optarg, "green") == 0 || strcmp(optarg, "blue") == 0){
				option_argv = "i";
				inputfName = argv[argc - 1];
				third_arg = 0;
				if (strcmp(optarg, "green") == 0)
				{
				    third_arg = 1;
				}
				else if(strcmp(optarg, "blue") == 0)
				{
				    third_arg = 2;
				}

				flag=0;
				break;
			}
			else
			{
			    printf("Error: Invalid channel specification: (%s); should be 'red', 'green' or 'blue'\n", optarg);
			    exit(1);
			}


            case 'r':
		if(flag==0){
			printf("Error: Multiple transformations specified\n");
			exit(1);
		}

               if (strcmp(optarg, "red") == 0 || strcmp(optarg, "green") == 0 || strcmp(optarg, "blue") == 0){
			option_argv = "r";
		        inputfName = argv[argc - 1];

	  		third_arg = 0;
		        if (strcmp(optarg, "green") == 0)
		        {
		            third_arg = 1;
		        }
		        else if (strcmp(optarg, "blue") == 0)
		        {
		            third_arg = 2;
		        }
			flag=0;
		        break;
		}
		else
		{
		    printf("Error: Invalid channel specification: (%s); should be 'red', 'green' or 'blue'\n", optarg);
		    exit(1);
		}

            case 's':
		if(flag==0)
		{
			printf("Error: Multiple transformations specified\n");
			exit(1);
		}
                option_argv = "s";
                inputfName = argv[argc - 1];
		flag=0;
                break;

            case 'm':
		if(flag==0)
		{
			printf("Error: Multiple transformations specified\n");
			exit(1);
		}
                option_argv = "m";
                inputfName = argv[argc - 1];
		flag=0;
                break;

            case 't':
		if(flag==0)
    {
			printf("Error: Multiple transformations specified\n");
			exit(1);
		}
                option_argv = "t";
                inputfName = argv[argc - 1];
                char* ptr_t;
                third_arg = strtol(optarg, &ptr_t, 10);
                if (third_arg > 8 || third_arg < 1)
                {
                    printf("Error: Invalid scale factor: %s; must be 1-8\n", optarg);
                    exit(1);
                }
		            flag=0;
                break;

            case 'n':
		if(flag==0){
			printf("Error: Multiple transformations specified\n");
			exit(1);
		}
                option_argv = "n";
                inputfName = argv[argc - 1];
                char* ptr_n;
                third_arg = strtol(optarg, &ptr_n, 10);
                if (third_arg > 8 || third_arg < 1)
                {
                    printf("Error: Invalid scale factor: %s; must be 1-8\n", optarg);
                    exit(1);
                }
		            flag=0;
                break;
            case 'o':
                outputfName = optarg;
                if (outputfName == NULL)
                {
                    printf("Error: No output file specified\n");
                    exit(1);
                }
                if(argc==4) option_argv = "b";
                inputfName = argv[argc - 1];

                break;
            // option not in optstring
            case '?':
		printf("Usage: ppmcvt [-bgirsmtno] [FILE]\n");
                exit(1);
                break;
        }
    }

    if (strcmp(option_argv, "b") == 0)
    {

        PPMImage* ppmimg = read_ppmfile(inputfName);

        PBMImage* pbmimg = new_pbmimage(ppmimg->width, ppmimg->height);

        ppm2pbm(ppmimg, pbmimg, outputfName);

	      return 0;
    }
    if (strcmp(option_argv, "g") == 0)
    {
        PPMImage* ppmimg = read_ppmfile(inputfName);
        PGMImage* pgmimg = new_pgmimage(ppmimg->width, ppmimg->height, ppmimg->max);
        ppm2pgm(ppmimg, pgmimg, third_arg, outputfName);
        return 0;
  
  
  
    }
    if (strcmp(option_argv, "i") == 0)
    {
        PPMImage* ppmimg = read_ppmfile(inputfName);
        isolate(ppmimg, third_arg);
        
        write_ppmfile(ppmimg, outputfName);

        del_ppmimage(ppmimg);
	      return 0;
    }
    if (strcmp(option_argv, "r") == 0)
    {
        PPMImage* ppmimg = read_ppmfile(inputfName);
        remove_channel(ppmimg, third_arg);
        
        write_ppmfile(ppmimg, outputfName);
        del_ppmimage(ppmimg);
        
	      return 0;
    }
    if (strcmp(option_argv, "s") == 0)
    {
        PPMImage* ppmimg = read_ppmfile(inputfName);
        sepia(ppmimg);
        write_ppmfile(ppmimg, outputfName);
        del_ppmimage(ppmimg);
        
	      return 0;
    }
    if (strcmp(option_argv, "m") == 0)
    {    
        PPMImage* ppmimg = read_ppmfile(inputfName);
        mirror(ppmimg);
        
        write_ppmfile(ppmimg, outputfName);
        del_ppmimage(ppmimg);
	      return 0;
    }
    if (strcmp(option_argv, "t") == 0)
    {   
        PPMImage* ppmimg = read_ppmfile(inputfName);	
        thumbnail(ppmimg, third_arg, outputfName);
	      return 0;
    }
    if (strcmp(option_argv, "n") == 0)
    {
        PPMImage* ppmimg = read_ppmfile(inputfName);
        nup(ppmimg, third_arg, outputfName);
        return 0;
    }
    return 0;
}



