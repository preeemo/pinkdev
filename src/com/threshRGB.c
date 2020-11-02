/* Call to ltresh */

#include <stdint.h>
#include <stdio.h>
#include <mcimage.h>
#include <lthresh.h>
#include <stdlib.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * imageR, * imageG, * imageB;
  uint32_t low, high;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in1.ppm, low threshold, high threshold, output image\n", argv[0]);
    exit(0);
  }

  if (readrgbimage(argv[1], &imageR, &imageG, &imageB) == NULL)
  {
    fprintf(stderr, "thresh: readimage failed\n");
    exit(0);
  }

  low = atoi(argv[2]);
  high = atoi(argv[3]);
  
  if (! lthresh(imageR, low, high)){
    fprintf(stderr, "thresh: function laddconst failed\n");
    exit(0);
  }

  if (! lthresh(imageG, low, high)) {
    fprintf(stderr, "thresh: function laddconst failed\n");
    exit(0);
  }

  if (! lthresh(imageB, low, high)) {
    fprintf(stderr, "thresh: function laddconst failed\n");
    exit(0);
  }


  writergbimage(imageR, imageG, imageB, argv[argc-1]);
  freeimage(imageR);
  freeimage(imageG);
  freeimage(imageB);

  return 0;
} /* main */


