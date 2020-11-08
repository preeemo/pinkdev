/* Call to pixelwise CGA */

#include <stdint.h>
#include <stdio.h>
#include <mcimage.h>
#include <lpixelwiseCGA.h>
#include <stdlib.h>

/* =============================================================== */
int main(int argc, char **argv){
/* =============================================================== */

  struct xvimage * image1;

  if (argc != 3)  {
    fprintf(stderr, "usage: %s\n\tin1.pgm,\n\toutput image\n", argv[0]);
    exit(0);
  }

  image1 = readimage(argv[1]);

  if (image1 == NULL)  {
    fprintf(stderr, "pixelwiseCGA: readimage failed\n");
    exit(0);
  }
  if (! lpixelwiseCGA(image1))  {
    fprintf(stderr, "pixelwiseCGA: function pixelwiseCGA failed\n");
    exit(0);
  }

  writeimage(image1, argv[argc-1]);
  freeimage(image1);

  return 0;
} /* main */

