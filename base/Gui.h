#ifndef GUI_H_
#define GUI_H_

#include "Lab4.h"
#include "IOEdit.h"
#include "level.h"
#include "Shapes.h"
#include "Camera.h"
#include "Helper.h"
//#include <ft2build.h>
//#include FT_FREETYPE_H

#define ICON_WIDTH 0.13
#define ICON_HEIGHT 0.22

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

struct Icon {
   Entity entity;
   float lookAtDistance;
   int textureIndex;
   glm::vec2 position;
}; 

//Initialize gui
void initGui(int EditMode);
void DrawCrosshair();
//Master call that will draw all gui elements if any depending on
//game mode
void DrawGui(int editMode);
//Draw in 2D
void ready2D();
//Draw in 3D
void ready3D();
//Called in IOEdit.cpp when in gui mode and the left mouse button
//is pressed
void GuiPressing(int xPos, int yPos);
//Retrieve the icon at Hot Bar index
Icon getHBIcon(int index);
int ImageLoad(char *filename, TexImage *image);
GLvoid LoadTexture(char *image_file, int tex_id);
void makeCheckerBoard ( int nRows, int nCols );

//text stuff at the bottom! Yay
void initText2D(const char * texturePath);
void printText2D(string text, int x, int y, int size);
void cleanupText2D();
#endif
