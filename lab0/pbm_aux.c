// This code is my own work, it was written without 
// consulting a tutor or code written by other students-Shuang Wu
#include "pbm.h"
#include <stdlib.h>
#include <stdio.h>



PPMImage * new_ppmimage(unsigned int w, unsigned int h, unsigned int m)
{
    PPMImage* p;
    p = (PPMImage *)malloc(sizeof(PPMImage));
    p->width = w;
    p->height = h;
    p->max = m;
    for (int i = 0; i < 3; i++) {
        p->pixmap[i] = (unsigned int **)malloc(h * sizeof(unsigned int*));
        for (int j = 0; j < h; j++) {
            p->pixmap[i][j] = (unsigned int *)malloc(w * sizeof(unsigned int*));
        }
    }
    return p;
}



void del_ppmimage( PPMImage* p )
{
    for (int rgb = 0; rgb < 3; rgb++)
    {
        for (int h = 0; h < p->height; h++)
        {
            free(p->pixmap[rgb][h]);
        }
        free(p->pixmap[rgb]);
    }
    free(p);
}


PBMImage * new_pbmimage( unsigned int w, unsigned int h )
{
    PBMImage* p = (PBMImage *)malloc(sizeof(PBMImage));
    p->width = w;
    p->height = h;
    p->pixmap = (unsigned int **)malloc(h * sizeof(unsigned int*));
    for (int i = 0; i < h; i++) {
        p->pixmap[i] = (unsigned int *)malloc(w * sizeof(unsigned int*));
    }
    return p;
}

PGMImage * new_pgmimage( unsigned int w, unsigned int h, unsigned int m )
{   
    PGMImage* p = (PGMImage *)malloc(sizeof(PGMImage));
    p->width = w;
    p->height = h;
    p->max = m;
    p->pixmap = (unsigned int **)malloc(h * sizeof(unsigned int*));
    for (int j = 0; j < h; j++)
    {
        p->pixmap[j] = (unsigned int *) malloc(w * sizeof(unsigned int*));
    }
    return p;
}

void del_pgmimage( PGMImage* p )
{
    for (int h = 0; h < p->height; h++)
    {
        free(p->pixmap[h]);
    }
    free(p);
}

void del_pbmimage( PBMImage* p )
{
    for (int h = 0; h < p->height; h++)
    {
        free(p->pixmap[h]);
    }
    free(p);
}

