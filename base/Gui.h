#ifndef GUI_H_
#define GUI_H_

#include "Lab4.h"
#include "IOEdit.h"
#include "Shapes.h"
#include "Camera.h"

/*TEXTURING*/

/*data struct for the image used for texture mapping*/
typedef struct TexImage {
   unsigned long sizeX;
   unsigned long sizeY;
   char *data;
} TexImage;

typedef struct RGB {
   GLubyte r;
   GLubyte g;
   GLubyte b;
} RGB;

void DrawCrosshair();
//Master call that will draw all gui elements if any depending on
//game mode
void DrawGui(int editMode);
//Draw in 2D
void ready2D();
//Draw in 3D
void ready3D();
int ImageLoad(char *filename, TexImage *image);
GLvoid LoadTexture(char *image_file, int tex_id);
#endif
