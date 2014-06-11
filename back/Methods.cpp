#include "Lab4.h"

//For positions of models
Node *head = NULL;

void Add_Entry(glm::vec3 Position) {
   Node *rtn = (Node *)malloc(sizeof(Node));

   //Data
   rtn->Position = Position;
   rtn->modelType = MODEL_TYPE_UNDEFINED;

   //Update table ptrs
   rtn->next = head;
   head = rtn;
}

int Table_Size() {
   Node *temp = head;
   int size = 0;

   while(temp != NULL) {
      size++;
      temp = temp->next;
   }
   return size;
}

void Set_ModelType(int index, int type) {
   Node *temp = head;
   int i = 0;

   while(i != index) {
      temp = temp->next;
      i++;
   }
   temp->modelType = type;   
}

Node Get_Index(int index) {
   Node *temp = head;
   Node temp2;
   int i = 0;

   while(i != index) {
      temp = temp->next;
      i++;
   }
   temp2 = *temp;
   return temp2;
}

void Free_LinkedList() {
   Node *temp;
   while(head != NULL) {
      temp = head->next;
      free(head);
      head = temp;
   }
}

float Magnitude(glm::vec3 vector) {
   return sqrt(pow(vector.x, 2.0) + pow(vector.y, 2.0) + pow(vector.z, 2.0));
}

int RandomInt(int one, int two) {
   return one + (rand() % (int)(two - one + 1));
}
