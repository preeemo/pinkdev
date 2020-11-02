/* Call to lhistn */

#include <stdint.h>
#include <stdio.h>
#include <mcimage.h>
#include <lhistn.h>
#include <stdlib.h>

/* =============================================================== */
int main(int argc, char **argv){
/* =============================================================== */
  struct xvimage * R, * G, * B;
  uint32_t low, high;

  if (argc != 5) {
    fprintf(stderr, "usage: %s image.ppm, low threshold, high threshold, output image\n", argv[0]);
    exit(0);
  }

  if (readrgbimage(argv[1], &R, &G, &B) == NULL) {
    fprintf(stderr, "histn: readimage failed\n");
    exit(0);
  }

  low = atoi(argv[2]);
  high = atoi(argv[3]);
  
  if (! lhistn(R, low, high)) {
    fprintf(stderr, "histn: function histn failed\n");
    exit(0);
  }

  if (! lhistn(G, low, high)) {
    fprintf(stderr, "histn: function histn failed\n");
    exit(0);
  }

  if (! lhistn(B, low, high)) {
    fprintf(stderr, "histn: function histn failed\n");
    exit(0);
  }

  writergbimage(R, G, B, argv[argc-1]);
  freeimage(R);
  freeimage(G);
  freeimage(B);

  return 0;
} /* main */


