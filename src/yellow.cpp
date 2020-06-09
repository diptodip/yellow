#include <stdio.h>

struct Vec3D {
	double x;
	double y;
	double z;
};

double dot(Vec3D *a, Vec3D *b) {
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

int main(int argc, char **args) {
	Vec3D v = {10, 5, 7.5};
	double length = dot(&v, &v);
	printf("v is %.2f %.2f %.2f\n", v.x, v.y, v.z);
	printf("length is %.2f\n", length);
	return 0;
}
