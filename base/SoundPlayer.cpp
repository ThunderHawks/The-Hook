#include "sound.h"

Sound bMusic = Sound("../Assets/Sounds/Bastion_From_Wharfs_To_Wilds.ogg", 75, true);

void PlayBackground() {
	bMusic.playSound();
}

void SetBackground(char *path) {
	bMusic = Sound(path, 75, true);
	bMusic.playSound();
}
void pauseBackground() {
	bMusic.pause();
}
