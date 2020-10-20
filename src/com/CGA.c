/* Call to CGA */

#include <stdint.h>
#include <stdio.h>
#include <mcimage.h>
#include <lCGA.h>
#include <stdlib.h>

/* =============================================================== */
int main(int argc, char **argv){
/* =============================================================== */

  struct xvimage * image1;
  

  if (argc != 3)  {
    fprintf(stderr, "usage: %s in1.pgm,  output image\n", argv[0]);
    exit(0);
  }

  image1 = readimage(argv[1]);  

  if (image1 == NULL)  {
    fprintf(stderr, "CGA: readimage failed\n");
    exit(0);
  }
  if (! lCGA(image1))  {
    fprintf(stderr, "CGA: function CGA failed\n");
    exit(0);
  }

  writeimage(image1, argv[2]);
  freeimage(image1);

  return 0;
} /* main */

