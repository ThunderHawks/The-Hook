#ifndef PARTICLE
#define PARTICLE
#include "glm/glm.hpp"

struct part{
  void (*move)(float step, part* thing);
  void (*create)(float step, part* thing);
  void (*destroy)(float step, part* thing);

  glm::vec3 getPos;
  float scatter;
  int amount;
  float transp;
  int seed;
  int mat;

  unsigned int posBuff;
  glm::vec3 *pos;

  unsigned int norBuff;
  glm::vec3 *normal;

  unsigned int sizBuff;
  float *size;

  int *active;
  glm::vec3 *velocity;
  glm::vec3 *ambientColor;
  int age;
};
void drawPart(part* thing);
void moveDust(float step, part*  thing);
void createDust(float step, part*  thing);
void destroyDust(float step, part*  thing);
part* createDustPart(int max,float scatter,glm::vec3 getPos);
void destroyDustPart(part* thing);

void moveMoney(float step, part*  thing);
void createMoney(float step, part*  thing);

void destroyMoney(float step, part*  thing);
part* createMoneyPart(int max,float scatter,glm::vec3 getPos);
#endif
