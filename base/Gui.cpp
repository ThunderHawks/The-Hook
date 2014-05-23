#include "Gui.h"

TexImage *TextureImage;
RGB myImage[64][64];
RGB* g_pixel;


void DrawCrosshair() {
   //SetupSq(0, 0, 5, 0.2, 0.2);
   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glLineWidth(3.0);
   SetupCube(0, 0, 0, 5, 0, 0.01, 0.04, 0.01);
   SetupCube(0, 0, 0, 5, 0, 0.03, 0.01, 0.01);
   //SetupCube(float x, float y, float z, int material, float angle, float scaleX, float scaleY, float scaleZ)
   //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void ready2D() {
   glCullFace(GL_BACK);
   glDisable(GL_CULL_FACE);

   //ModelTrans.MatrixMode
   glMatrixMode(GL_PROJECTION);
   ModelTrans.loadIdentity();
   safe_glUniformMatrix4fv(h_uProjMatrix, glm::value_ptr(glm::mat4(1)));
   //glLoadIdentity();

   ModelTrans.loadIdentity();
   glMatrixMode(GL_MODELVIEW);
   glm::ortho(0.0f, g_width, 0.0f, g_height, -1.0f, 1.0f);
   safe_glUniformMatrix4fv(h_uViewMatrix, glm::value_ptr(glm::mat4(1)));

   glDisable(GL_DEPTH_TEST);
   //glLoadIdentity();
//   glDisable(GL_DEPTH_TEST);

   //glCullFace(GL_BACK);
   //glEnable(GL_CULL_FACE);
}

void ready3D() {
/*
   SetView();
  // glMatrixMode(GL_PROJECTION);
   ModelTrans.popMatrix();

   SetProjectionMatrix();

  /// safe_glUniformMatrix4fv(h_uViewMatrix, glm::value_ptr(glm::mat4(1)));
  //glMatrixMode(GL_MODELVIEW);
   ModelTrans.popMatrix();
   glEnable(GL_DEPTH_TEST);
*/
    glViewport(0, 0, g_width, g_height);
    glMatrixMode(GL_PROJECTION);

   // glLoadIdentity();
    ModelTrans.loadIdentity();
    safe_glUniformMatrix4fv(h_uProjMatrix, glm::value_ptr(glm::perspective(80.0f, (float)g_width/g_height, 0.1f, 500.0f)));
    //glm::perspective(80.0f, (float)g_width/g_height, 0.1f, 500.0f);

    glMatrixMode(GL_MODELVIEW);
  //  glLoadIdentity();
    ModelTrans.loadIdentity();
    SetView();

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
}

void DrawHotBar() {
}

void DrawSelection() {
   printf("Drawing Selection\n");
  //SetupCube(0, 0, 0, 5, 0, 40, 40, 40);
   SetupCube(0, 0, 0, 5, 0, 1, 1, 1);
   SetupCube(0, 0, 0, 5, 0, 0.2, 0.2, 0.2);
   //SetupSquare(0, 0, 5, 1, 1);

}

void DrawGui(int editMode) {

   ready2D();

   printf("%d\n", editMode);

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
