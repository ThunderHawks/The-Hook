#include "ViewFrustum.h"

/*for reference*/
#define m11 matArray[0]
#define m12 matArray[1]
#define m13 matArray[2]
#define m14 matArray[3]
#define m21 matArray[4]
#define m22 matArray[5]
#define m23 matArray[6]
#define m24 matArray[7]
#define m31 matArray[8]
#define m32 matArray[9]
#define m33 matArray[10]
#define m34 matArray[11]
#define m41 matArray[12]
#define m42 matArray[13]
#define m43 matArray[14]
#define m44 matArray[15]

/* input:   the center of a bounding sphere, the radius of the bounding sphere (put 0 for just a point), and the model view matrix
 * summary: Checks if a bounding sphere is inside the view frustum
 * output:  A number representing which plane it didn't pass. Still working on things other than 0. might just have it always be 1 if it fails
 *				0 = inside frustum
 *				1 = not in frustum
*/
int checkViewFrustum (glm::vec3 point, int rad, glm::mat4 mView) {
	float a;					//a is for x
	float b;					//b is for y
	float c;					//c is for z
	float d;					//d is for w
	const float *pSource = (const float*)glm::value_ptr(mView); //stores the mat4 in a way we can read it
	double matArray[16] = {0.0};	//save content of mat4, because pointers are scary. Can be removed later if it needs to be
	
	/*store the mView matrix inside this array for later usage*/
	for (int i = 0; i < 16; ++i)
		matArray[i] = pSource[i];
		
	/*I can use 2 fancy loops to iterate through it and make the code smaller. However, Until I know this works, I won't be using it. Here it is below though:*/
	
		for (int i = 0; i < 12; ) {
			a = m41 + matArray[i++];
			b = m42 + matArray[i++];
			c = m43 + matArray[i++];
			d = m44 + matArray[i++];
			
			if (a*point.x + b*point.y + c*point.z + d + rad < 0)
				return 1;
		}
		
		for (int i = 0; i < 12; ) {
			a = m41 - matArray[i++];
			b = m42 - matArray[i++];
			c = m43 - matArray[i++];
			d = m44 - matArray[i++];
			
			if (a*point.x + b*point.y + c*point.z + d + rad < 0)
				return 1;
		}
	
		
	/* checking left plane
	a = m41 + m11;
	b = m42 + m12;
	c = m43 + m13;
	d = m44 + m14;
	//if it is not in the space
	if (a*point.x + b*point.y + c*point.z + d + rad < 0)
		return 1;
	
	/* checking right plane
	a = m41 - m11;
	b = m42 - m12;
	c = m43 - m13;
	d = m44 - m14;
	//if it is not in the space
	if (a*point.x + b*point.y + c*point.z + d + rad < 0)
		return 2;
		
	/* checking bottom plane
	a = m41 + m21;
	b = m42 + m22;
	c = m43 + m23;
	d = m44 + m24;
	//if it is not in the space
	if (a*point.x + b*point.y + c*point.z + d + rad < 0)
		return 3;
		
	/* checking top plane
	a = m41 - m21;
	b = m42 - m22;
	c = m43 - m23;
	d = m44 - m24;
	//if it is not in the space
	if (a*point.x + b*point.y + c*point.z + d + rad < 0)
		return 4;
		
	/* checking near plane
	a = m41 + m31;
	b = m42 + m32;
	c = m43 + m33;
	d = m44 + m34;
	//if it is not in the space
	if (a*point.x + b*point.y + c*point.z + d + rad < 0)
		return 5;
		
	/* checking far plane
	a = m41 - m31;
	b = m42 - m32;
	c = m43 - m33;
	d = m44 - m34;
	//if it is not in the space
	if (a*point.x + b*point.y + c*point.z + d + rad < 0)
		return 6;
	
	*/	
	return 0;
}
