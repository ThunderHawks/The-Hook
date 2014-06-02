#include "camBox.h"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
/*#include glm
// this stuff is stupid guys dont try it at home, enter page at own risk (http://en.wikipedia.org/wiki/Change_of_basis)
struct camBox{
  float x,y,z;//center
  float h,w,d;//"radius"
  glm::vec3 ax;//rotatioion axis
  float amt;//rotation amount
}*/
vector<camBox*> boxes;
void* createCameraBox(float x,float y,float z,float h,float w,float d){//no rotation yet because no rotation in phys render (gota work on that)
  camBox* bx = (camBox*)malloc(sizeof(camBox));
  bx->x=x;
  bx->y=y;
  bx->z=z;
  
  bx->h=h;
  bx->w=w;
  bx->d=d;
  
  bx->ax = glm::vec3(0,0,0);
  bx->amt = 0;
  
  boxes.push_back(bx);
  return bx;
}
int pointBox(glm::vec3 vc, camBox* bx){
  glm::mat4 point = glm::mat4(0);
  glm::mat4 cent = glm::mat4(0);
  glm::vec3 cen;
  point[0][0] = vc.x;
  point[1][1] = vc.y;
  point[2][2] = vc.z;
  point[3][3] = 0;//this is the translation location, not used because point is bing changed in coordinate frames (vec3 rotation);
  
  cent[0][0] = bx->x;
  cent[1][1] = bx->y;
  cent[2][2] = bx->z;
  cent[3][3] = 0;
  
  if(bx->amt){//small optimization to reduce calcs if no rotation is applied
    point = glm::rotate(point,bx->amt,bx->ax);//rotate point so that it is in box rotation space
    cent = glm::rotate(cent,bx->amt,bx->ax);//rotate the box center from world to box rotation space
  }
  //int ret;//dont need this as the return is only 6 line (not the 32 for boxbox)
  glm::vec4 mult = glm::vec4(1,1,1,0);
  glm::vec4 vcc = point * mult;
  glm::vec4 cenn = cent * mult;
  vc.x = vcc.x;
  vc.y = vcc.y;
  vc.z = vcc.z;
  cen.x= cenn.x;
  cen.y = cenn.y;
  cen.z = cenn.z;
  
  return    (vc.x < cen.x+bx->w)&&//middle + width/2
            (vc.x > cen.x-bx->w)&&//middle - width/2
            
            (vc.y < cen.y+bx->h)&&
            (vc.y > cen.y-bx->h)&&
           
            (vc.z < cen.z+bx->d)&&
            (vc.z > cen.z-bx->d);
}
glm::vec3 rotatePoint(glm::vec3 pt, float amt, glm::vec3 ax){//rotates a glm vec3 (couldnt find docs for a similar function)
  glm::mat4 point = glm::mat4(0);
  point[0][0] = pt.x;
  point[1][1] = pt.y;
  point[2][2] = pt.z;
  point[3][3] = 0;
  glm::vec4 ret = glm::rotate(point,amt,ax)*glm::vec4(1,1,1,0);
  return glm::vec3(ret.x,ret.y,ret.z);
}
int BoxIntoBox(camBox* a, camBox* b){//check if a has points in b
  glm::vec3 aCenter =  rotatePoint(glm::vec3(a->x,a->y,a->z),a->amt,a->ax);//rotate to a-box space
  glm::vec3 testPoint = aCenter;
  int ret = 0;//box starts with assumed no collision
  testPoint.x+=a->w;//add width
  testPoint.y+=a->h;//height
  testPoint.z+=a->d;//depth
  testPoint = rotatePoint(glm::vec3(a->x,a->y,a->z),-a->amt,a->ax);//return to world space +++
  ret |= pointBox(testPoint,b);//test against b OR in the result, if any return true then box corner is colliding
  
  testPoint = aCenter;
  testPoint.x+=a->w;//add width
  testPoint.y+=a->h;//height
  testPoint.z-=a->d;//depth
  testPoint = rotatePoint(glm::vec3(a->x,a->y,a->z),-a->amt,a->ax);//return to world space ++-
  ret |= pointBox(testPoint,b);//test against b
  
  testPoint = aCenter;
  testPoint.x+=a->w;//add width
  testPoint.y-=a->h;//height
  testPoint.z+=a->d;//depth
  testPoint = rotatePoint(glm::vec3(a->x,a->y,a->z),-a->amt,a->ax);//return to world space +-+
  ret |= pointBox(testPoint,b);//test against b
  
  testPoint = aCenter;
  testPoint.x+=a->w;//add width
  testPoint.y-=a->h;//height
  testPoint.z-=a->d;//depth
  testPoint = rotatePoint(glm::vec3(a->x,a->y,a->z),-a->amt,a->ax);//return to world space +--
  ret |= pointBox(testPoint,b);//test against b
  
  testPoint = aCenter;
  testPoint.x-=a->w;//add width
  testPoint.y+=a->h;//height
  testPoint.z+=a->d;//depth
  testPoint = rotatePoint(glm::vec3(a->x,a->y,a->z),-a->amt,a->ax);//return to world space -++
  ret |= pointBox(testPoint,b);//test against b
  
  testPoint = aCenter;
  testPoint.x-=a->w;//add width
  testPoint.y+=a->h;//height
  testPoint.z-=a->d;//depth
  testPoint = rotatePoint(glm::vec3(a->x,a->y,a->z),-a->amt,a->ax);//return to world space -+-
  ret |= pointBox(testPoint,b);//test against b
  
  testPoint = aCenter;
  testPoint.x-=a->w;//add width
  testPoint.y-=a->h;//height
  testPoint.z+=a->d;//depth
  testPoint = rotatePoint(glm::vec3(a->x,a->y,a->z),-a->amt,a->ax);//return to world space --+
  ret |= pointBox(testPoint,b);//test against b
  
  testPoint = aCenter;
  testPoint.x-=a->w;//add width
  testPoint.y-=a->h;//height
  testPoint.z-=a->d;//depth
  testPoint = rotatePoint(glm::vec3(a->x,a->y,a->z),-a->amt,a->ax);//return to world space ---
  ret |= pointBox(testPoint,b);//test against b
}
int BoxAndBox(camBox* a,camBox* b){
  return BoxIntoBox(a,b) || BoxIntoBox(b,a);
}

int inRange(int num,float dist,glm::mat4 lookat){//request from mathew
   glm::vec4 start = glm::vec4(0,0,0,1);
   glm::vec4 step = glm::vec4(0,0,dist/num,0);

   int hit = 0;
   for(int i = 1; i < num; i++){
      glm::vec4 test4 = lookat*start+lookat*(step*i);
      glm::vec3 test;

      test[0] = test4[0];
      test[1] = test4[1];
      test[2] = test4[2];
      
      vector<Entity*> posible = pointLevelTest(test);
      for(int j=0;j<posible.size();j++){
         if (pointBox(test,(camBox*)posible[j]->physics)){
            return 1;
         }
      }
   }
   return 0;
}

float distLineToPoint(glm::mat4 line,glm::vec3 pt){
  return -1;
}
float distLineToLine(glm::mat4 a,glm::mat4 b){
  return -1;
}
int LineToBox(glm::mat4 line,camBox* bx){
  return 0;
}
