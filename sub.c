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
int main(void) {
	FILE *dF = fopen("dirty.txt", "r");
	FILE *wF = fopen("watermark.txt", "r");
	int x, y, temp1, temp2;
	double dR, dG, dB, dA; /* dirty */
	double wR, wG, wB, wA; /* watermark */
	double cR, cG, cB, cA; /* clean */
	while (1) {
		temp1 = fscanf(dF, "%d,%d:(%lf,%lf,%lf,%lf)",
			&x, &y, &dR, &dG, &dB, &dA);
		temp2 = fscanf(wF, "%d,%d:(%lf,%lf,%lf,%lf)",
			&x, &y, &wR, &wG, &wB, &wA);
		if (temp1 < 6 || temp2 < 6)
			break;
		dA /= 256.0;
		wA /= 256.0;
		/*
			let
			a = cA
			b = wA
			c = dA
			x = c[RGB]
			y = w[RGB]
			z = d[RGB]
		*/
		cA = (dA - wA) / (1 - wA);
		cR = (dA * dR - wA * wR) / (cA - cA * wA);
		cG = (dA * dG - wA * wG) / (cA - cA * wA);
		cB = (dA * dB - wA * wB) / (cA - cA * wA);
		printf("%d,%d:(%d,%d,%d,%d)\n", x, y, (int) cR,
			(int) cG, (int) cB, (int) (cA * 256.0));
	}
	return 0;
}
