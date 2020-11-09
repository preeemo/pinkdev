EXE=$(BDIR)/addconst $(BDIR)/deriche $(BDIR)/ppm2pgm $(BDIR)/pgm2ppm $(BDIR)/imgdiff $(BDIR)/imgdiffRGB $(BDIR)/thresh $(BDIR)/threshRGB $(BDIR)/histn $(BDIR)/histe $(BDIR)/histeRGB $(BDIR)/histnRGB $(BDIR)/wavg $(BDIR)/wavgRGB $(BDIR)/pixelwiseCGA $(BDIR)/pixelwiseCGA_RGB $(BDIR)/CGA $(BDIR)/CGA_RGB $(BDIR)/PSNRestimator $(BDIR)/PSNRestimatorRGB 

DOC=$(DOCDIR)/tutorialEnglish.html

CCFLAGS = -g -DUNIXIO -Wall

IDIR = ./include
ODIR = ./obj
BDIR = ./bin
CDIR = ./src/com
LDIR = ./src/lib
DOCDIR = ./doc
CC = cc

# Documentation, if not already comiled, can be compiled thanks to 'make doc'
# You can also do a 'make clean' if you want

all:	$(EXE)
doc:    $(DOC)

# ===============================================================
# shell commands
# ===============================================================

$(BDIR)/addconst:	$(CDIR)/addconst.c $(IDIR)/laddconst.h $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(ODIR)/laddconst.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/addconst.c $(ODIR)/laddconst.o $(ODIR)/mcimage.o -o $(BDIR)/addconst

$(BDIR)/imgdiff:	$(CDIR)/imgdiff.c $(IDIR)/limgdiff.h $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(ODIR)/limgdiff.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/imgdiff.c $(ODIR)/limgdiff.o $(ODIR)/mcimage.o -o $(BDIR)/imgdiff

$(BDIR)/imgdiffRGB:	$(CDIR)/imgdiffRGB.c $(IDIR)/limgdiff.h $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(ODIR)/limgdiff.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/imgdiffRGB.c $(ODIR)/limgdiff.o $(ODIR)/mcimage.o -o $(BDIR)/imgdiffRGB

$(BDIR)/thresh:	$(CDIR)/thresh.c $(IDIR)/lthresh.h $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(ODIR)/lthresh.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/thresh.c $(ODIR)/lthresh.o $(ODIR)/mcimage.o -o $(BDIR)/thresh

$(BDIR)/threshRGB:	$(CDIR)/threshRGB.c $(IDIR)/lthresh.h $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(ODIR)/lthresh.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/threshRGB.c $(ODIR)/lthresh.o $(ODIR)/mcimage.o -o $(BDIR)/threshRGB

$(BDIR)/histe:		$(CDIR)/histe.c $(IDIR)/lhiste.h $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(ODIR)/lhiste.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/histe.c $(ODIR)/lhiste.o $(ODIR)/mcimage.o -o $(BDIR)/histe

$(BDIR)/histeRGB:	$(CDIR)/histeRGB.c $(IDIR)/lhiste.h $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(ODIR)/lhiste.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/histeRGB.c $(ODIR)/lhiste.o $(ODIR)/mcimage.o -o $(BDIR)/histeRGB

$(BDIR)/histn:		$(CDIR)/histn.c $(IDIR)/lhistn.h $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(ODIR)/lhistn.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/histn.c $(ODIR)/lhistn.o $(ODIR)/mcimage.o -o $(BDIR)/histn

$(BDIR)/histnRGB:	$(CDIR)/histnRGB.c $(IDIR)/lhistn.h $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(ODIR)/lhistn.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/histnRGB.c $(ODIR)/lhistn.o $(ODIR)/mcimage.o -o $(BDIR)/histnRGB
	
$(BDIR)/wavg:		$(CDIR)/wavg.c $(IDIR)/lwavg.h $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(ODIR)/lwavg.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/wavg.c $(ODIR)/lwavg.o $(ODIR)/mcimage.o -lm -o $(BDIR)/wavg	
	
$(BDIR)/wavgRGB:		$(CDIR)/wavgRGB.c $(IDIR)/lwavg.h $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(ODIR)/lwavg.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/wavgRGB.c $(ODIR)/lwavg.o $(ODIR)/mcimage.o -lm -o $(BDIR)/wavgRGB	
	
$(BDIR)/pixelwiseCGA:	$(CDIR)/pixelwiseCGA.c $(IDIR)/lpixelwiseCGA.h $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(ODIR)/lpixelwiseCGA.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/pixelwiseCGA.c $(ODIR)/lpixelwiseCGA.o $(ODIR)/mcimage.o -lm -o $(BDIR)/pixelwiseCGA

$(BDIR)/pixelwiseCGA_RGB:	$(CDIR)/pixelwiseCGA_RGB.c $(IDIR)/lpixelwiseCGA_RGB.h $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(ODIR)/lpixelwiseCGA_RGB.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/pixelwiseCGA_RGB.c $(ODIR)/lpixelwiseCGA_RGB.o $(ODIR)/mcimage.o -lm -o $(BDIR)/pixelwiseCGA_RGB

$(BDIR)/CGA:		$(CDIR)/CGA.c $(IDIR)/lCGA.h $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(ODIR)/lCGA.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/CGA.c $(ODIR)/lCGA.o $(ODIR)/mcimage.o -lm -o $(BDIR)/CGA
	
$(BDIR)/CGA_RGB:	$(CDIR)/CGA_RGB.c $(IDIR)/lCGA_RGB.h $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(ODIR)/lCGA_RGB.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/CGA_RGB.c $(ODIR)/lCGA_RGB.o $(ODIR)/mcimage.o -lm -o $(BDIR)/CGA_RGB
	
$(BDIR)/PSNRestimator:	$(CDIR)/PSNRestimator.c $(IDIR)/lPSNRestimator.h $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(ODIR)/PSNRestimator.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/PSNRestimator.c $(ODIR)/lPSNRestimator.o $(ODIR)/mcimage.o -lm -o $(BDIR)/PSNRestimator
	
$(BDIR)/PSNRestimatorRGB:	$(CDIR)/PSNRestimatorRGB.c $(IDIR)/lPSNRestimator.h $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(ODIR)/PSNRestimator.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/PSNRestimatorRGB.c $(ODIR)/lPSNRestimator.o $(ODIR)/mcimage.o -lm -o $(BDIR)/PSNRestimatorRGB

$(BDIR)/deriche:	$(CDIR)/deriche.c $(IDIR)/lderiche.h $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(ODIR)/lderiche.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/deriche.c $(ODIR)/lderiche.o $(ODIR)/mcimage.o -lm -o $(BDIR)/deriche

# ===============================================================
# Library functions 
# ===============================================================

# Basic ones:
# ===============================================================

$(ODIR)/mcimage.o:	$(LDIR)/mcimage.c $(IDIR)/mcimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcimage.c -o $(ODIR)/mcimage.o

# Operators:
# ===============================================================

$(ODIR)/laddconst.o:	$(LDIR)/laddconst.c $(IDIR)/laddconst.h $(IDIR)/mcimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/laddconst.c -o $(ODIR)/laddconst.o

$(ODIR)/limgdiff.o:	$(LDIR)/limgdiff.c $(IDIR)/limgdiff.h $(IDIR)/mcimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/limgdiff.c -o $(ODIR)/limgdiff.o

$(ODIR)/lthresh.o:	$(LDIR)/lthresh.c $(IDIR)/lthresh.h $(IDIR)/mcimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lthresh.c -o $(ODIR)/lthresh.o

$(ODIR)/lhiste.o:	$(LDIR)/lhiste.c $(IDIR)/lhiste.h $(IDIR)/mcimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lhiste.c -o $(ODIR)/lhiste.o

$(ODIR)/lhistn.o:	$(LDIR)/lhistn.c $(IDIR)/lhistn.h $(IDIR)/mcimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lhistn.c -o $(ODIR)/lhistn.o

$(ODIR)/lwavg.o:	$(LDIR)/lwavg.c $(IDIR)/lwavg.h $(IDIR)/mcimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lwavg.c -o $(ODIR)/lwavg.o
	
$(ODIR)/lpixelwiseCGA.o:	$(LDIR)/lpixelwiseCGA.c $(IDIR)/lpixelwiseCGA.h $(IDIR)/mcimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lpixelwiseCGA.c -o $(ODIR)/lpixelwiseCGA.o
	
$(ODIR)/lpixelwiseCGA_RGB.o:	$(LDIR)/lpixelwiseCGA_RGB.c $(IDIR)/lpixelwiseCGA_RGB.h $(IDIR)/mcimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lpixelwiseCGA_RGB.c -o $(ODIR)/lpixelwiseCGA_RGB.o

$(ODIR)/lCGA.o:	$(LDIR)/lCGA.c $(IDIR)/lCGA.h $(IDIR)/mcimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lCGA.c -o $(ODIR)/lCGA.o

$(ODIR)/lCGA_RGB.o:	$(LDIR)/lCGA_RGB.c $(IDIR)/lCGA_RGB.h $(IDIR)/mcimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lCGA_RGB.c -o $(ODIR)/lCGA_RGB.o

$(ODIR)/PSNRestimator.o:	$(LDIR)/lPSNRestimator.c $(IDIR)/lPSNRestimator.h $(IDIR)/mcimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lPSNRestimator.c -o $(ODIR)/lPSNRestimator.o

$(ODIR)/lderiche.o:	$(LDIR)/lderiche.c $(IDIR)/lderiche.h $(IDIR)/mcimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lderiche.c -o $(ODIR)/lderiche.o

$(BDIR)/ppm2pgm:	$(CDIR)/ppm2pgm.c $(IDIR)/mcimage.h $(LDIR)/mcimage.c
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/ppm2pgm.c  $(ODIR)/mcimage.o -o $(BDIR)/ppm2pgm

$(BDIR)/pgm2ppm:	$(CDIR)/pgm2ppm.c $(IDIR)/mcimage.h $(LDIR)/mcimage.c
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/pgm2ppm.c $(ODIR)/mcimage.o -o $(BDIR)/pgm2ppm

# Documentation
# ===============================================================
$(DOCDIR)/tutorialEnglish.html: $(DOCDIR)/tutorialEnglish.tex
	cd $(DOCDIR); htlatex ./tutorialEnglish; cd ..

clean:
	rm -rf *~ $(ODIR)/* $(IDIR)/*~ $(CDIR)/*~ $(LDIR)/*~ $(DOCDIR)/*~ 
	cd $(DOCDIR); rm -rf *.4ct *.4tc *.aux *.dvi *.idv *.lg *.log *.tmp *.xref 




