/* Call to limgdiff */

#include <stdint.h>
#include <stdio.h>
#include <mcimage.h>
#include <limgdiff.h>
#include <stdlib.h>

/* =============================================================== */
int main(int argc, char **argv){
/* =============================================================== */

  struct xvimage * image1;
  struct xvimage * image2;


  if (argc != 4) {
    fprintf(stderr, "usage: %s in1.pgm, in2.pmg, out.pgm \n", argv[0]);
    exit(0);
  }

  image1 = readimage(argv[1]);  
  if (image1 == NULL)
  {
    fprintf(stderr, "imgdiff: readimage failed\n");
    exit(0);
  }
  image2 = readimage(argv[2]);  
  if (image2 == NULL)
  {
    fprintf(stderr, "imgdiff: readimage failed\n");
    exit(0);
  }

  if (! limgdiff(image1, image2))
  {
    fprintf(stderr, "imgdiff: function imgdiff failed\n");
    exit(0);
  }

  writeimage(image1, argv[3]);
  freeimage(image1);

  return 0;
} /* main */


