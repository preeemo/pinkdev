/* Call to lhiste */

#include <stdint.h>
#include <stdio.h>
#include <mcimage.h>
#include <lhiste.h>
#include <stdlib.h>

/* =============================================================== */
int main(int argc, char **argv){
/* =============================================================== */
  struct xvimage * image1;
	
  if (argc != 3) {
    fprintf(stderr, "usage: %s image.pgm, output image\n", argv[0]);
    exit(0);
  }

  image1 = readimage(argv[1]);  
  if (image1 == NULL)
  {
    fprintf(stderr, "histe: readimage failed\n");
    exit(0);
  }
  
  if (! lhiste(image1)) {
    fprintf(stderr, "histe: function histe failed\n");
    exit(0);
  }
  
  writeimage(image1, argv[2]);
  freeimage(image1);

  return 0;
} /* main */


