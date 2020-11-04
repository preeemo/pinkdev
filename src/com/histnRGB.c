/* Call to lhistn */

#include <stdint.h>
#include <stdio.h>
#include <mcimage.h>
#include <lhistn.h>
#include <stdlib.h>

/* =============================================================== */
int main(int argc, char **argv){
/* =============================================================== */
  struct xvimage * imageR, * imageG, * imageB;
  uint32_t low, high;

  if (argc != 5) {
    fprintf(stderr, "usage: %s image.ppm, low threshold, high threshold, output image\n", argv[0]);
    exit(0);
  }

  if (readrgbimage(argv[1], &imageR, &imageG, &imageB) == 0) {
    fprintf(stderr, "histn: readimage failed\n");
    exit(0);
  }

  low = atoi(argv[2]);
  high = atoi(argv[3]);
  
  if (! lhistn(imageR, low, high)) {
    fprintf(stderr, "histn: function histn failed\n");
    exit(0);
  }

  if (! lhistn(imageG, low, high)) {
    fprintf(stderr, "histn: function histn failed\n");
    exit(0);
  }

  if (! lhistn(imageB, low, high)) {
    fprintf(stderr, "histn: function histn failed\n");
    exit(0);
  }

  writergbimage(imageR, imageG, imageB, argv[argc-1]);
  freeimage(imageR);
  freeimage(imageG);
  freeimage(imageB);

  return 0;
} /* main */


