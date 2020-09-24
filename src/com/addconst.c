/* Call to laddconst */

#include <stdint.h>
#include <stdio.h>
#include <mcimage.h>
#include <laddconst.h>
#include <stdlib.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image1;
  uint32_t constante;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in1.pgm constante out.pgm \n", argv[0]);
    exit(0);
  }

  image1 = readimage(argv[1]);  
  if (image1 == NULL)
  {
    fprintf(stderr, "addconst: readimage failed\n");
    exit(0);
  }
  constante = atoi(argv[2]);

  if (! laddconst(image1, constante))
  {
    fprintf(stderr, "addconst: function laddconst failed\n");
    exit(0);
  }

  writeimage(image1, argv[3]);
  freeimage(image1);

  return 0;
} /* main */


