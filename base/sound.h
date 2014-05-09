#ifndef SOUNDPLAYER_H_
#define SOUNDPLAYER_H_

#include <iostream>
#include <irrKlang.h>
#include <string.h>
#include <cstdlib>

class Sound {

	private:
		irrklang::ISoundEngine* engine;
		irrklang::ISound* music;
		int position;

		//Pauses sound
		void setPause();

		//Resumes sound
		void resume();

	public:
		//The path to the sound file (ex: ../Sounds/Bastion_Wharfs_To_Wilds.ogg)
		char* fileName;
		int volume; //The volume (0-100): 0 - mute, 100 - max
		bool loop; //Whether sound loops: true - loops, false - once through

		Sound(); //default constructor
		Sound(char* fileName, int volume, bool isLooped); //custom constructor

		//Plays a sound (use this if you created a Sound object with the custom constructor)
		void playSound();

		/* Plays a sound
		 * path - the path to sound file (ex: ../Sounds/Bastion_Wharfs_To_Wilds.ogg)
		 * loop - whether or not sound is looped (set to true for looping)
		 */
		void playSound(char* path, bool isLoop);

		//Either pauses or resumes the sound
		void pause();

		//Deletes a sound object
		int deleteSound();

};

#endif 
