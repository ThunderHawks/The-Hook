#include "Gui.h"

TexImage *TextureImage;
RGB myImage[64][64];
RGB* g_pixel;

//Location of each HotBar Icon Index
vector<glm::vec2> HBIndices;

//Location of each Selection Screen Icon Index
vector<glm::vec2> SSIndices;

//Last selection index pressed
int lastSelectionIndex = -1;

void initGui(int EditMode) {
   if(EditMode) {
      //Initialize HotBar icon indices
      for(int i = 0; i < 9; i++) {
         HBIndices.push_back(glm::vec2(0.15 * i - 0.6, -0.88));
      }

     //Initialize Selection screen icon indicies
     for(int j = 3; j >= 0; j--) {
         for(int i = 0; i < 9; i++) {
            SSIndices.push_back(glm::vec2(0.15 * i - 0.6 , 0.3 * j - 0.2));
         }
      }
   }
}

void DrawCrosshair() {
   SetupSq(0, 0, 5, 0.01, 0.04);
   SetupSq(0, 0, 5, 0.03, 0.01);
}

void ready2D() {
   //glCullFace(GL_BACK);
   glDisable(GL_CULL_FACE);

   glMatrixMode(GL_PROJECTION);
   ModelTrans.loadIdentity();
   safe_glUniformMatrix4fv(h_uProjMatrix, glm::value_ptr(glm::mat4(1)));

   ModelTrans.loadIdentity();
   glMatrixMode(GL_MODELVIEW);
   glm::ortho(0.0f, g_width, 0.0f, g_height, -1.0f, 1.0f);
   safe_glUniformMatrix4fv(h_uViewMatrix, glm::value_ptr(glm::mat4(1)));

   glDisable(GL_DEPTH_TEST);
}

void ready3D() {
    glViewport(0, 0, g_width, g_height);
    glMatrixMode(GL_PROJECTION);

    ModelTrans.loadIdentity();
    safe_glUniformMatrix4fv(h_uProjMatrix, glm::value_ptr(glm::perspective(80.0f, (float)g_width/g_height, 0.1f, 500.0f)));

    glMatrixMode(GL_MODELVIEW);
    ModelTrans.loadIdentity();
    SetView();

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
}

void DrawHotBar() {
   glm::vec2 tempPos;

   //Draw background
   SetupSq(0, -0.9, 5, 1.4, 0.3);

   //Draw Icons
   for(int i = 0; i < HBIndices.size(); i++) {
      tempPos = HBIndices[i];
      SetupSq(tempPos.x, tempPos.y, 4, ICON_WIDTH, ICON_HEIGHT);      
   }
}

void DrawSelection() {
   glm::vec2 tempPos;

   //Draw background
   SetupSq(0, 0.25, 5, 1.4, 1.3);

   //Draw icons
   for(int i = 0; i < SSIndices.size(); i++) {
      tempPos = SSIndices[i];
      SetupSq(tempPos.x, tempPos.y, 4, ICON_WIDTH, ICON_HEIGHT);      
   }
}


bool iconPressed(glm::vec2 iconPos, int xPos, int yPos) {

   float worldX = p2i_x(xPos);
   float worldY = p2i_y(yPos);

   //printf("iconPos: %f %f, mouse: %f %f\n", iconPos.x, iconPos.y, worldX, worldY);
   //printf("mouse: %d %d, p2w: %f %f, p2i: %f %f\n", xPos, yPos, worldX, worldY, p2i_x(xPos), p2i_y(yPos));
   //printf("width/height: %f %f\n\n", p2i_x(ICON_WIDTH), p2i_y(ICON_HEIGHT));
   //printf("world: %f %f, icon: %f %f\n", worldX, worldY, iconPos.x, iconPos.y);
   //printf("x: %f <= %f <= %f\n", iconPos.x - ICON_WIDTH/2.0, worldX, iconPos.x + ICON_WIDTH/2.0);
   //printf("y: %f <= %f <= %f\n", iconPos.y - ICON_HEIGHT/2.0, worldY, iconPos.y + ICON_HEIGHT/2.0);

   if(iconPos.x - ICON_WIDTH/2.0 <= worldX && worldX <= iconPos.x + ICON_WIDTH/2.0) {
      if(iconPos.y - ICON_HEIGHT/2.0 <= worldY && worldY <= iconPos.y + ICON_HEIGHT/2.0) {
         return true;
      }
   }

   return false;
}

void GuiPressing(int xPos, int yPos) {
   //printf("Tried to press button at %d %d\n", xPos, yPos);

   //Test HB icons
   for(int i = 0; i < HBIndices.size(); i++) {

      if(iconPressed(HBIndices[i], xPos, yPos) == true) {
         printf("~~~~Icon %d Pressed on hotbar\n", i);
         if(lastSelectionIndex != -1) {
            SetHotBarIndex(i + 1, lastSelectionIndex);
            lastSelectionIndex = -1;
         }
         return;
      }
   }

   //Test SS icons
   for(int i = 0; i < SSIndices.size(); i++) {
      if(iconPressed(SSIndices[i], xPos, yPos) == true) {
         printf("~~~~Icon %d Pressed on selection\n", i);
         lastSelectionIndex = i;
         return; 
      }
   }
}

void DrawGui(int editMode) {

   ready2D();

   if(editMode) {
      DrawHotBar();
      //Display gui with all of the models to edit hotbar
      if(getEPressed('G')) {
         DrawSelection();
      }
   }
   else {
      DrawCrosshair();
   }

   ready3D();
}

GLvoid LoadTexture(char* image_file, int texID) {

  TextureImage = (TexImage *) malloc(sizeof(TexImage));
  if (TextureImage == NULL) {
    printf("Error allocating space for image");
    exit(1);
  }
  cout << "trying to load " << image_file << endl;
  if (!ImageLoad(image_file, TextureImage)) {
    exit(1);
  }
  /*  2d texture, level of detail 0 (normal), 3 components (red, green, blue),            */
  /*  x size from image, y size from image,                                              */
  /*  border 0 (normal), rgb color data, unsigned byte data, data  */
  glBindTexture(GL_TEXTURE_2D, texID);
  glTexImage2D(GL_TEXTURE_2D, 0, 3,
    TextureImage->sizeX, TextureImage->sizeY,
    0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); /*  cheap scaling when image bigger than texture */
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); /*  cheap scaling when image smalled than texture*/

}

static unsigned int getint(FILE *fp) {
  int c, c1, c2, c3;

  /*  get 4 bytes */
  c = getc(fp);
  c1 = getc(fp);
  c2 = getc(fp);
  c3 = getc(fp);

  return ((unsigned int) c) +
    (((unsigned int) c1) << 8) +
    (((unsigned int) c2) << 16) +
    (((unsigned int) c3) << 24);
}

static unsigned int getshort(FILE *fp){
  int c, c1;

  /* get 2 bytes*/
  c = getc(fp);
  c1 = getc(fp);

  return ((unsigned int) c) + (((unsigned int) c1) << 8);
}

int ImageLoad(char *filename, TexImage *image) {
  FILE *file;
  unsigned long size;                 /*  size of the image in bytes. */
  unsigned long i;                    /*  standard counter. */
  unsigned short int planes;          /*  number of planes in image (must be 1)  */
  unsigned short int bpp;             /*  number of bits per pixel (must be 24) */
  char temp;                          /*  used to convert bgr to rgb color. */

  /*  make sure the file is there. */
  if ((file = fopen(filename, "rb"))==NULL) {
    printf("File Not Found : %s\n",filename);
    return 0;
  }

  /*  seek through the bmp header, up to the width height: */
  fseek(file, 18, SEEK_CUR);

  /*  No 100% errorchecking anymore!!! */

  /*  read the width */    image->sizeX = getint (file);

  /*  read the height */
  image->sizeY = getint (file);

  /*  calculate the size (assuming 24 bits or 3 bytes per pixel). */
  size = image->sizeX * image->sizeY * 3;

  /*  read the planes */
  planes = getshort(file);
  if (planes != 1) {
    printf("Planes from %s is not 1: %u\n", filename, planes);
    return 0;
  }

  /*  read the bpp */
  bpp = getshort(file);
  if (bpp != 24) {
    printf("Bpp from %s is not 24: %u\n", filename, bpp);
    return 0;
  }

  /*  seek past the rest of the bitmap header. */
  fseek(file, 24, SEEK_CUR);

  /*  read the data.  */
  image->data = (char *) malloc(size);
  if (image->data == NULL) {
    printf("Error allocating memory for color-corrected image data");
    return 0;
  }
  if ((i = fread(image->data, size, 1, file)) != 1) {
    printf("Error reading image data from %s.\n", filename);
    return 0;
  }

  for (i=0;i<size;i+=3) { /*  reverse all of the colors. (bgr -> rgb) */
    temp = image->data[i];
    image->data[i] = image->data[i+2];
    image->data[i+2] = temp;
  }

  fclose(file); /* Close the file and release the filedes */

  /*  we're done. */
  return 1;
}
