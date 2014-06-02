#ifndef SOUND_H
#define SOUND_H
#include <iostream>
#include <irrKlang.h>
#include <string.h>
#include <cstdlib>
#include <algorithm>

class Sound {

	private:
		irrklang::ISoundEngine* engine;
		irrklang::ISound* music;
		char* fileName; //The path to the sound file (ex: ../Sounds/Bastion_Wharfs_To_Wilds.ogg)
		int position; //The sound file's play position in milliseconds
		int volume; //The volume

		/**************************
		 PRIVATE FUNCTIONS
		 *************************/
		//Mutes the sound (but still playing in the background)
		void setMute();

		//Unmutes sound (and sets it to what it was previously, or default 50)
		void unmute();

	public:

		/**************************
		 CONSTRUCTORS
		 *************************/
		Sound(); //Default constructor
		Sound(char* fileName, int volume, bool isLooped); //Custom constructor

		/**************************
		 SETTER/GETTER FUNCTIONS
		 *************************/
		//Set the path to the BGM
		void setMusicPath(char* path);

		//Get the path to the BGM
		char* getMusicPath();

		//Set the sound file's play position
		void setPosition(int pos);

		//Get the sound file's play position
		int getPosition();

		//Set whether the BGM is looped or not
		void setLoop(bool loop);

		//Get whether the BGM is looped or not
		bool getLoop();

		//Get whether the BGM is paused or not
		bool isPaused();

		//Get whether the BGM is playing or not
		bool isPlaying();

		/**************************
		 PLAY FUNCTIONS
		 *************************/
		//Plays a song (use this if you created a Sound object with the custom constructor)
		//Return Value: true if successfully played sound
		bool playMusic();

		/* Plays a song
		 * path - the path to sound file (ex: ../Sounds/Bastion_Wharfs_To_Wilds.ogg)
		 * loop - whether or not sound is looped (set to true for looping)
		 * 
		 * Return Value: true if successfully played sound
		 */
		bool playMusic(char* path, bool loop);

		//Plays a sound effect
		void playSFX(char* path);
		
		// Plays a 3D sound effect
		void play3DSFX(char* path, float x, float y, float z);

		/**************************
		 VOLUME CONTROL FUNCTIONS
		 *************************/
		//Set volume levels (stops at 0 [mute] or 100 [max] automatically)
		void setVolume(int vol);

		//Increases volume by 5
		void increaseVolume();

		//Decreases volume by 5
		void decreaseVolume();

		//Either mutes or unmutes all sounds (volume = 0)
		void muteAll();

		/**************************
		 OTHER FUNCTIONS
		 *************************/
		//Either pauses or resumes the BGM (SFX still works)
		void pause();

		void stopSong();

		//Deletes a sound object
		int deleteSound();

};

#endif


