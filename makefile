EXE=$(BDIR)/addconst $(BDIR)/deriche $(BDIR)/ppm2pgm $(BDIR)/pgm2ppm $(BDIR)/imgdiff $(BDIR)/thresh $(BDIR)/histe $(BDIR)/wavg
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

$(BDIR)/imgdiff:	$(CDIR)/imgdiffEXE.c $(ODIR)/imgdiff.o $(IDIR)/imgdiff.h $(IDIR)/mcimage.h $(ODIR)/mcimage.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/imgdiffEXE.c $(ODIR)/imgdiff.o $(ODIR)/mcimage.o -o $(BDIR)/imgdiff

$(BDIR)/thresh:	        $(CDIR)/thresh.c $(IDIR)/lthresh.h $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(ODIR)/thresh.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/thresh.c $(ODIR)/lthresh.o $(ODIR)/mcimage.o -o $(BDIR)/thresh

$(BDIR)/histe:	        $(CDIR)/histe.c $(IDIR)/lhiste.h $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(ODIR)/histe.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/histe.c $(ODIR)/lhiste.o $(ODIR)/mcimage.o -o $(BDIR)/histe
	
$(BDIR)/wavg:	        $(CDIR)/wavg.c $(IDIR)/lwavg.h $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(ODIR)/wavg.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/wavg.c $(ODIR)/lwavg.o $(ODIR)/mcimage.o -o $(BDIR)/wavg

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

$(ODIR)/imgdiff.o:	$(LDIR)/imgdiff.c $(IDIR)/imgdiff.h $(IDIR)/mcimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/imgdiff.c -o $(ODIR)/imgdiff.o

$(ODIR)/thresh.o:	$(LDIR)/lthresh.c $(IDIR)/lthresh.h $(IDIR)/mcimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lthresh.c -o $(ODIR)/lthresh.o

$(ODIR)/histe.o:	$(LDIR)/lhiste.c $(IDIR)/lhiste.h $(IDIR)/mcimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lhiste.c -o $(ODIR)/lhiste.o

$(ODIR)/wavg.o:	$(LDIR)/lwavg.c $(IDIR)/lwavg.h $(IDIR)/mcimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lwavg.c -o $(ODIR)/lwavg.o

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




