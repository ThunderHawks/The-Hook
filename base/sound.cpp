#include "sound.h"

using namespace irrklang;
using namespace std;

/**************************
 CONSTRUCTORS
 *************************/
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
	setVolume(volume);
}

Sound::Sound(char* file, int volume, bool loop) {
	engine = createIrrKlangDevice();

	if(!engine) {
		cout << "Error: Could not create sound engine" << endl;
		exit(0);
	}

	fileName = file;
	position = 0;
	volume = volume;
	music = engine->play2D(fileName, loop, true, true);
 
	setLoop(loop);
	setVolume(volume);
}

/**************************
 PRIVATE FUNCTIONS
 *************************/
void Sound::setMute() {
	engine->setSoundVolume(0);
}

void Sound::unmute() {
	if(volume == 0)
		setVolume(50);
	else
		setVolume(volume);
}

void Sound::setVolume(int vol) {
	volume = min(max(vol, 0), 100);

	engine->setSoundVolume((double)volume/100);
}

/**************************
 PUBLIC FUNCTIONS
 *************************/

/**************************
 SETTER/GETTER FUNCTIONS
 *************************/
void Sound::setMusicPath(char* path) {
	fileName = path;
}

char* Sound::getMusicPath() {
	return fileName;
}

void Sound::setPosition(int pos) {
	position = pos;
}

int Sound::getPosition() {
	return position;
}

void Sound::setLoop(bool loop) {
	music->setIsLooped(loop);
}

bool Sound::getLoop() {
	return music->isLooped();
}

bool Sound::isPaused() {
	return music->getIsPaused();
}

bool Sound::isPlaying() {
	return engine->isCurrentlyPlaying(fileName);
}

/**************************
 PLAY FUNCTIONS
 *************************/
bool Sound::playMusic() {
	music = engine->play2D(fileName, getLoop(), false, true);

	if(!music) {
		cout << "Error: Could not play music" << endl;
		return false;
	}

	return true;
}

bool Sound::playMusic(char* path, bool loop) {
	fileName = path;
	setLoop(loop);

	music = engine->play2D(path, loop, false, true);

	if(!music) {
		cout << "Error: Could not play music" << endl;
		return false;
	}

	return true;
}

void Sound::playSFX(char* path) {
	engine->play2D(path);
}

/**************************
 VOLUME CONTROL FUNCTIONS
 *************************/
void Sound::increaseVolume() {
	volume = min(max(volume+5, 0), 100);

	setVolume(volume);
}

void Sound::decreaseVolume() {
	volume = min(max(volume-5, 0), 100);

	setVolume(volume);
}

void Sound::muteAll() {
	if(volume > 0 && engine->getSoundVolume() == 0)
		unmute();
	else
		setMute();
}

/**************************
 OTHER FUNCTIONS
 *************************/
void Sound::pause() {
	engine->setAllSoundsPaused(!isPaused());
}

int Sound::deleteSound() {
	music->stop();
	engine->drop();
	music->drop();
	return 0;
}
