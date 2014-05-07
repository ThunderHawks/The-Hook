GLEW = -lGLEW
BULLET = -I/usr/local/include/bullet -lBulletDynamics -lBulletCollision -lLinearMath
LIBS = -I/usr/include/assimp -lassimp -lglfw3 -lGLU -lGLEW -lGL -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lrt -I/usr/local/include/bullet -I/usr/include/bullet -lBulletDynamics -lBulletCollision -lLinearMath
OBJECT = Lab4.o Camera.o GLSL_helper.o Helper.o InitHelpers.o IO.o level.o Mesh.o mesh_loader.o MStackHelp.o physSystem.o Shapes.o ViewFrustum.o

all: $(OBJECT)
	g++ $(OBJECT) $(LIBS)

%.o: %.cpp
	g++ -c $< $(LIBS)

%.cpp: %.h
	touch $@

orig: 
	g++ *.cpp -I/usr/include/assimp -lassimp -lglfw3 -lGLU -lGLEW -lGL -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lrt -I/usr/local/include/bullet -I/usr/include/bullet -lBulletDynamics -lBulletCollision -lLinearMath

warnings:
	g++ *.cpp -I/usr/include/assimp -lassimp -lglfw3 -lGLU -lGLEW -lGL -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lrt -I/usr/local/include/bullet -I/usr/include/bullet -lBulletDynamics -lBulletCollision -lLinearMath -Wall

pkgconfig:
	g++ *.cpp `pkg-config --libs --static --cflags glu glfw3` $(GLEW) $(BULLET) 
	
clean:
	rm -f a.out *.o *.gch *~
