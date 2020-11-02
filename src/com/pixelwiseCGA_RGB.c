/* Call to pixelwise */

#include <stdint.h>
#include <stdio.h>
#include <mcimage.h>
#include <lpixelwiseCGA.h>
#include <stdlib.h>

/* =============================================================== */
int main(int argc, char **argv){
/* =============================================================== */

  struct xvimage * imageR, * imageG, * imageB;
  
  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.ppm out.pgm\n", argv[0]);
    exit(1);
  }

  if (readrgbimage(argv[1], &imageR, &imageG, &imageB) == 0)
  {
    fprintf(stderr, "rgb2bw: readrgbimage failed\n");
    exit(1);
  }

  if (! lpixelwiseCGA(imageR))  {
    fprintf(stderr, "pixelwiseCGA_RGB: function pixelwiseCGA_RGB failed\n");
    exit(0);
  }

  if (! lpixelwiseCGA(imageG))  {
    fprintf(stderr, "pixelwiseCGA_RGB: function pixelwiseCGA_RGB failed\n");
    exit(0);
  }
  
  if (! lpixelwiseCGA(imageB))  {
    fprintf(stderr, "pixelwiseCGA_RGB: function pixelwiseCGA_RGB failed\n");
    exit(0);
  }


  writergbimage(imageR, imageG, imageB, argv[argc-1]);
  freeimage(imageR);
  freeimage(imageG);
  freeimage(imageB);


  return 0;
} /* main */


