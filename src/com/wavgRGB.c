/* Call to wavg */

#include <stdint.h>
#include <stdio.h>
#include <mcimage.h>
#include <lwavg.h>
#include <stdlib.h>

/* =============================================================== */
int main(int argc, char **argv){
/* =============================================================== */

  struct xvimage * imageR, * imageG, * imageB;
  uint32_t kernel = 0;
  
  if (argc != 4) {
    fprintf(stderr, "usage: %s in.ppm,\n k: kernel type ('0': unitary kernel - simple average; '1': gaussian kernel),\n out.pgm\n", argv[0]);
    exit(1);
  }

  if (readrgbimage(argv[1], &imageR, &imageG, &imageB) == 0) {
    fprintf(stderr, "wavgRGB: readrgbimage failed\n");
    exit(1);
  }

  kernel = atoi(argv[2]);

  if (! lwavg(imageR, kernel)) {
    fprintf(stderr, "wavgRGB: function wavgRGB failed\n");
    exit(0);
  }

  if (! lwavg(imageG, kernel)) {
    fprintf(stderr, "wavgRGB: function wavgRGB failed\n");
    exit(0);
  }
  
  if (! lwavg(imageB, kernel)) {
    fprintf(stderr, "wavgRGB: function wavgRGB failed\n");
    exit(0);
  }


  writergbimage(imageR, imageG, imageB, argv[argc-1]);
  freeimage(imageR);
  freeimage(imageG);
  freeimage(imageB);


  return 0;
} /* main */


