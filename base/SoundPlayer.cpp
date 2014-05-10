#include "sound.h"

Sound bMusic = Sound("../Assets/Sounds/Bastion_From_Wharfs_To_Wilds.ogg", 75, true);

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
