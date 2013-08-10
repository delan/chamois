/*

http://stackoverflow.com/q/10781953

x, y, z = component (old, overlay, new)
a, b, c = alpha [0..1] (old, overlay, new)

c=b+a(1-b)
z=(by+ax(1-b))/c

Now solve for (a, x) in terms of (b, c, y, z):

a(1-b)=c-b
a=(c-b)/(1-b)

ax(1-b)=cz-by
x=(cz-by)/(a-ab)

*/

#include <stdio.h>
#include <wand/MagickWand.h>

int ThrowWandException(MagickWand *w) {
	ExceptionType s;
	char *d = MagickGetException(w, &s);
	fprintf(stderr, "error %d: %s\n", s, d);
	MagickRelinquishMemory(d);
	return 1;
}

int main(int argc, char **argv) {
	int retval = 0;
	size_t x, y;
	size_t dW, dH, wW, wH; /* dimensions: dirty, watermark */
	double dR, dG, dB, dA; /* pixel components of dirty */
	double wR, wG, wB, wA; /* pixel components of watermark */
	double cR, cG, cB, cA; /* pixel components of clean */
	MagickWand *dM, *wM, *cM; /* wands: dirty, watermark, clean */
	PixelIterator *dI, *wI, *cI; /* iterators: dirty, watermark, clean */
	PixelWand **dP, **wP, **cP; /* row arrays: dirty, watermark, clean */
	if (argc < 4) {
		fputs("usage: sub <dirty> <watermark> <clean>\n", stderr);
		return 1;
	}
	MagickWandGenesis();
	dM = NewMagickWand();
	wM = NewMagickWand();
	cM = NewMagickWand();
	if (MagickReadImage(dM, argv[1]) == MagickFalse) {
		retval = ThrowWandException(dM);
		goto cleanup;
	}
	if (MagickReadImage(wM, argv[2]) == MagickFalse) {
		retval = ThrowWandException(dM);
		goto cleanup;
	}
	dW = MagickGetImageWidth(dM);
	dH = MagickGetImageHeight(dM);
	wW = MagickGetImageWidth(wM);
	wH = MagickGetImageHeight(wM);
	if (dW != wW || dH != wH) {
		fputs("error: different image dimensions\n", stderr);
		retval = 1;
		goto cleanup;
	}
	MagickSetSize(cM, dW, dH);
	MagickReadImage(cM, "xc:none");
	dI = NewPixelIterator(dM);
	wI = NewPixelIterator(wM);
	cI = NewPixelIterator(cM);
	for (y = 0; y < dH; y++) {
		dP = PixelGetNextIteratorRow(dI, &x);
		wP = PixelGetNextIteratorRow(wI, &x);
		cP = PixelGetNextIteratorRow(cI, &x);
		while (x--) {
			dR = PixelGetRed(dP[x]);
			dG = PixelGetGreen(dP[x]);
			dB = PixelGetBlue(dP[x]);
			dA = PixelGetAlpha(dP[x]);
			wR = PixelGetRed(wP[x]);
			wG = PixelGetGreen(wP[x]);
			wB = PixelGetBlue(wP[x]);
			wA = PixelGetAlpha(wP[x]);
			cA = (dA - wA) / (1 - wA);
			cR = (dA * dR - wA * wR) / (cA - cA * wA);
			cG = (dA * dG - wA * wG) / (cA - cA * wA);
			cB = (dA * dB - wA * wB) / (cA - cA * wA);
			PixelSetRed(cP[x], cR);
			PixelSetGreen(cP[x], cG);
			PixelSetBlue(cP[x], cB);
			PixelSetAlpha(cP[x], cA);
		}
		PixelSyncIterator(cI);
	}
	if (MagickWriteImage(cM, argv[3]) == MagickFalse) {
		retval = ThrowWandException(dM);
		goto cleanup;
	}
	cleanup:
	DestroyMagickWand(dM);
	DestroyMagickWand(wM);
	DestroyMagickWand(cM);
	DestroyPixelIterator(dI);
	DestroyPixelIterator(wI);
	DestroyPixelIterator(cI);
	MagickWandTerminus();
	return retval;
}
