/* Call to lhiste */

#include <stdint.h>
#include <stdio.h>
#include <mcimage.h>
#include <lhiste.h>
#include <stdlib.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
printf("OK\n");
  struct xvimage * image1;
	
  if (argc != 3) {
    fprintf(stderr, "usage: %s in1.pgm, output image\n", argv[0]);
    exit(0);
  }
  printf("OK2\n");

  image1 = readimage(argv[1]);  
  if (image1 == NULL)
  {
    fprintf(stderr, "histe: readimage failed\n");
    exit(0);
  }
  printf("OK3\n");
  if (! lhiste(image1)) {
    fprintf(stderr, "histe: function histe failed\n");
    exit(0);
  }
  writeimage(image1, argv[2]);
  freeimage(image1);

  return 0;
} /* main */


