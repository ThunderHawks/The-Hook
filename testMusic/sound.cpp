#include "sound.h"

using namespace irrklang;
using namespace std;

Sound::Sound() {
	engine = createIrrKlangDevice();
	music = 0;

	if(!engine) {
		cout << "Error: Could not create sound engine" << endl;
		exit(0);
	}

	fileName = "/0";
	position = 0;
	volume = 50;
	engine->setSoundVolume(1);
}

Sound::Sound(char* file, int volume, bool isLooped) {
	engine = createIrrKlangDevice();
	music = 0;

	if(!engine) {
		cout << "Error: Could not create sound engine" << endl;
		exit(0);
	}

	fileName = file;
	position = 0;
	volume = volume;
	loop = isLooped;
	engine->setSoundVolume(1);
}

void Sound::setPause() {
	position = music->getPlayPosition();
	music->setIsPaused(true);
}

void Sound::resume() {
	if(music->getIsPaused()) {
		music = engine->play2D(fileName, true, false, true);

		if(position != -1)
			music->setPlayPosition(position);
	}
}

void Sound::playSound() {
	music = engine->play2D(fileName, loop, false, true);
}

void Sound::playSound(char* path, bool isLoop) {
	fileName = path;
	loop = isLoop;

	music = engine->play2D(path, loop, false, true);
}

void Sound::pause() {
	if(music->getIsPaused())
		resume();
	else
		setPause();
}

int Sound::deleteSound() {
	engine->drop();
	return 0;
}
