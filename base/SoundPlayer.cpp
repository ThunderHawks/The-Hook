#include "SoundPlayer.h"
#include "sound.h"

Sound bMusic = Sound("../Assets/Sounds/Bastion_From_Wharfs_To_Wilds.ogg", 75, true);
int trackNumber = 0;
int trackCount = 0;
bool isMuted = false;

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
int SwapSong(int inc) {
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
