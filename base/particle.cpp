#include "particle.h"
#include "Lab4.h"
#include "Mesh.h"
#include "InitHelpers.h"
#include "Shapes.h"
//#include "glm/glm.hpp"
//#include "mesh_loader.h"
//#include "InitHelpers.h"
//#include <GL/glut.h>
//#include <GL/glew.h>
/*struct part{
   void move(float step, part* thing);
   void create(float step, part* thing);
   void destroy(float step, part* thing);

   glm::mat4 (*getPos)();
   float scatter;
   int amount;
   float transp;
   int seed;

   int posBuff;
   glm::vec3[] pos;

   int norBuff;
   glm::vec3[] normal;

   int sizVbo;
   float[] size;

   glm::vec3[] velocity;
glm::vec3[] ambientColor;
}*/
void drawPart(part* thing){
      //
      SetModel(0,0,0,1,1,1,0);
      glBindBuffer(GL_ARRAY_BUFFER, thing->norBuff);
      float* data = (float*)glMapBuffer(GL_ARRAY_BUFFER,GL_READ_WRITE);
      for(int i=0;i<thing->amount;i++){
         vec3 tmp = glm::normalize(thing->pos[i]);
         data[i*3+0] = tmp.x;
         data[i*3+1] = tmp.y;
         data[i*3+2] = tmp.z;
      }
      glUnmapBuffer(GL_ARRAY_BUFFER);
   
      SetMaterial(0);
      //set transforms to idents
      safe_glEnableVertexAttribArray(h_aPosition);
      glBindBuffer(GL_ARRAY_BUFFER, CubeBuffObj);
      safe_glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

      safe_glEnableVertexAttribArray(h_aNormal);
      glBindBuffer(GL_ARRAY_BUFFER, NormalBuffObj);
      safe_glVertexAttribPointer(h_aNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

      //draw
      glBindBuffer(GL_ARRAY_BUFFER, thing->posBuff);
      glDrawElements(GL_POINTS, thing->amount, GL_UNSIGNED_SHORT, 0);

}
void moveDust(float step, part*  thing){
  for(int i=0;i<thing->amount;i++){
    if(thing->active[i]){
      thing->pos[i] += thing->velocity[i];
      thing->velocity[i] *= .95;
      thing->size[i] = rand()%2?thing->size[i]*1.1:thing->size[i]*.9;
    }
  }
}
void createDust(float step, part*  thing){
  srand(thing->seed);
  //dont do anything third party will handle
  thing->seed = rand();
}

void destroyDust(float step, part*  thing){
  srand(thing->seed);
  for(int i=0;i<thing->amount;i++){
    if(rand()*1.0/RAND_MAX>.97){
      thing->active[i] = 0;
    }
  }
  thing->seed = rand();
}
part* createDustPart(int max,float scatter,glm::mat4 (*getPos)()){
   part* thing = (part*)malloc(sizeof(part));
   thing->getPos = getPos;
   thing->pos = (glm::vec3*) calloc(max,sizeof(glm::vec3));
   thing->velocity = (glm::vec3*) calloc(max,sizeof(glm::vec3));
   thing->ambientColor = (glm::vec3*) calloc(max,sizeof(glm::vec3));
   thing->normal = (glm::vec3*) calloc(max,sizeof(glm::vec3));
   thing->size = (float*) calloc(max,sizeof(float));
   thing->move = &(moveDust);
   thing->create = &(createDust);
   thing->destroy = &(destroyDust);
   thing->amount = max;
   thing->scatter = scatter;

   thing->seed = rand();
   srand(thing->seed);
   //position vbo object

   glGenBuffers(1,&(thing->posBuff));
   glGenBuffers(1,&(thing->norBuff));
   glGenBuffers(1,&(thing->sizBuff));

   for(int i=0;i<max;i++){
      glm::vec4 tmp = thing->getPos()*glm::vec4(0,0,0,1);
      thing->pos[i] = glm::vec3(tmp[0],tmp[1],tmp[2]);
      thing->velocity[i] = glm::vec3(cos(i*2*3.14159/max),sin(rand()*scatter/RAND_MAX),sin(i*2*3.14159/max));
   }

   glBindBuffer(GL_ARRAY_BUFFER, thing->posBuff);
      printf("here %d\n",glGetError());
   glBufferData(GL_ARRAY_BUFFER, thing->amount*(sizeof(GLfloat)*3), NULL,GL_DYNAMIC_DRAW);//4 not 3?
}
void destroyDustPart(part* thing){
  //unbind things on gpu?
  free(thing->pos);
  free(thing->velocity);
  free(thing->active);
  free(thing->ambientColor);
  free(thing->normal);
  free(thing->size);
  free(thing);
}
