
# cphex\makefile

LIBS=
all:	admin	LibSerCGRD.o


LibSerHV.o:	LibSerCGRD.c	LibSerCGRD.h	data.h	
	echo "compilation de LibSerCGRD"
	gcc -c LibSerHV.c

admin:	data.h	admin.c	LibSerCGRD.o			
	echo "Compilation de admin"
	gcc -o admin	admin.c	LibSerCGRD.o 

