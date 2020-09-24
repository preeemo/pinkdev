#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <mcimage.h>
#include <lderiche.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image1;
  double alpha;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in1.pgm alpha out.pgm \n", argv[0]);
    exit(0);
  }

  image1 = readimage(argv[1]);  
  if (image1 == NULL)
  {
    fprintf(stderr, "deriche: readimage failed\n");
    exit(0);
  }
  alpha = atof(argv[2]);

  if (! lderiche(image1, alpha))
  {
    fprintf(stderr, "deriche: function lderiche failed\n");
    exit(0);
  }

  writeimage(image1, argv[3]);
  freeimage(image1);

  return 0;
} /* main */


