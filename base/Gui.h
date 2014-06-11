#ifndef GUI_H_
#define GUI_H_

#include "Lab4.h"
#include <time.h>
#include "IOEdit.h"
#include "IOGame.h"
#include "level.h"
#include "Shapes.h"
#include "Camera.h"
#include "Helper.h"
#include "Objective.h"
#include "physSystem.h"
//#include <ft2build.h>
//#include FT_FREETYPE_H

#define ICON_WIDTH 0.13
#define ICON_HEIGHT 0.22
#define BUTTON_WIDTH 0.4
#define BUTTON_HEIGHT 0.3
#define DIGIT_WIDTH 0.06
#define DIGIT_HEIGHT 0.15
#define PLAY_BUTTON 1
#define EDIT_BUTTON 2
#define QUIT_BUTTON 3
#define WORLD1_SELECT_BUTTON 4
#define WORLD2_SELECT_BUTTON 5
#define WORLD3_SELECT_BUTTON 6
#define WORLD4_SELECT_BUTTON 7
#define BACK_BUTTON 8

extern GLFWwindow *window;
extern int inProgress;

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

struct Button {
   glm::vec2 position;
   int textureIndex;
   int ID;
};

//Initialize gui
void initGui();
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
void GuiPressing(int mode, int xPos, int yPos);
//Retrieve the icon at Hot Bar index
Icon getHBIcon(int index);
int ImageLoad(char *filename, TexImage *image);
GLvoid LoadTexture(char *image_file, int tex_id);
void makeCheckerBoard ( int nRows, int nCols );
#endif
