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
  struct xvimage * image1;
  uint32_t low, high;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in1.pgm, low threshold, high threshold, output image\n", argv[0]);
    exit(0);
  }

  image1 = readimage(argv[1]);  
  if (image1 == NULL)
  {
    fprintf(stderr, "thresh: readimage failed\n");
    exit(0);
  }
  low = atoi(argv[2]);
  high = atoi(argv[3]);
  if (! lthresh(image1, low, high))
  {
    fprintf(stderr, "thresh: function laddconst failed\n");
    exit(0);
  }

  writeimage(image1, argv[4]);
  freeimage(image1);

  return 0;
} /* main */


