#include "sound.h"
#include <iostream>
#include <string.h>
#include <stdio.h>

using namespace irrklang;
using namespace std;

int main() {
	std::cout << "Starting program...";
	char* song = "./Bastion_From_Wharfs_To_Wilds.ogg";
	Sound sound = Sound(song, 75, true);
	sound.playSound();

	char i;

	do
	{
		printf("Press 'q' to quit, 'p' to pause\n");

		if(i == 'p')
			sound.pause();

	} while((i = getchar()) != 'q');

	sound.deleteSound();

	return 0;
}
