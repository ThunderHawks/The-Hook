#include "SoundPlayer.h"

using namespace std;

Sound bMusic = Sound("../Assets/Sounds/cityMain.mp3", 75, true);
bool isMuted = false;

SoundPlayer::SoundPlayer() {
	BGM = Sound();
}

char* SoundPlayer::getTrackPath(int trackNum) {
	return playList[trackNum].track;
}

int SoundPlayer::getTrackId(char* trackFile) {
	for(int i = 0; i < playList.size(); i++)
		if(strcmp(trackFile, playList[i].track) == 0)
			return i;

	return -1;
}

void SoundPlayer::CreatePlayList(char* musicFile) {
	ParsePlayList(musicFile);

	if(playList.size() > 0)
		BGM.playMusic(getTrackPath(0), true);
}

void SoundPlayer::ParsePlayList(char* musicFile) {
	ifstream mFile(musicFile);
	string line;
	char* trackPath;

	while(getline(mFile, line)) {
		trackPath = new char[line.length()+1];
		strcpy(trackPath, line.c_str());
		playList.push_back(Track(playList.size(), trackPath));
	}
}

void SoundPlayer::nextSong() {
	int trackNum = getTrackId(BGM.getMusicPath()) + 1;

	if(trackNum >= playList.size())
		trackNum = 0;

	BGM.stopSong();
cout << playList.size() << endl;
	BGM.playMusic(getTrackPath(trackNum), true);

}

void SoundPlayer::prevSong() {
	int trackNum = getTrackId(BGM.getMusicPath()) - 1;

	if(trackNum < 0)
		trackNum = playList.size() - 1;

	BGM.stopSong();
	BGM.playMusic(getTrackPath(trackNum), true);
}

void SoundPlayer::muteAll() {
	BGM.muteAll();
}
/******************************************************************************/

void PlayBackground() {
	bMusic.playMusic();
}

void SetBackground(char *path) {
	bMusic = Sound(path, 75, true);
	bMusic.playMusic();
}
void pauseBackground() {
	bMusic.pause();
}

/*Mutes Everything, sound FXdon't play*/
void MuteAll() {
	isMuted = !isMuted;
	bMusic.muteAll();
}
/*Changes the Background music the song number. Returns the song number*/
int SwapSong(int songNum) {
	return 0;
}
/*increment the song number. returns the new song number*/
int NextSong() {
	return 0;
}
/*decrement the song number. Returns the new song number*/
int PrevSong() {
	return 0;
}
/* Input: Please Use a Macro to determine which sound effect is playing. The macros are listed above
 *Summary: Plays the sound effect that corresponds to the macro if it is not muted.*/
void PlayFX(int fx) {
	if (!isMuted) {
		switch(fx % MAXSOUNDS) {
			case COLLISION_FX:
				bMusic.playSFX("../Assets/Sounds/Fistpunch.mp3");
				break;
				
			case THROW_GRAP_FX:
				bMusic.playSFX("../Assets/Sounds/HookShot.mp3");
				break;
				
			case GRAP_CONNECT_FX:
				bMusic.playSFX("../Assets/Sounds/Hooks_Wall_Hit.mp3");
				break;
				
			case SUCCESS_FX:
				bMusic.playSFX("../Assets/Sounds/YAY.mp3");
				break;
				
			case FAIL_FX:
				bMusic.playSFX("../Assets/Sounds/Dont_fail_me_again.mp3");
				break;
				
			case JUMP_FX:
				bMusic.playSFX("../Assets/Sounds/Jump.wav");
				break;
		}
	}
}
