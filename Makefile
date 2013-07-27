CFLAGS=`pkg-config --cflags MagickWand`
LDLIBS=`pkg-config --libs MagickWand`
all: chamois
chamois: chamois.c
