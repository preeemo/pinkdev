/* Call to limgdiff */

#include <stdint.h>
#include <stdio.h>
#include <mcimage.h>
#include <limgdiff.h>
#include <stdlib.h>

/* =============================================================== */
int main(int argc, char **argv){
/* =============================================================== */


  struct xvimage * imageR1, * imageG1, * imageB1;
  struct xvimage * imageR2, * imageG2, * imageB2;


  if (argc != 4) {
    fprintf(stderr, "usage: %s in1.ppm, in2.ppg, output image \n", argv[0]);
    exit(0);
  }

  if (readrgbimage(argv[1], &imageR1, &imageG1, &imageB1) == NULL) {
    fprintf(stderr, "imgdiff: readrgbimage failed\n");
    exit(0);
  }

  if (readrgbimage(argv[2], &imageR2, &imageG2, &imageB2) == NULL) {
    fprintf(stderr, "imgdiff: readrgbimage failed\n");
    exit(0);
  }

  if (! limgdiff(imageR1, imageR2)) {
    fprintf(stderr, "imgdiff: function imgdiff failed\n");
    exit(0);
  }

  if (! limgdiff(imageG1, imageG2)) {
    fprintf(stderr, "imgdiff: function imgdiff failed\n");
    exit(0);
  }

  if (! limgdiff(imageB1, imageB2)) {
    fprintf(stderr, "imgdiff: function imgdiff failed\n");
    exit(0);
  }

  writergbimage(imageR1, imageG1, imageB1, argv[argc-1]);
  freeimage(imageR1);
  freeimage(imageG1);
  freeimage(imageB1);

  return 0;
} /* main */


