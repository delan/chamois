#!/bin/sh
for i in dirty watermark; do
	echo converting $i to text ...
	convert $i.png $i.txt
	echo initialising header ...
	head -n1 $i.txt > clean.txt
	echo preformatting $i text ...
	tail -n+2 $i.txt > $i.txt.2
	cat $i.txt.2 | tr -d ' ' | cut -d '#' -f 1 > $i.txt.3
	mv $i.txt.3 $i.txt.2
	mv $i.txt.2 $i.txt
done
echo compiling subtract program ...
cc -ansi -Wall -pedantic -o sub sub.c
echo subtracting watermark from dirty ...
./sub >> clean.txt
echo converting clean text to image ...
convert clean.txt clean.png
rm {clean,dirty,watermark}.txt