/* Call to pixelwise */

#include <stdint.h>
#include <stdio.h>
#include <mcimage.h>
#include <lpixelwiseCGA_RGB.h>
#include <stdlib.h>

/* =============================================================== */
int main(int argc, char **argv){
/* =============================================================== */

  struct xvimage * imageR, * imageG, * imageB;
  
  if (argc != 3) {
    fprintf(stderr, "usage: %s\n\tin1.pgm,\n\tcolor: (1 for RGB, 0 for grayscale),\n\toutput image\n", argv[0]);
    exit(0);
  }

  if (readrgbimage(argv[1], &imageR, &imageG, &imageB) == 0) {
    fprintf(stderr, "pixelwiseCGA_RGB: readrgbimage failed\n");
    exit(1);
  }

  if (! lpixelwiseCGA_RGB(imageR, imageG, imageB))  {
    fprintf(stderr, "pixelwiseCGA_RGB: function lpixelwiseCGA_RGB failed\n");
    exit(0);
  }



  writergbimage(imageR, imageG, imageB, argv[argc-1]);
  freeimage(imageR);
  freeimage(imageG);
  freeimage(imageB);


  return 0;
} /* main */


