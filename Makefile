SHELL = /bin/sh
prefix = /usr
CC = gcc
C++ = g++
GLUT_LIBS =
X_LIBADD =  -lXmu -lXext -lXi -lX11
INCLUDES = -Iinclude -I$(prefix)/include  
LDADD =
CFLAGS = -g -O2 -Wall -fomit-frame-pointer -ffast-math -D_REENTRANT
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS)
LINK = $(CC) $(CFLAGS) $(LDFLAGS) -o $@
#./main

.SUFFIXES:
.SUFFIXES: .cpp .c .o 

.c.o:
	$(COMPILE) -c $<

.cpp.o:
	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c $<

	
all: main

CLI_OBJECTS=compilation.o 

main: $(CLI_OBJECTS)
	$(LINK) $(CLI_OBJECTS) $(LDADD) $(LIBS)
	rm -f *.o
	clear
	

clean:
	clear
	-rm -f *.o $(PROGRAMS)
