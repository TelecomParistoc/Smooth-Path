/*
Copyright(C) 2017
Vincent Gaillard

Permission granted to use this code only for teaching projects and private practice.
Do not distribute this code outside the teaching assignements.
All rights reserved.
*/

#include <math.h>
#include <stdio.h>
#include <graphics.h>

typedef struct{
	float x;
	float y;
} vec;

// inputs
vec A, B, Adir, Bdir;
float r1, r2;
// output
vec C, D, O1, O2;

/* Return length of a vector
*/
float len(vec v)
{
	return(sqrt(v.x * v.x + v.y * v.y));
}

/* Return distance between v1 and v2
*/
float dist(vec v1, vec v2)
{
	float a = v1.x - v2.x;
	float b = v1.y - v2.y;
	return(sqrt(a * a + b * b));
}

/* Return norme of a vector
*/
vec norm(vec v)
{
	float l = len(v);
	vec res = {v.x/l, v.y/l};
	return res;
}

/* return perpenducular vector of a vector (trigo)
*/
vec perp(vec v)
{
	vec res = {-v.y, v.x};
	return res;
}

/* return scalar product between two vectors
*/
float scal(vec v1, vec v2)
{
	return(v1.x * v2.x + v1.y * v2.y);
}

/* multiplication between vector and scalar
*/
vec mult(vec v, float s)
{
	vec res = {v.x * s, v.y * s};
	return res;
}

/* Add two vectors
*/
vec add(vec v1, vec v2)
{
	vec res = {v1.x + v2.x, v1.y + v2.y};
	return res;
}

/* Substract two vectors
*/
vec sub(vec v1, vec v2)
{
	vec res = {v1.x - v2.x, v1.y - v2.y};
	return res;
}

/* return intersection point betwwen two lines
** a line is define by a point and a vector
** flag: 0: intersection found
**       1: same line
**       2: parallel lines (not same)
*/
vec intersect(vec p1, vec p2, vec v1, vec v2, int* flag)
{
	vec res;
	if(v1.y * v2.x != v2.y * v1.x)
	{
		// lines are not parallel
		res.x = (v1.x*v2.x*(p2.y-p1.y)+p1.x*v1.y*v2.x-p2.x*v2.y*v1.x)/(v1.y*v2.x-v2.y*v1.x);
		if(v1.x == 0)
			res.y = p1.y + (res.x - p1.x) / v1.x * v1.y;
		else
			res.y = p2.y + (res.x - p2.x) / v2.x * v2.y;
		*flag = 0;
		return res;
	}
	// lines are parallel
	*flag = 2;
	if((p1.y - p2.y) * v1.x == (p1.x - p2.x) * v1.y)
		// lines are identical
		*flag = 1;
	res.x = (p1.x + p2.x) / 2;
	res.y = (p1.y + p2.y) / 2;
	return res;
}

/* Return determinant between two vectors
*/
float det(vec v1, vec v2)
{
	return(v1.x * v2.y - v1.y * v2.x);
}

/* Problem: Go from A in Adir direction to B in Bdir direction smoothly
** Constraints: Rotation at A must have a radius r1 and, at B, a r2 radius
** Result: Points C and D where the path leave A circle and respectively join B circle
*/
void smoothPath()
{
	// normalize Adir and Bdir
	Adir = norm(Adir);
	Bdir = norm(Bdir);
	// get perpendicular direction of Adir and Bdir
	O1 = perp(Adir);
	O2 = perp(Bdir);
	// get intersection between two perpendicular lines
	int flag;
	vec I = intersect(A, B, O1, O2, &flag);
	if(flag == 2)
	{
		// directions and points are aligned
		C.x = A.x;
		C.y = A.y;
		D.x = B.x;
		D.y = B.y;
	}
	// O1 and O2 become center of cercles followed by the path
	O1 = add(A, mult(O1, (dist(I, add(A, O1)) < dist(I, add(A, mult(O1, -1)))) ? r1 : -r1));
	O2 = add(B, mult(O2, (dist(I, add(B, O2)) < dist(I, add(B, mult(O2, -1)))) ? r2 : -r2));
	// find all confirurations of two tangents of the two circles
	float ABx = O2.x - O1.x;
	float ABy = O2.y - O1.y;
	// get rotation (CW, or CCW) aroud circles
	int rot1 = (det(sub(A, O1), Adir) > 0) ? 1 : 0; // 1:CCW, 0:CW
	int rot2 = (det(sub(B, O2), Bdir) > 0) ? 1 : 0; // 1:CCW, 0:CW
	float r, theta1, theta2;
	if(rot1 == rot2)
		// path does not cross line between O1 and O2
		r = r1 - r2;
	else
		// path cross line between O1 and O2
		r = r1 + r2;
	// two possibilities for angle still exists
	theta1 = 2 * atan((ABy + sqrt(ABx*ABx + ABy*ABy - r*r)) / (ABx + r));
	theta2 = 2 * atan((ABy - sqrt(ABx*ABx + ABy*ABy - r*r)) / (ABx + r));
	// find angles (x,O1A) and (x,O1B)
	float a1 = atan2(A.y - O1.y, A.x - O1.x);
	// choose between theta1 and theta2
	float diff1 = theta1 - a1;
	float diff2 = theta2 - a1;
	if(diff1 < 0) diff1 += 2 * M_PI;
	if(diff2 < 0) diff2 += 2 * M_PI;
	int cond = (diff1 > diff2 ? 1 : 0) + rot1;
	a1 = (cond == 1) ? theta1 : theta2;
	C.x = O1.x + r1 * cos(a1);
	C.y = O1.y + r1 * sin(a1);
	if(rot1 != rot2)
	{
		D.x = O2.x + r2 * cos(a1 + M_PI);
		D.y = O2.y + r2 * sin(a1 + M_PI);
	}
	else
	{
		D.x = O2.x + r2 * cos(a1);
		D.y = O2.y + r2 * sin(a1);
	}
}

/* Return the maximum value between a and b
*/
float maxf(float a, float b)
{
	return (a < b) ? b : a;
}

/* Return the minimum value between a and b
*/
int mini(int a, int b)
{
	return (a > b) ? b : a;
}

void draw()
{
	// get maximum and minimum values
	float maxR = maxf(r1, r2);
	float min = O1.x, max = O1.x;
	if(min > O1.y) min = O1.y;
	if(min > O2.x) min = O2.x;
	if(min > O2.y) min = O2.y;
	if(max < O1.y) max = O1.y;
	if(max < O2.x) max = O2.x;
	if(max < O2.y) max = O2.y;
	min -= maxR + 2;
	max += maxR + 2;
	// draw
	int gd = DETECT, gm;
	initgraph(&gd, &gm, NULL);
	float s = mini(getmaxx(), getmaxy()) / (max - min);
	circle((O1.x - min) * s, (O1.y - min) * s, r1 * s);
	circle((O2.x - min) * s, (O2.y - min) * s, r2 * s);
	line((C.x - min)*s, (C.y - min) * s, (D.x - min)*s, (D.y - min)*s);
	setcolor(4);
	line((A.x + Adir.x - min)*s, (A.y + Adir.y - min)*s, (A.x - min)*s, (A.y - min)*s);
	line((B.x + Bdir.x - min)*s, (B.y + Bdir.y - min)*s, (B.x - min)*s, (B.y - min)*s);
	outtextxy((A.x - min)*s, (A.y - min)*s, "A");
	outtextxy((B.x - min)*s, (B.y - min)*s, "B");
	outtextxy((C.x - min)*s, (C.y - min)*s, "C");
	outtextxy((D.x - min)*s, (D.y - min)*s, "D");
	getch();
	closegraph();
}

int main()
{
	A.x = 2.2; // Start position X
	A.y = -3.4; // Start position Y
	Adir.x = 2; // Start direction X
	Adir.y = 0.2; // Start direction Y
	B.x = -1.1; // Stop position X
	B.y = 4; // Stop position Y
	Bdir.x = -0.3; // Stop direction X
	Bdir.y = -1.4; //Stop direction Y
	r1 = 0.8; // First radius
	r2 = 2.4; // Second radius
	printf("A[%f,%f] // Start\n", A.x, A.y);
	printf("B[%f,%f] // Stop\n", B.x, B.y);
	smoothPath();
	printf("O1:[%f,%f] // Fisrt circle center\n", O1.x, O1.y);
	printf("O2:[%f,%f] // Second circle center\n", O2.x, O2.y);
	printf("C[%f,%f] // Point where the path separates from the circle 1\n", C.x, C.y);
	printf("D[%f,%f] // Point where the path join the circle 2\n", D.x, D.y);
	draw();
	return 0;
}
