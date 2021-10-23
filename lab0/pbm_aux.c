#include "pbm.h"

PPMImage * new_ppmimage( unsigned int w, unsigned int h, unsigned int m )
{
    //struct pointer initialization
    PPMImage* p;
    p = malloc(sizeof(PPMImage));
    p->width = w;
    p->height = h;
    p->max = m;
    for (int i = 0; i < 3; i++) {
        p->pixmap[i] = malloc(h * sizeof(unsigned int*));
        for (int j = 0; j < h; j++) {
            p->pixmap[i][j] = malloc(w * sizeof(unsigned int));
        }
    }
    return p;

}

PBMImage * new_pbmimage( unsigned int w, unsigned int h )
{
    PBMImage* p = malloc(sizeof(PBMImage));
    p->width = w;
    p->height = h;
    p->pixmap = malloc(h * sizeof(unsigned int*));
    for (int i = 0; i < h; i++) {
        p->pixmap[i] = malloc(w * sizeof(unsigned int));
    }
    return p;
}

PGMImage * new_pgmimage( unsigned int w, unsigned int h, unsigned int m )
{
    PGMImage* p = (PGMImage*)malloc(sizeof(PGMImage));
    p->width = w;
    p->height = h;
    p->max = m;

    p->pixmap = malloc(h * sizeof(unsigned int));
    for (int j = 0; j < h; j++)
    {
        p->pixmap[j] = malloc(w * sizeof(unsigned int));
    }
    return p;
}

void del_ppmimage( PPMImage * p )
{
    // pixel_map means [[RGB], [row], [column]], the position [row][column] is R or G or B
    for (int RGB = 0; RGB < 3; RGB++)
    {
        for (int h = 0; h < p->height; h++)
        {
            free(p->pixmap[RGB][h]);
        }
        free(p->pixmap[RGB]);
    }
    free(p->pixmap);
    free(p);
}

void del_pgmimage( PGMImage * p )
{
    for (int h = 0; h < p->height; h++)
    {
        free(p->pixmap[h]);
    }
    free(p->pixmap);
    free(p);
}

void del_pbmimage( PBMImage * p )
{
    for (int h = 0; h < p->height; h++)
    {
        free(p->pixmap[h]);
    }
    free(p->pixmap);
    free(p);
}

