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

GLuint textures[50];


bool debtMode = true;
bool displayedVictory = false; 
int debt = 100000;
int savings = 0;
//Used to keep track of addition in score
int lastScore = 0;
vector<int> fpsTextures;
vector<int> digitTextures;
//Where updated textures are stored until dance
//of that digit is completed
vector<int> updatedDigitTextures;
//Allows the score text to dance!
bool danceScore = false;
bool goingUp;
vector<glm::vec2> scoreDigitPosition;
int digitDanceIndex;

//Clock
clock_t t;

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
         //Rounded building
         icon.entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.2, 0.2, 0.2), 0.0, 19);
         break;
      case 20:
         //Cafe
         icon.entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.3, 0.3, 0.3), 0.0, 20);
         break;
      case 21:
         //Mailbox
         icon.entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.4, 0.4, 0.4), 0.0, 21);
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
   glGenTextures(1, textures + 0);
   glGenTextures(1, textures + 1);

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
   LoadTexture((char *)"../Assets/Textures/controls.bmp", textures[31]);
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
   LoadTexture((char *)"../Assets/Fonts/collegeLoans.bmp", textures[45]);
   LoadTexture((char *)"../Assets/Textures/winScreen.bmp", textures[46]);
   LoadTexture((char *)"../Assets/Fonts/savings.bmp", textures[47]);
   LoadTexture((char *)"../Assets/Fonts/$.bmp", textures[48]);
   LoadTexture((char *)"../Assets/Textures/fps.bmp", textures[49]);

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
   SSIndices.push_back(createIcon(19, textures[2], 15.0, glm::vec2(-0.45, 0.1)));
   SSIndices.push_back(createIcon(20, textures[2], 15.0, glm::vec2(-0.3, 0.1)));
   SSIndices.push_back(createIcon(21, textures[2], 8.0, glm::vec2(-0.15, 0.1)));
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

   //Textures for score
   digitTextures.push_back(textures[34]);
   digitTextures.push_back(textures[33]);
   digitTextures.push_back(textures[33]);
   digitTextures.push_back(textures[33]);
   digitTextures.push_back(textures[33]);
   digitTextures.push_back(textures[33]);


   updatedDigitTextures.push_back(textures[33]);
   updatedDigitTextures.push_back(textures[33]);
   updatedDigitTextures.push_back(textures[33]);
   updatedDigitTextures.push_back(textures[33]);
   updatedDigitTextures.push_back(textures[33]);
   updatedDigitTextures.push_back(textures[33]);
   //Textures for fps
   fpsTextures.push_back(textures[33]);
   fpsTextures.push_back(textures[39]);
   //Clock
   t = clock();
   //Positions for score text
   scoreDigitPosition.push_back(glm::vec2(p2i_x(g_width) - 0.46, p2i_y(g_height) - 0.5));
   scoreDigitPosition.push_back(glm::vec2(p2i_x(g_width) - 0.41, p2i_y(g_height) - 0.5));
   scoreDigitPosition.push_back(glm::vec2(p2i_x(g_width) - 0.36, p2i_y(g_height) - 0.5));
   scoreDigitPosition.push_back(glm::vec2(p2i_x(g_width) - 0.31, p2i_y(g_height) - 0.5));
   scoreDigitPosition.push_back(glm::vec2(p2i_x(g_width) - 0.26, p2i_y(g_height) - 0.5));
   scoreDigitPosition.push_back(glm::vec2(p2i_x(g_width) - 0.21, p2i_y(g_height) - 0.5));

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
   glUniform1f(h_uTextMode, 1);

   if(grappleInRange(-dir.x, -dir.y, -dir.z)) {
      SetupSq(0, 0, textures[44], 0.05, 0.05);
   }
   else {
      SetupSq(0, 0, textures[43], 0.05, 0.05);
   }
   glUniform1f(h_uTextMode, 0);
}



void fpsDigitTextures() {
   int fpsTemp;
   int fps = (int)getFPS();

   for(int i = 0; i < 2; i++) {
      fpsTemp = fps%10;
      fps /= 10;

      switch(fpsTemp) {
         case 0:
            fpsTextures[i] = textures[33];
            break;
         case 1:
            fpsTextures[i] = textures[34];
            break;
         case 2:
            fpsTextures[i] = textures[35];
            break;
         case 3:
            fpsTextures[i] = textures[36];
            break;
         case 4:
            fpsTextures[i] = textures[37];
            break;
         case 5:
            fpsTextures[i] = textures[38];
            break;
         case 6:
            fpsTextures[i] = textures[39];
            break;
         case 7:
            fpsTextures[i] = textures[40];
            break;
         case 8:
            fpsTextures[i] = textures[41];
            break;
         case 9:
            fpsTextures[i] = textures[42];
            break;
      }
   }
}

void DrawFPS() {

   //Don't update too often
   if((clock() - t)/CLOCKS_PER_SEC > 0.5) {
      fpsDigitTextures();
      t = clock();
   }

   glUniform1f(h_uTextMode, 1.0);
   //Draw the fps background
   SetupSq(-1.0 * p2i_x(g_width) + 0.1, -1.0 * p2i_y(g_height) + 0.2, textures[49], 0.2, 0.4);
   //SetupColoredSq(-1.0 * p2i_x(g_width) + 0.1, -1.0 * p2i_y(g_height) + 0.2, glm::vec3(0.0, 0.0, 0.0), 0.2, 0.4);

   //Draw fps digits
   SetupSq(-1.0 * p2i_x(g_width) + 0.07, -1.0 * p2i_y(g_height) + 0.15, fpsTextures[1], DIGIT_WIDTH, DIGIT_HEIGHT);
   SetupSq(-1.0 * p2i_x(g_width) + 0.12, -1.0 * p2i_y(g_height) + 0.15,  fpsTextures[0], DIGIT_WIDTH, DIGIT_HEIGHT);

   glUniform1f(h_uTextMode, 0);
}

void DrawVictory() {
   glUniform1f(h_uTextMode, 1.1);

   //Draw the victory screen
   SetupSq(0, 0, textures[46], 1.0, 1.0);

   glUniform1f(h_uTextMode, 0);
}

void scoreDigitTextures() {
   int debtTemp, digitTemp;

   //Show debt
   if(debtMode == true) {
      //Debt to display
      debtTemp = debt - getPoints()/10;

      //If reached zero
      if(debtTemp <= 0) {
         //Make no lower than zero
         debtTemp = 0;
         debt = 0;
         //Display victory
         debtMode = false;
      }
   }
   //Show savings
   else if(debtMode == false) {
      debtTemp = savings + getPoints()/10 - 100000;
      //debtTemp = savings;
      if(debtTemp >= 999999) {
         debtTemp = 999999;
      }
   }

   //Convert debt number into an array of ints
   for(int i = 5; i >= 0; i--) {
      digitTemp = debtTemp%10;
      debtTemp /= 10;

      switch(digitTemp) {
         case 0:
            updatedDigitTextures[i] = textures[33];
            break;
         case 1:
            //printf("%d to 1\n", i);
            updatedDigitTextures[i] = textures[34];
            break;
         case 2:
            //printf("%d to 2\n", i);
            updatedDigitTextures[i] = textures[35];
            break;
         case 3:
            //printf("%d to 3\n", i);
            updatedDigitTextures[i] = textures[36];
            break;
         case 4:
            //printf("%d to 4\n", i);
            updatedDigitTextures[i] = textures[37];
            break;
         case 5:
            //printf("%d to 5\n", i);
            updatedDigitTextures[i] = textures[38];
            break;
         case 6:
            //printf("%d to 6\n", i);
            updatedDigitTextures[i] = textures[39];
            break;
         case 7:
            //printf("%d to 7\n", i);
            updatedDigitTextures[i] = textures[40];
            break;
         case 8:
            //printf("%d to 8\n", i);
            updatedDigitTextures[i] = textures[41];
            break;
         case 9:
            //printf("%d to 9\n", i);
            updatedDigitTextures[i] = textures[42];
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
}

void initScoreDance() {
   lastScore = getPoints();
   danceScore = true;
   digitDanceIndex = 0;
   goingUp = true;
}

void DrawScore() {

   //Only call scoreDigitTextures if the score has changed
   if(getPoints() != lastScore) {
      scoreDigitTextures();
      initScoreDance();
   }

   //Background

   float debtTemp = debt - getPoints()/10;
   float normalizeDebt = debtTemp/debt;
   //printf("debtTemp: %f\n", debtTemp);
   //Black border
   SetupColoredSq(p2i_x(g_width) - 0.3, p2i_y(g_height) - 0.35,  glm::vec3(0.0, 0.0, 0.0), 0.51, 0.515);
   //Background
   SetupColoredSq(p2i_x(g_width) - 0.3, p2i_y(g_height) - 0.35,  glm::vec3(normalizeDebt, 1.0 - normalizeDebt, 0.1), 0.5, 0.5);

   glUniform1f(h_uTextMode, 1);

   //Draw "College Loan"
   if(debtMode == true) {
      SetupSq(p2i_x(g_width) - 0.3, p2i_y(g_height) - 0.3,  textures[45], 0.5, 0.4);
   }
   //Draw "Savings"
   else {
      SetupSq(p2i_x(g_width) - 0.3, p2i_y(g_height) - 0.3,  textures[47], 0.5, 0.4);
   }

   //SetupColoredSq(0, 0.35, glm::vec3(1.0, 0.2, 0.2), 1.4, 1.1);


   //scoreDigitPosition.push_back(glm::vec2(p2i_x(g_width) - 0.51, p2i_y(g_height) - 0.5));
   
   SetupSq(p2i_x(g_width) - 0.51, p2i_y(g_height) - 0.5,  textures[48], DIGIT_WIDTH, DIGIT_HEIGHT);
   SetupSq(scoreDigitPosition[0].x, scoreDigitPosition[0].y,  digitTextures[0], DIGIT_WIDTH, DIGIT_HEIGHT);
   SetupSq(scoreDigitPosition[1].x, scoreDigitPosition[1].y,  digitTextures[1], DIGIT_WIDTH, DIGIT_HEIGHT);
   SetupSq(scoreDigitPosition[2].x, scoreDigitPosition[2].y,  digitTextures[2], DIGIT_WIDTH, DIGIT_HEIGHT);
   SetupSq(scoreDigitPosition[3].x, scoreDigitPosition[3].y,  digitTextures[3], DIGIT_WIDTH, DIGIT_HEIGHT);
   SetupSq(scoreDigitPosition[4].x, scoreDigitPosition[4].y,  digitTextures[4], DIGIT_WIDTH, DIGIT_HEIGHT);
   SetupSq(scoreDigitPosition[5].x, scoreDigitPosition[5].y,  digitTextures[5], DIGIT_WIDTH, DIGIT_HEIGHT); 
   glUniform1f(h_uTextMode, 0);
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
               //printf("button %d pressed\n", i);
               switch(StartScreen1[i].ID) {
                  case(PLAY_BUTTON):
                     StartScreenShowing = 2;
                     break;
                  case(EDIT_BUTTON):
                     StartScreenShowing = 3;
                     break;
                  case(QUIT_BUTTON):
                     //printf("exiting\n");
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
      //printf("Gui Pressed Edit mode\n");
      //printf("Tried to press button at %d %d\n", xPos, yPos);

      //Test HB icons
      for(int i = 0; i < HBIndices.size(); i++) {

         if(iconPressed(HBIndices[i].position, xPos, yPos) == true) {
            //printf("~~~~Icon %d Pressed on hotbar\n", i);
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
            //printf("~~~~Icon %d Pressed on selection\n", i);
            lastSelectedIcon = SSIndices[i];
            iconSelected = true;
            return; 
         }
       }
   }
}

void AnimationStep() {
   if(danceScore == true) {
      glm::vec2 temp = scoreDigitPosition[digitDanceIndex];

      if(goingUp == true) {
         //If dancing digit is less than the height limit
         if(temp.y < 0.6) {
            //Move up slightly
            scoreDigitPosition[digitDanceIndex] = glm::vec2(temp.x, temp.y + 0.025);
         }
         //Else at height limit so go back down
         else {
            goingUp = false;
            //Update texture at that position
            digitTextures[digitDanceIndex] = updatedDigitTextures[digitDanceIndex];
            scoreDigitPosition[digitDanceIndex] = glm::vec2(temp.x, temp.y - 0.025);
         }
      }
      else {
         //If dancing digit is greater than the height limit
         if(temp.y - 0.025 >= 0.5) {
            //Move up slightly
            scoreDigitPosition[digitDanceIndex] = glm::vec2(temp.x, temp.y - 0.025);
         }
         //Else at height limit so stop with this index
         else {
            goingUp = true;
            //If last dance index
            if(digitDanceIndex == 5) {
               //Stop dance
               danceScore = false;
            }
            //Else continue dance
            else {
               digitDanceIndex++;
            }
         }
      }
   }
}

void DrawGui(int mode) {
   
   ready2D();

   AnimationStep();

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

      DrawScore();
      DrawFPS();

      //Draw the victory screen if debt is just paid off
      if(displayedVictory == false && debtMode == false) {
         DrawVictory();

         //Turn off victory screen if V is pressed to continue
         if(getGPressed('V')) {
            displayedVictory = true;
         }
      }
      //Else draw help screen if 'H' is held
      else if(getGPressed('H')) {
         DrawGameControls();
      }
      //Else draw Crosshair
      else {
         DrawCrosshair();
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
