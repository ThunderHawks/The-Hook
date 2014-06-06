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
      thing->age++;
      thing->move(1.0,thing);
      glPointSize(10);
      //printf("draw a part\n");
      //
      SetModel(0,0,0,1,1,1,0);
      SetMaterial(thing->mat);//7
      //SetMaterial(18);//7
      glBindBuffer(GL_ARRAY_BUFFER, thing->posBuff);
      float* pointdata = (float*)glMapBuffer(GL_ARRAY_BUFFER,GL_READ_WRITE);
      for(int i=0;i<thing->amount;i++){
         //printf("orig %f %f %f\n",data[i],data[i+1],data[i+2]);
         //printf("%f %f %f is pos of e part\n",thing->pos[i].x,thing->pos[i].x,thing->pos[i].x);
         vec3 tmp = thing->pos[i];
         pointdata[i*3+0] = tmp.x;
         pointdata[i*3+1] = tmp.y;
         pointdata[i*3+2] = tmp.z;
         //printf("now  %f %f %f\n",data[i],data[i+1],data[i+2]);
      }
      glUnmapBuffer(GL_ARRAY_BUFFER);

      glBindBuffer(GL_ARRAY_BUFFER, thing->sizBuff);
      float* sizedata = (float*)glMapBuffer(GL_ARRAY_BUFFER,GL_READ_WRITE);
      for(int i=0;i<thing->amount;i++){
         //printf("psize is %f\n",sizedata[i]);
         sizedata[i] = thing->size[i];
      }
      glUnmapBuffer(GL_ARRAY_BUFFER);

      //set transforms to idents
      safe_glEnableVertexAttribArray(h_aPosition);
      glBindBuffer(GL_ARRAY_BUFFER, thing->posBuff);
      safe_glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

      safe_glEnableVertexAttribArray(h_aNormal);
      glBindBuffer(GL_ARRAY_BUFFER, thing->norBuff);
      safe_glVertexAttribPointer(h_aNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

      safe_glEnableVertexAttribArray(h_aPointSize);
      glBindBuffer(GL_ARRAY_BUFFER, thing->sizBuff);
      safe_glVertexAttribPointer(h_aPointSize, 1, GL_FLOAT, GL_FALSE, 0, 0);

      //printf("the buff is h_aPointSize %d\n",h_aPointSize);

      //draw
      glBindBuffer(GL_ARRAY_BUFFER, thing->posBuff);
      //printf("the amt is %d\n",thing->amount);
      glDrawArrays(GL_POINTS,0, thing->amount);

}
void moveDust(float step, part*  thing){
  //printf("%d thing is \n",thing);
  for(int i=0;i<thing->amount;i++){
   //printf("%d is active\n",thing->active[i]);
    if(thing->active[i]){
      thing->pos[i] += thing->velocity[i];
      thing->velocity[i] *= .95;
      thing->velocity[i].y -= .05;
      //thing->pos[i].y = thing->pos[i].y>0?thing->pos[i].y:0;
      thing->size[i] = rand()%2?thing->size[i]:thing->size[i]-1;
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
part* createDustPart(int max,float scatter,glm::vec3 getPos){
   part* thing = (part*)malloc(sizeof(part));
   thing->getPos = getPos;
   thing->pos = (glm::vec3*) calloc(max,sizeof(glm::vec3));
   thing->velocity = (glm::vec3*) calloc(max,sizeof(glm::vec3));
   thing->ambientColor = (glm::vec3*) calloc(max,sizeof(glm::vec3));
   thing->normal = (glm::vec3*) calloc(max,sizeof(glm::vec3));
   thing->size = (float*) calloc(max,sizeof(float));
   thing->active = (int*) calloc(max,sizeof(int));
   thing->move = &(moveDust);
   thing->create = &(createDust);
   thing->destroy = &(destroyDust);
   thing->amount = max;
   thing->scatter = scatter;
   thing->age = 0;
   thing->mat = 18;
   thing->seed = rand();
   srand(thing->seed);
   //position vbo object
   printf("%d %d %d  then",thing->posBuff,thing->norBuff,thing->sizBuff);
   glGenBuffers(1,&(thing->posBuff));
   glGenBuffers(1,&(thing->norBuff));
   glGenBuffers(1,&(thing->sizBuff));
   printf("%d %d %d\n",thing->posBuff,thing->norBuff,thing->sizBuff);
   printf("startinit\n");
   for(int i=0;i<max;i++){
      //printf("a\n");
      thing->pos[i] = thing->getPos;//+glm::vec3(rand()*1.0/RAND_MAX,rand()*1.0/RAND_MAX,rand()*1.0/RAND_MAX);
//      thing->pos[i] = thing->getPos;
      //printf("b\n");
      thing->velocity[i] = glm::normalize(glm::vec3((rand()*2.0/RAND_MAX)-1,1,(rand()*2.0/RAND_MAX)-1));
      thing->velocity[i].y = rand()*scatter/RAND_MAX;
      thing->velocity[i] *= .2;
      //printf("c\n");
      thing->active[i] = 1;
      thing->size[i] = 10;
      //printf("d\n");
   }
   printf("ending\n");

   glBindBuffer(GL_ARRAY_BUFFER, thing->posBuff);
   glBufferData(GL_ARRAY_BUFFER, thing->amount*(sizeof(GLfloat)*3), NULL,GL_DYNAMIC_DRAW);//4 not 3?
   glBindBuffer(GL_ARRAY_BUFFER, thing->norBuff);
   glBufferData(GL_ARRAY_BUFFER, thing->amount*(sizeof(GLfloat)*3), NULL,GL_DYNAMIC_DRAW);//4 not 3?
   glBindBuffer(GL_ARRAY_BUFFER, thing->sizBuff);
   glBufferData(GL_ARRAY_BUFFER, thing->amount*(sizeof(GLfloat)), NULL,GL_DYNAMIC_DRAW);//4 not 3?
   printf("wut\n");
   return thing;
}
void destroyDustPart(part* thing){
  //unbind things on gpu?
  free(thing->pos);
  /*The line below causes segfaults. Free invalid pointer*/
  free(thing->velocity);
  free(thing->active);
  free(thing->ambientColor);
  free(thing->normal);
  free(thing->size);
  free(thing);
}

void moveMoney(float step, part*  thing){
  //printf("%d thing is \n",thing);
  for(int i=0;i<thing->amount;i++){
   //printf("%d is active\n",thing->active[i]);
    if(thing->active[i]){
      thing->velocity[i].x*= (thing->velocity[i].y+1)*1.01;
      thing->velocity[i].z*= (thing->velocity[i].y+1)*1.01;
      thing->pos[i] += thing->velocity[i];
      
//      thing->velocity[i] *= .95;
      thing->velocity[i].y -= .01;
      //thing->pos[i].y = thing->pos[i].y>0?thing->pos[i].y:0;
    }
  }
}
void createMoney(float step, part*  thing){
  srand(thing->seed);
  //dont do anything third party will handle
  thing->seed = rand();
}

void destroyMoney(float step, part*  thing){
  srand(thing->seed);
  for(int i=0;i<thing->amount;i++){
    if(rand()*1.0/RAND_MAX>.97){
      thing->active[i] = 0;
    }
  }
  thing->seed = rand();
}
part* createMoneyPart(int max,float scatter,glm::vec3 getPos){
   part* thing = (part*)malloc(sizeof(part));
   thing->getPos = getPos;
   thing->pos = (glm::vec3*) calloc(max,sizeof(glm::vec3));
   thing->velocity = (glm::vec3*) calloc(max,sizeof(glm::vec3));
   thing->ambientColor = (glm::vec3*) calloc(max,sizeof(glm::vec3));
   thing->normal = (glm::vec3*) calloc(max,sizeof(glm::vec3));
   thing->size = (float*) calloc(max,sizeof(float));
   thing->active = (int*) calloc(max,sizeof(int));
   thing->move = &(moveMoney);
   thing->create = &(createMoney);
   thing->destroy = &(destroyMoney);
   thing->amount = max;
   thing->scatter = scatter;
   thing->age = 0;
   thing->mat = 19;
   thing->seed = rand();
   srand(thing->seed);
   //position vbo object
   printf("%d %d %d  then",thing->posBuff,thing->norBuff,thing->sizBuff);
   glGenBuffers(1,&(thing->posBuff));
   glGenBuffers(1,&(thing->norBuff));
   glGenBuffers(1,&(thing->sizBuff));
   printf("%d %d %d\n",thing->posBuff,thing->norBuff,thing->sizBuff);
   printf("startinit\n");
   for(int i=0;i<max;i++){
      //printf("a\n");
//      thing->pos[i] = thing->getPos+glm::vec3(rand()*1.0/RAND_MAX,rand()*1.0/RAND_MAX,rand()*1.0/RAND_MAX);
      thing->pos[i] = thing->getPos;
      //printf("b\n");
      thing->velocity[i] = glm::normalize(glm::vec3((rand()*2.0/RAND_MAX)-1,1,(rand()*2.0/RAND_MAX)-1));
      thing->velocity[i] *= .2;
      thing->velocity[i].y = rand()*scatter/RAND_MAX;
      //printf("c\n");
      thing->active[i] = 1;
      thing->size[i] = 25;
      //printf("d\n");
   }
   printf("ending\n");

   glBindBuffer(GL_ARRAY_BUFFER, thing->posBuff);
   glBufferData(GL_ARRAY_BUFFER, thing->amount*(sizeof(GLfloat)*3), NULL,GL_DYNAMIC_DRAW);//4 not 3?
   glBindBuffer(GL_ARRAY_BUFFER, thing->norBuff);
   glBufferData(GL_ARRAY_BUFFER, thing->amount*(sizeof(GLfloat)*3), NULL,GL_DYNAMIC_DRAW);//4 not 3?
   glBindBuffer(GL_ARRAY_BUFFER, thing->sizBuff);
   glBufferData(GL_ARRAY_BUFFER, thing->amount*(sizeof(GLfloat)), NULL,GL_DYNAMIC_DRAW);//4 not 3?
   printf("wut\n");
   return thing;
}
/*void destroyMoneyPart(part* thing){
  //unbind things on gpu?
  free(thing->pos);
  free(thing->velocity);
  free(thing->active);
  free(thing->ambientColor);
  free(thing->normal);
  free(thing->size);
  free(thing);
}*/
