/*
x, y, z = old, overlay, new
a = alpha [0..1]

z=(1-a)x+ay

Now solve for x in terms of a, y, z:

(1-a)x=z-ay
x=(z-ay)/(1-a)

*/

#include <stdio.h>
int main(void) {
	FILE *f = fopen("dirty.txt", "r");
	FILE *F = fopen("watermark.txt", "r");
	int x, y, temp1, temp2;
	double r, g, b, R, G, B, A;
	while (1) {
		temp1 = fscanf(f, "%d,%d:(%lf,%lf,%lf)", &x, &y, &r, &g, &b);
		temp2 = fscanf(F, "%d,%d:(%lf,%lf,%lf,%lf)", &x, &y, &R, &G, &B, &A);
		if (temp1 < 5 || temp2 < 6)
			break;
		r = (r - (A / 255.0) * R) / (1 - (A / 255.0));
		g = (g - (A / 255.0) * G) / (1 - (A / 255.0));
		b = (b - (A / 255.0) * B) / (1 - (A / 255.0));
		printf("%d,%d:(%d,%d,%d,255)\n", x, y, (int) r, (int) g, (int) b);
	}
	return 0;
}
