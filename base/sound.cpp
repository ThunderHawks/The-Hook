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
	music = engine->play2D(path, loop, false, true);
	setLoop(loop);

	if(!music) {
		cout << "Error: Could not play music" << endl;
		return false;
	}

	return true;
}

void Sound::playSFX(char* path) {
	engine->play2D(path);
}

void Sound::play3DSFX(char* path, float charX, float charY, float charZ, float grappleX, float grappleY, float grappleZ) {
	glm::vec3 gaze = GetLookAt() - GetEye();
	vec3df grapplePosition(grappleX, grappleY, grappleZ);

	vec3df position(charX, charY, charZ);    			// position of the listener
	vec3df lookDirection(gaze.x, gaze.y, gaze.z); 	// the direction the listener looks into
	vec3df velPerSecond(0,0,0);    						// only relevant for doppler effects
 	vec3df upVector(0,1,0);        						// where 'up' is in your 3D scene

 	engine->setListenerPosition(position, lookDirection, velPerSecond, upVector);

	ISound * snd = engine->play3D(path, grapplePosition, false, true);
	snd->setMinDistance(30.0f); // a loud sound
   snd->setIsPaused(false); 	 // unpause the sound
	snd->drop(); 					 //garbage clean up
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

void Sound::muteBGM() {
	if(!isPaused())
		music->setIsPaused(true);
	else
		music->setIsPaused(false);
}

/**************************
 OTHER FUNCTIONS
 *************************/
void Sound::pause() {
	engine->setAllSoundsPaused(!isPaused());
}

void Sound::stopSong() {
	music->stop();
	music->drop();
}

int Sound::deleteSound() {
	music->stop();
	engine->drop();
	music->drop();
	return 0;
}
