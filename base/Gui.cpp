#include "Gui.h"
//#include "Helper.h"

TexImage *TextureImage;
RGB myImage[64][64];
RGB* g_pixel;

//Location of each HotBar Icon Index
vector<Icon> HBIndices;

//Location of each Selection Screen Icon Index
vector<Icon> SSIndices;

//Location of start Start Screen Buttons
vector<Button> StartScreen1;
vector<Button> StartScreen2;

//Indicates which start screen is active
//1 - StartScreen 1
//2 - StartScreen 2 (Play)
//3 - StartScreen 3 (Edit)
int StartScreenShowing = 1;

//Last selection index pressed
bool iconSelected = false;
Icon lastSelectedIcon;

GLuint textures[30];
//FT_Library library;
//FT_Face face;
int debt = 200000;

Button createButton(int textureIndex, glm::vec2 position, int ID) {
   Button button;
   
   button.position = position;
   button.textureIndex = textureIndex;
   button.ID = ID;
   return button;
}

//Icon createIcon(Entity ent, float distance, int texIndex, glm::vec2 pos) {
Icon createIcon(int meshIndex, int textureIndex, float distance, glm::vec2 pos) {
   Icon icon;

   //Set icon's entity
   switch(meshIndex) {
      case 0:
         //Basic Blg
         icon.entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.3, 0.3, 0.3), 0.0, 0);
         break;
      case 1:
         //Shop Bldg
         icon.entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.25, 0.25, 0.25), 0.0, 1);
         break;
      case 2:
         //Cinderblock
         icon.entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0), 0.0, 2);
         break;
      case 3:
         //Medium Blg
         icon.entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.1, 0.1, 0.1), 0.0, 3);
         break;
      case 4:
         //Side walk corner
         icon.entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0), 0.0, 4);
         break;
      case 5:
         //SideWalk
         icon.entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0), 0.0, 5);
         break;
      case 6:
         //Street light
         icon.entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.5, 0.5, 0.5), 0.0, 6);
         break;
      case 7:
         //Table
         icon.entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.3, 0.3, 0.3), 0.0, 7);
         break;
      case 8:
         //Tall blg
         icon.entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.4, 0.4, 0.4), 0.0, 8);
         break;
      case 9:
         //Water tower
         icon.entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.2, 0.2, 0.2), 0.0, 9);
         break;
      case 10:
         //Record Building
         icon.entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.2, 0.2, 0.2), 0.0, 10);
         break;
      case 11:
         //Wall
         icon.entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.2, 0.2, 0.2), 0.0, 11);
         break;
      case 12:
         //Gas Station
         icon.entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.3, 0.3, 0.3), 0.0, 12);
         break;
      case 13:
         //Half slab
         icon.entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0), 0.0, 13);
         break;
      case 14:
         //Mart
         icon.entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.3, 0.3, 0.3), 0.0, 14);
         break;
      case 15:
         //Flag
         icon.entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.2, 0.2, 0.2), 0.0, 15);
         break;
      case 16:
         //AsymbBldg
         icon.entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.1, 0.1, 0.1), 0.0, 16);
         break;
      case 17:
         //PointyBldg
         icon.entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.1, 0.1, 0.1), 0.0, 17);
         break;
      case 18:
         //Destination Beacon
         icon.entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.05, 0.01, 0.05), 0.0, 18);
         break;
      case 19:
         //Selection tool
         icon.entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.01, 0.01, 0.01), 0.0, 19);
         break;
   }
   icon.lookAtDistance = distance;
   icon.textureIndex = textureIndex;
   icon.position = pos;
   return icon;
}

void initGui() {

   Icon iconTemp;

      //Initialize Freetype library
      /*int error = FT_Init_FreeType(&library);
      if(error) {
         printf("Error: An error has occurred initializing the FREETYPE library\n");
      }
      //Initialize Freetype New Face
      error = FT_New_Face(library, "/usr/share/fonts/truetype/arial.ttf", 0, &face );
      if(error == FT_Err_Unknown_File_Format) {
         printf("Error: Font file could be opened and read but unsupported format\n");
      }
      else if(error) {
         printf("Error: Font file could not be opened and read or broken\n");
      }
*/

   glGenTextures(50, textures);
   glGenTextures(1, textures + 1);

   printf("here\n");
   LoadTexture((char *)"../Assets/Textures/ModMBasicBldg.bmp", textures[0]);
   LoadTexture((char *)"../Assets/Textures/shopBldg.bmp", textures[1]);
   LoadTexture((char *)"../Assets/Textures/cinderblock.bmp", textures[2]);
   LoadTexture((char *)"../Assets/Textures/mediumBasicBuilding.bmp", textures[3]);
   LoadTexture((char *)"../Assets/Textures/sidewalkCorner.bmp", textures[4]);
   LoadTexture((char *)"../Assets/Textures/sidewalk.bmp", textures[5]);
   LoadTexture((char *)"../Assets/Textures/streetlight.bmp", textures[6]);
   LoadTexture((char *)"../Assets/Textures/table.bmp", textures[7]);
   LoadTexture((char *)"../Assets/Textures/tallBldg.bmp", textures[8]);
   LoadTexture((char *)"../Assets/Textures/waterTower.bmp", textures[9]);
   LoadTexture((char *)"../Assets/Textures/simpleCRBldg.bmp", textures[10]);
   LoadTexture((char *)"../Assets/Textures/wall.bmp", textures[11]);
   LoadTexture((char *)"../Assets/Textures/gasStation.bmp", textures[12]); 
   LoadTexture((char *)"../Assets/Textures/halfSlab.bmp", textures[13]);
   LoadTexture((char *)"../Assets/Textures/mart.bmp", textures[14]);
   LoadTexture((char *)"../Assets/Textures/flag.bmp", textures[15]);
   LoadTexture((char *)"../Assets/Textures/asymBldg.bmp", textures[16]);
   LoadTexture((char *)"../Assets/Textures/pointyBldg.bmp", textures[17]);
   LoadTexture((char *)"../Assets/Textures/SelectionScreenTexture.bmp", textures[18]);
   LoadTexture((char *)"../Assets/Textures/SelectionUI_converted.bmp", textures[19]);
   LoadTexture((char *)"../Assets/Textures/crackedTexture.bmp", textures[20]);
   LoadTexture((char *)"../Assets/Textures/caution.bmp", textures[21]);
   LoadTexture((char *)"../Assets/Fonts/font1.bmp", textures[22]);
   LoadTexture((char *)"../Assets/Textures/playButton.bmp", textures[23]);   
   LoadTexture((char *)"../Assets/Textures/editButton.bmp", textures[24]);
   LoadTexture((char *)"../Assets/Textures/quitButton.bmp", textures[25]);
   LoadTexture((char *)"../Assets/Textures/world1Button.bmp", textures[26]);
   LoadTexture((char *)"../Assets/Textures/world2Button.bmp", textures[27]);
   LoadTexture((char *)"../Assets/Textures/world3Button.bmp", textures[28]);
   LoadTexture((char *)"../Assets/Textures/world4Button.bmp", textures[29]);
   LoadTexture((char *)"../Assets/Textures/backButton.bmp", textures[30]);
   LoadTexture((char *)"../Assets/Textures/gameInstructions1.0.bmp", textures[31]);
   LoadTexture((char *)"../Assets/Textures/ScoreBackground.bmp", textures[32]);
   LoadTexture((char *)"../Assets/Fonts/0.bmp", textures[33]);
   LoadTexture((char *)"../Assets/Fonts/1.bmp", textures[34]);
   LoadTexture((char *)"../Assets/Fonts/2.bmp", textures[35]);
   LoadTexture((char *)"../Assets/Fonts/3.bmp", textures[36]);
   LoadTexture((char *)"../Assets/Fonts/4.bmp", textures[37]);
   LoadTexture((char *)"../Assets/Fonts/5.bmp", textures[38]);
   LoadTexture((char *)"../Assets/Fonts/6.bmp", textures[39]);
   LoadTexture((char *)"../Assets/Fonts/7.bmp", textures[40]);
   LoadTexture((char *)"../Assets/Fonts/8.bmp", textures[41]);
   LoadTexture((char *)"../Assets/Fonts/9.bmp", textures[42]);
   LoadTexture((char *)"../Assets/Textures/redCrossHair.bmp", textures[43]);
   LoadTexture((char *)"../Assets/Textures/greenCrossHair.bmp", textures[44]);
   

   //Initialize HotBar icons
   HBIndices.push_back(createIcon(0, textures[0], 23.5, glm::vec2(-0.6, -0.88)));
   HBIndices.push_back(createIcon(1, textures[1], 12.5, glm::vec2(-0.45, -0.88)));
   HBIndices.push_back(createIcon(2, textures[2], 3.5, glm::vec2(-0.3, -0.88)));
   HBIndices.push_back(createIcon(3, textures[3], 8.0, glm::vec2(-0.15, -0.88)));
   HBIndices.push_back(createIcon(4, textures[4], 8.0, glm::vec2(0.0, -0.88)));
   HBIndices.push_back(createIcon(5, textures[5], 8.0, glm::vec2(0.15, -0.88)));
   HBIndices.push_back(createIcon(6, textures[6], 11.5, glm::vec2(0.3, -0.88)));
   HBIndices.push_back(createIcon(7, textures[7], 5.0, glm::vec2(0.45, -0.88)));
   HBIndices.push_back(createIcon(8, textures[8], 500.0, glm::vec2(0.6, -0.88)));

   //Initialize Selection screen icons
   //Row 1
   SSIndices.push_back(createIcon(0, textures[0], 23.5, glm::vec2(-0.6, 0.7)));
   SSIndices.push_back(createIcon(1, textures[1], 12.5, glm::vec2(-0.45, 0.7)));
   SSIndices.push_back(createIcon(2, textures[2], 3.5, glm::vec2(-0.3, 0.7)));
   SSIndices.push_back(createIcon(3, textures[3], 8.0, glm::vec2(-0.15, 0.7)));
   SSIndices.push_back(createIcon(4, textures[4], 8.0, glm::vec2(0.0, 0.7)));
   SSIndices.push_back(createIcon(5, textures[5], 8.0, glm::vec2(0.15, 0.7)));
   SSIndices.push_back(createIcon(6, textures[6], 11.5, glm::vec2(0.3, 0.7)));
   SSIndices.push_back(createIcon(7, textures[7], 5.0, glm::vec2(0.45, 0.7)));
   SSIndices.push_back(createIcon(8, textures[8], 500.0, glm::vec2(0.6, 0.7)));
   //Row 2
   SSIndices.push_back(createIcon(9, textures[9], 8.0, glm::vec2(-0.6, 0.4)));
   SSIndices.push_back(createIcon(10, textures[10], 35.6, glm::vec2(-0.45, 0.4)));
   SSIndices.push_back(createIcon(11, textures[11], 5.8, glm::vec2(-0.3, 0.4)));
   SSIndices.push_back(createIcon(12, textures[12], 18.0, glm::vec2(-0.15, 0.4)));
   SSIndices.push_back(createIcon(13, textures[13], 8.0, glm::vec2(0.0, 0.4)));
   SSIndices.push_back(createIcon(14, textures[14], 22.5, glm::vec2(0.15, 0.4)));
   SSIndices.push_back(createIcon(15, textures[15], 4.1, glm::vec2(0.3, 0.4)));
   SSIndices.push_back(createIcon(16, textures[16], 54.7, glm::vec2(0.45, 0.4)));
   SSIndices.push_back(createIcon(17, textures[17], 70.0, glm::vec2(0.6, 0.4)));
   //Row 3 
   SSIndices.push_back(createIcon(18, textures[16], 8.0, glm::vec2(-0.6, 0.1)));
   SSIndices.push_back(createIcon(19, textures[16], 8.0, glm::vec2(-0.45, 0.1)));
   SSIndices.push_back(createIcon(0, textures[21], 8.0, glm::vec2(-0.3, 0.1)));
   SSIndices.push_back(createIcon(0, textures[21], 8.0, glm::vec2(-0.15, 0.1)));
   SSIndices.push_back(createIcon(0, textures[21], 8.0, glm::vec2(0.0, 0.1)));
   SSIndices.push_back(createIcon(0, textures[21], 8.0, glm::vec2(0.15, 0.1)));
   SSIndices.push_back(createIcon(0, textures[21], 8.0, glm::vec2(0.3, 0.1)));
   SSIndices.push_back(createIcon(0, textures[21], 8.0, glm::vec2(0.45, 0.1)));
   SSIndices.push_back(createIcon(0, textures[21], 8.0, glm::vec2(0.6, 0.1)));

   //Initialize Start Screen buttons
   //Screen 1
   StartScreen1.push_back(createButton(textures[23], glm::vec2(0.7, 0.4), PLAY_BUTTON));
   StartScreen1.push_back(createButton(textures[24], glm::vec2(0.7, 0.0), EDIT_BUTTON));
   StartScreen1.push_back(createButton(textures[25], glm::vec2(0.7, -0.4), QUIT_BUTTON));

   //Screen 2
   StartScreen2.push_back(createButton(textures[26], glm::vec2(0.7, 0.8), WORLD1_SELECT_BUTTON));
   StartScreen2.push_back(createButton(textures[27], glm::vec2(0.7, 0.4), WORLD2_SELECT_BUTTON));
   StartScreen2.push_back(createButton(textures[28], glm::vec2(0.7, 0.0), WORLD3_SELECT_BUTTON));
   StartScreen2.push_back(createButton(textures[29], glm::vec2(0.7, -0.4), WORLD4_SELECT_BUTTON));
   StartScreen2.push_back(createButton(textures[30], glm::vec2(0.7, -0.8), BACK_BUTTON));

      //Row 4*/
/*
      SSIndices.push_back(createIcon(40, 8.0, glm::vec2(-0.6, -0.2)));
      SSIndices.push_back(createIcon(40, 8.0, glm::vec2(-0.45, -0.2)));
      SSIndices.push_back(createIcon(40, 8.0, glm::vec2(-0.3, -0.2)));
      SSIndices.push_back(createIcon(40, 8.0, glm::vec2(-0.15, -0.2)));
      SSIndices.push_back(createIcon(40, 8.0, glm::vec2(0.0, -0.2)));
      SSIndices.push_back(createIcon(40, 8.0, glm::vec2(0.15, -0.2)));
      SSIndices.push_back(createIcon(40, 8.0, glm::vec2(0.3, -0.2)));
      SSIndices.push_back(createIcon(40, 8.0, glm::vec2(0.45, -0.2)));
      SSIndices.push_back(createIcon(40, 8.0, glm::vec2(0.6, -0.2)));
*/
      //For the time being
   glEnable(GL_TEXTURE_2D);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

   makeCheckerBoard(64, 64);
}

Icon getHBIcon(int index) {
   return HBIndices[index - 1];
}

void DrawCrosshair() {
   glm::vec3 dir = GetLookAt()- GetEye();

   if(grappleInRange(-dir.x, -dir.y, -dir.z)) {
      SetupSq(0, 0, textures[44], 0.05, 0.05);
   }
   else {
      SetupSq(0, 0, textures[43], 0.1, 0.05);
   }
}

vector<int> scoreDigitTextures() {
   vector<int> digitTextures;
   int array[6];
   int debtTemp = debt - getPoints()/10;
   int digitTemp;

   //Convert debt number into an array of ints
   for(int i = 5; i >= 0; i--) {
      digitTemp = debtTemp%10;
      debtTemp /= 10;

      switch(digitTemp) {
         case 0:
            digitTextures.push_back(textures[33]);
            break;
         case 1:
            digitTextures.push_back(textures[34]);
            break;
         case 2:
            digitTextures.push_back(textures[35]);
            break;
         case 3:
            digitTextures.push_back(textures[36]);
            break;
         case 4:
            digitTextures.push_back(textures[37]);
            break;
         case 5:
            digitTextures.push_back(textures[38]);
            break;
         case 6:
            digitTextures.push_back(textures[39]);
            break;
         case 7:
            digitTextures.push_back(textures[40]);
            break;
         case 8:
            digitTextures.push_back(textures[41]);
            break;
         case 9:
            digitTextures.push_back(textures[42]);
            break;

      }
   }
   
  // printf("%d%d%d,%d%d%d\n", array[0], array[1], array[2], array[3], array[4], array[5]);
  // printf("hi?\n");
//   printf("%d\n", array[0]);
/*
   //Convert array of ints into array of digit textures
   for(int i = 5; i >= 0; i--) {
      printf("i: %d\n", i);
      switch(array[i]) {
         case 0: 
            digitTextures[i] = textures[33];
            break;
         case 1: 
            digitTextures[i] = textures[34];
            break;
         case 2: 
            digitTextures[i] = textures[35];
            break;
         case 3: 
            digitTextures[i] = textures[36];
            break;
         case 4: 
            digitTextures[i] = textures[37];
            break;
         case 5: 
            digitTextures[i] = textures[38];
            break;
         case 6: 
            digitTextures[i] = textures[39];
            break;
         case 7: 
            digitTextures[i] = textures[40];
            break;
         case 8: 
            digitTextures[i] = textures[41];
            break;
         case 9: 
            digitTextures[i] = textures[42];
            break;
      }
   }*/
   return digitTextures;
}

void DrawScore() {
   vector<int> digitTextures = scoreDigitTextures();
   //Background
   SetupSq(p2i_x(g_width) - 0.3, p2i_y(g_height) - 0.3,  textures[32], 0.5, 0.4);

   SetupSq(p2i_x(g_width) - 0.46, p2i_y(g_height) - 0.365,  digitTextures[5], DIGIT_WIDTH, DIGIT_HEIGHT);
   SetupSq(p2i_x(g_width) - 0.41, p2i_y(g_height) - 0.365,  digitTextures[4], DIGIT_WIDTH, DIGIT_HEIGHT);
   SetupSq(p2i_x(g_width) - 0.36, p2i_y(g_height) - 0.365,  digitTextures[3], DIGIT_WIDTH, DIGIT_HEIGHT);
   SetupSq(p2i_x(g_width) - 0.31, p2i_y(g_height) - 0.365,  digitTextures[2], DIGIT_WIDTH, DIGIT_HEIGHT);
   SetupSq(p2i_x(g_width) - 0.26, p2i_y(g_height) - 0.365,  digitTextures[1], DIGIT_WIDTH, DIGIT_HEIGHT);
   SetupSq(p2i_x(g_width) - 0.21, p2i_y(g_height) - 0.365,  digitTextures[0], DIGIT_WIDTH, DIGIT_HEIGHT);  
}

void ready2D() {
   glUniform1f(h_uGuiMode, 1);
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
    glUniform1f(h_uGuiMode, 0);

    glViewport(0, 0, g_width, g_height);
    glMatrixMode(GL_PROJECTION);

    ModelTrans.loadIdentity();
    safe_glUniformMatrix4fv(h_uProjMatrix, glm::value_ptr(glm::perspective(80.0f, (float)g_width/g_height, 0.1f, 500.0f)));

    glMatrixMode(GL_MODELVIEW);
    ModelTrans.loadIdentity();
    //SetView();

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
}

void DrawGameControls() {
   SetupSq(0, 0, textures[31], 1.4, 1.4);
}

void DrawStartScreen() {
   Button buttonTemp;

   //printf("Drawing Start Screen\n");
   //printf("Screen: %d\n", StartScreenShowing);

   //Background
   SetupSq(0, 0, textures[31], p2w_x(g_width), p2w_y(g_height) * 2.0);

   if(StartScreenShowing == 1) {
      //printf("SS1: %d\n", (int)StartScreen1.size());
      for(int i = 0; i < StartScreen1.size(); i++) {
         buttonTemp = StartScreen1[i];
         SetupSq(buttonTemp.position.x, buttonTemp.position.y, buttonTemp.textureIndex, BUTTON_WIDTH, BUTTON_HEIGHT);
      }
   }
   if(StartScreenShowing == 2 || StartScreenShowing == 3) {
      //printf("SS2: %d\n", (int)StartScreen2.size());
      for(int i = 0; i < StartScreen2.size(); i++) {
         buttonTemp = StartScreen2[i];
         SetupSq(buttonTemp.position.x, buttonTemp.position.y, buttonTemp.textureIndex, BUTTON_WIDTH, BUTTON_HEIGHT);
      }
   }
}

void DrawHotBar() {
   Icon iconTemp;

   //Draw background
   SetupSq(0, -0.9, textures[20], 1.4, 0.3);

   //Draw Icons
   for(int i = 0; i < HBIndices.size(); i++) {
      iconTemp = HBIndices[i];
      SetupSq(iconTemp.position.x, iconTemp.position.y, iconTemp.textureIndex, ICON_WIDTH, ICON_HEIGHT);      
   }
}

void DrawSelection() {
   Icon iconTemp;

   //Draw background
   SetupSq(0, 0.35, textures[20], 1.4, 1.1);

   //Draw icons
   for(int i = 0; i < SSIndices.size(); i++) {
      iconTemp = SSIndices[i];
      SetupSq(iconTemp.position.x, iconTemp.position.y, iconTemp.textureIndex, ICON_WIDTH, ICON_HEIGHT); ;      
   }
}

bool buttonPressed(glm::vec2 buttonPos, int xPos, int yPos) {
   float worldX = p2i_x(xPos);
   float worldY = p2i_y(yPos);

   if(buttonPos.x - BUTTON_WIDTH/2.0 <= worldX && worldX <= buttonPos.x + BUTTON_WIDTH/2.0) {
      if(buttonPos.y - BUTTON_HEIGHT/2.0 <= worldY && worldY <= buttonPos.y + BUTTON_HEIGHT/2.0) {
         PlayFX(BUTTON_FX);
         return true;
      }
   }

   return false;
}

bool iconPressed(glm::vec2 iconPos, int xPos, int yPos) {

   float worldX = p2i_x(xPos);
   float worldY = p2i_y(yPos);

   if(iconPos.x - ICON_WIDTH/2.0 <= worldX && worldX <= iconPos.x + ICON_WIDTH/2.0) {
      if(iconPos.y - ICON_HEIGHT/2.0 <= worldY && worldY <= iconPos.y + ICON_HEIGHT/2.0) {
         return true;
      }
   }

   return false;
}

void SetHBIcon(Icon icon, int HBIndex) {
   //Save HB position
   glm::vec2 posTemp = HBIndices[HBIndex].position;
   //Set icon over HB icon
   HBIndices[HBIndex] = icon;
   //Restore HB position
   HBIndices[HBIndex].position = posTemp;
}

void GuiPressing(int mode, int xPos, int yPos) {
   if(mode == STARTSCREEN_MODE) {
      //Screen with only Play, Edit, and Quit
      if(StartScreenShowing == 1) {
         //printf("Gui Pressed StartScreen mode\n");
         for(int i = 0; i < (int)StartScreen1.size(); i++) {
            if(buttonPressed(StartScreen1[i].position, xPos, yPos) == true) {
               printf("button %d pressed\n", i);
               switch(StartScreen1[i].ID) {
                  case(PLAY_BUTTON):
                     StartScreenShowing = 2;
                     break;
                  case(EDIT_BUTTON):
                     StartScreenShowing = 3;
                     break;
                  case(QUIT_BUTTON):
                     printf("exiting\n");
                     //exit( EXIT_SUCCESS );
                     glfwDestroyWindow(window);
                     break;
               }
               return;
            }
         }
      }
      //Screen with all worlds in Play
      else if(StartScreenShowing == 2) {
         //printf("Gui Pressed StartScreen mode\n");
         for(int i = 0; i < (int)StartScreen2.size(); i++) {
            if(buttonPressed(StartScreen2[i].position, xPos, yPos) == true) {

               switch(StartScreen2[i].ID) {
                  case(WORLD1_SELECT_BUTTON):
                     initPlay("level1.wub");
                     break;
                  case(WORLD2_SELECT_BUTTON):
                     initPlay("level2.wub");
                     break;
                  case(WORLD3_SELECT_BUTTON):
                     initPlay("level3.wub");
                     break;
                  case(WORLD4_SELECT_BUTTON):
                     initPlay("level4.wub");
                     break;
                  case(BACK_BUTTON):
                     StartScreenShowing = 1;
                     break;
               }
               return;
            }
         }
      }
      //Screen with all worlds in Edit
      else if(StartScreenShowing == 3) {
         for(int i = 0; i < (int)StartScreen2.size(); i++) {
            if(buttonPressed(StartScreen2[i].position, xPos, yPos) == true) {
               switch(StartScreen2[i].ID) {
                  case(WORLD1_SELECT_BUTTON):
                     initEdit("level1.wub");
                     break;
                  case(WORLD2_SELECT_BUTTON):
                     initEdit("level2.wub");
                     break;
                  case(WORLD3_SELECT_BUTTON):
                     initEdit("level3.wub");
                     break;
                  case(WORLD4_SELECT_BUTTON):
                     initEdit("level4.wub");
                     break;
                  case(BACK_BUTTON):
                     StartScreenShowing = 1;
                     break;
               }
               return;
            }
         }
      }
   }
   else if(mode == EDIT_MODE) {
      printf("Gui Pressed Edit mode\n");
      //printf("Tried to press button at %d %d\n", xPos, yPos);

      //Test HB icons
      for(int i = 0; i < HBIndices.size(); i++) {

         if(iconPressed(HBIndices[i].position, xPos, yPos) == true) {
            printf("~~~~Icon %d Pressed on hotbar\n", i);
            if(iconSelected == true) {
               SetHBIcon(lastSelectedIcon, i);
               iconSelected = false;
            }
            return;
         }
      }

      //Test SS icons
      for(int i = 0; i < SSIndices.size(); i++) {
         if(iconPressed(SSIndices[i].position, xPos, yPos) == true) {
            printf("~~~~Icon %d Pressed on selection\n", i);
            lastSelectedIcon = SSIndices[i];
            iconSelected = true;
            return; 
         }
       }
   }
}

void DrawGui(int mode) {
   
   ready2D();

   //printf("Mode; %d\n", mode);
   //If start screen mode
   if(mode == -1) {
      //printf("%f %f\n", g_width, g_height);
      //printf("Drawing start screen\n");
      //printf("%f %f\n", p2w_x(g_width), p2w_y(g_height));
      DrawStartScreen();
   }
   //Else if edit mode
   else if(mode) {
      DrawHotBar();
      //Display gui with all of the models to edit hotbar
      if(getEPressed('G')) {
         DrawSelection();
      }
   }
   //Else game mode
   else {
      DrawCrosshair();
      DrawScore();

      if(getGPressed('H')) {
         DrawGameControls();
      }
      //printText2D(string text, int x, int y, int size)
      //printText2D("Corn Flakes", 0, 0, 1.0);
   }

   //printf("%d\n", (int)getPoints()/10);

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
  //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); /*  cheap scaling when image bigger than texture */
  //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); /*  cheap scaling when image smalled than texture*/

  // ... nice trilinear filtering.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
  glGenerateMipmapEXT(GL_TEXTURE_2D);
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

void makeCheckerBoard ( int nRows, int nCols )
{
  g_pixel = new RGB[nRows * nCols];
  int c;

  long count = 0;
  for ( int i=0; i < nRows; i++ ) {
    for ( int j=0; j < nCols; j++ ) {
      count = j*nCols +i;
      c = (((i/8) + (j/8)) %2) * 255;
      g_pixel[count].r = c;
      g_pixel[count].g = c;
      g_pixel[count].b = c;

    }
  }
  /* set up the checker board texture as well */
  glBindTexture(GL_TEXTURE_2D, 2);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, g_pixel);
}

/***************LET'S WRITE TEXT!!!!! :D ***************************************************************/
std::vector<glm::vec2> vertices;
std::vector<glm::vec2> UVs;
unsigned short idx[] = {0, 1, 2,
                        3, 1, 2,
                               };

void initText2D(const char * texturePath) {

}

void cleanupText2D() {
  glUniform1f(h_uTextMode, 0);
}

/*Made with help from http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-11-2d-text/ */
void printText2D(string text, int x, int y, int size) {
  char character;
  float uv_x, uv_y;
  GLuint vertex_Buffer;
  GLuint uvBuffer;
  GLuint IndexBuffer;

  //cleanupText2D();
  //initText2D("../Assets/Fonts/font1.bmp");
  glUniform1f(h_uTextMode, 1);
   //setup texture unit
 //glEnable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, textures[0]);

  for ( unsigned int i=0 ; i < text.size() ; i++ ){
    character = text[i] - 32;
    uv_x = (character%10)/10.0f;
    uv_y = (character/10)/10.0f;

    glm::vec2 vertex_up_left    = glm::vec2( x+i*size     , y+size );
    glm::vec2 vertex_up_right   = glm::vec2( x+i*size+size, y+size );
    glm::vec2 vertex_down_right = glm::vec2( x+i*size+size, y      );
    glm::vec2 vertex_down_left  = glm::vec2( x+i*size     , y      );

    vertices.push_back(vertex_up_left   );
    vertices.push_back(vertex_down_left );
    vertices.push_back(vertex_up_right  );

    vertices.push_back(vertex_down_right);
    vertices.push_back(vertex_up_right);
    vertices.push_back(vertex_down_left);

    glm::vec2 uv_up_left    = glm::vec2( uv_x           , 1.0f - uv_y );
    glm::vec2 uv_up_right   = glm::vec2( uv_x+1.0f/10.0f, 1.0f - uv_y );
    glm::vec2 uv_down_right = glm::vec2( uv_x+1.0f/10.0f, 1.0f - (uv_y + 1.0f/10.0f) );
    glm::vec2 uv_down_left  = glm::vec2( uv_x           , 1.0f - (uv_y + 1.0f/10.0f) );
//void initScore(glm::vec2 upLeft, glm::vec2 upRight, glm::vec2 downLeft, glm::vec2 downRight)
  //  initScore(uv_up_left, uv_up_right, uv_down_left, uv_down_right);
    //SetupScore(float x, float y, int texture, float scaleX, float scaleY) 
  //  SetupScore(0, 0, 3, 0.2, 0.2);

    UVs.push_back(uv_up_left   );
    UVs.push_back(uv_down_left );
    UVs.push_back(uv_up_right  );

    UVs.push_back(uv_down_right);
    UVs.push_back(uv_up_right  );
    UVs.push_back(uv_down_left );
  }

  glGenBuffers(1, &vertex_Buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_Buffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec2), &vertices.front(), GL_STATIC_DRAW);

  glGenBuffers(1, &uvBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
  glBufferData(GL_ARRAY_BUFFER, UVs.size()*sizeof(glm::vec2), &UVs.front(), GL_STATIC_DRAW);

  
  safe_glEnableVertexAttribArray(h_utexpos);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_Buffer);
  safe_glVertexAttribPointer(safe_glGetAttribLocation(ShadeProg, "textPos"), 2, GL_FLOAT, GL_FALSE, 0, 0);

  safe_glEnableVertexAttribArray(h_aTexCoord);
  glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
  safe_glVertexAttribPointer(h_aTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glGenBuffers(1, &IndexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);


  /* draw!*/
  glDrawElements(GL_TRIANGLES, vertices.size(), GL_UNSIGNED_SHORT, 0);

  //safe_glUniform1i(h_uTexUnit, 0);
  /* Disable the attributes used by our shader*/
  glDisable(GL_TEXTURE_2D);
  safe_glDisableVertexAttribArray(safe_glGetAttribLocation(ShadeProg, "textPos"));
  safe_glDisableVertexAttribArray(h_aTexCoord);

  cleanupText2D();
}
