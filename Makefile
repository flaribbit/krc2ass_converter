CC=gcc
CFLAGS=-s -O2 -DNDEBUG -Izlib
OBJ=zlib/adler32.o zlib/compress.o zlib/crc32.o zlib/deflate.o zlib/gzclose.o \
	zlib/gzlib.o zlib/gzread.o zlib/gzwrite.o zlib/infback.o zlib/inffast.o \
	zlib/inflate.o zlib/inftrees.o zlib/trees.o zlib/uncompr.o zlib/zutil.o

main.exe: main.o resource.o $(OBJ)
	$(CC) main.o resource.o $(OBJ) -luser32 -lgdi32 -lcomctl32 -lkernel32 -mwindows -o main.exe
resource.o: resource.rc
	windres resource.rc -o resource.o
main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

zlib/adler32.o: zlib/adler32.c
	$(CC) $(CFLAGS) -c zlib/adler32.c -o zlib/adler32.o
zlib/compress.o: zlib/compress.c
	$(CC) $(CFLAGS) -c zlib/compress.c -o zlib/compress.o
zlib/crc32.o: zlib/crc32.c
	$(CC) $(CFLAGS) -c zlib/crc32.c -o zlib/crc32.o
zlib/deflate.o: zlib/deflate.c
	$(CC) $(CFLAGS) -c zlib/deflate.c -o zlib/deflate.o
zlib/gzclose.o: zlib/gzclose.c
	$(CC) $(CFLAGS) -c zlib/gzclose.c -o zlib/gzclose.o
zlib/gzlib.o: zlib/gzlib.c
	$(CC) $(CFLAGS) -c zlib/gzlib.c -o zlib/gzlib.o
zlib/gzread.o: zlib/gzread.c
	$(CC) $(CFLAGS) -c zlib/gzread.c -o zlib/gzread.o
zlib/gzwrite.o: zlib/gzwrite.c
	$(CC) $(CFLAGS) -c zlib/gzwrite.c -o zlib/gzwrite.o
zlib/infback.o: zlib/infback.c
	$(CC) $(CFLAGS) -c zlib/infback.c -o zlib/infback.o
zlib/inffast.o: zlib/inffast.c
	$(CC) $(CFLAGS) -c zlib/inffast.c -o zlib/inffast.o
zlib/inflate.o: zlib/inflate.c
	$(CC) $(CFLAGS) -c zlib/inflate.c -o zlib/inflate.o
zlib/inftrees.o: zlib/inftrees.c
	$(CC) $(CFLAGS) -c zlib/inftrees.c -o zlib/inftrees.o
zlib/trees.o: zlib/trees.c
	$(CC) $(CFLAGS) -c zlib/trees.c -o zlib/trees.o
zlib/uncompr.o: zlib/uncompr.c
	$(CC) $(CFLAGS) -c zlib/uncompr.c -o zlib/uncompr.o
zlib/zutil.o: zlib/zutil.c
	$(CC) $(CFLAGS) -c zlib/zutil.c -o zlib/zutil.o
