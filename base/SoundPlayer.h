#ifndef SOUNDPLAYER_H_
#define SOUNDPLAYER_H_

#include "sound.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

/*SOUND FX MACROS*/
#define COLLISION_FX 0			//for colliding with things
#define THROW_GRAP_FX 1			//for throwing the grappling hook
#define GRAP_CONNECT_FX 2		//for the grappling hook connecting with something
#define SUCCESS_FX 3				//for doing something good, like beating a mission
#define FAIL_FX 4					//You have disappointed me for the last time
#define JUMP_FX 5					//BOING!
#define BUTTON_FX 6

#define MAXSOUNDS 7

class SoundPlayer {
	private:
		struct Track {
      	Track()
      	{
      		trackId = 0;
				track = 0;
         }

			Track(int trackNum, char* trackPath)
			{
				trackId = trackNum;
				track = trackPath;
			}
        
			int trackId;
         char* track;
		};

		std::vector<Track> playList;

		/**************************
		 PRIVATE FUNCTIONS
		 *************************/
		//Creates a track
		Track MakeTrack(std::string line, int trackNum);

		//Parses playList.txt for music file locations
		void ParsePlayList(char* musicFile);

	public:
		Sound BGM;

		/**************************
		 CONSTRUCTORS
		 *************************/
		SoundPlayer();

		/**************************
		 SETTER/GETTER FUNCTIONS
		 *************************/
		//Returns the song file's path (ex: ../Assets/Sounds/cityMain.mp3)
		char* getTrackPath(int trackNum);

		//Returns the song's id
		int getTrackId(char* trackFile);

		/**************************
		 INITIALIZING FUNCTIONS
		 *************************/
		//Initializes BGM and fills the playlist with all songs in playList.txt
		void CreatePlayList(char* musicFile);

		/**************************
		 PLAY FUNCTIONS
		 *************************/
		//Plays the next song
		void nextSong();

		//Plays the previous song
		void prevSong();
		
		//Mutes everything; SFX don't play
		void muteAll();
		
		//Mutes the BGM; SFX plays
		void muteBGM();

};

/*Sets up the sound stuff, and plays the default song*/
void PlayBackground();
/*Sets up the sound stuff, and plays the song pointed to in the path. That song is set to song 0*/
void SetBackground(char *path);
/*Pause the background Music*/
void pauseBackground();
/*Mutes Everything, sound FXdon't play*/
void MuteAll();
/*Changes the Background music the song number. Returns the song number*/
int SwapSong(int inc);
/*increment the song number. returns the new song number*/
int NextSong();
/*decrement the song number. Returns the new song number*/
int PrevSong();
/* Input: Please Use a Macro to determine which sound effect is playing. The macros are listed above
 *Summary: Plays the sound effect that corresponds to the macro if it is not muted.*/
void PlayFX(int fx);
//Checks if it is muted
bool IsMuted();

#endif 

/*Sound effects from
http://www.soundboard.com/sb/attackontitansoundfx
Irrklang
Bastion
https://www.youtube.com/watch?v=3KVEe6UVpYw
http://www.freesfx.co.uk/
BFXR
https://www.youtube.com/watch?v=4enMMX_fmZQ
*/
