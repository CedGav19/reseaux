# cphex\makefile
LIBS=
all:    admin    cli    ser    udplib.o    LibSerCGRD.o  

LibSerCGRD.o: LibSerCGRD.c LibSerCGRD.h data.h
		echo "compilation de LibSerCGRD"
		cc -c LibSerCGRD.c

admin: data.h admin.c LibSerCGRD.o
		echo "Compilation de admin"
		cc -o admin admin.c LibSerCGRD.o

udplib.o: ./udplib/udplib.h ./udplib/udplib.c
		echo "Compilation de udplib.o"
		cc -c ./udplib/udplib.c

cli: cli.c requeteCGRD.h data.h udplib.o
		echo "Compilation de client"
		cc -o cli cli.c udplib.o $(LIBS)

ser: ser.c requeteCGRD.h data.h udplib.o LibSerCGRD.o
		echo "Compilation de serveur"
		cc -o ser ser.c udplib.o LibSerCGRD.o $(LIBS)
		