/* Call to lhistn */

#include <stdint.h>
#include <stdio.h>
#include <mcimage.h>
#include <lhistn.h>
#include <stdlib.h>

/* =============================================================== */
int main(int argc, char **argv){
/* =============================================================== */
  struct xvimage * image1;
  uint32_t low, high;

  if (argc != 5) {
    fprintf(stderr, "usage: %s image.pgm, low threshold, high threshold, output image\n", argv[0]);
    exit(0);
  }

  image1 = readimage(argv[1]);

  if (image1 == NULL) {
    fprintf(stderr, "histn: readimage failed\n");
    exit(0);
  }

  low = atoi(argv[2]);
  high = atoi(argv[3]);
  
  if (!lhistn(image1, low, high)) {
    fprintf(stderr, "histn: function histn failed\n");
    exit(0);
  }

  writeimage(image1, argv[4]);
  freeimage(image1);

  return 0;
} /* main */


