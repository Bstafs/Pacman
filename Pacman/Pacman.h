#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif
#endif

// Just need to include main header file
#include "S2D/S2D.h"


// Including Libraries and Definitions
#define MUNCHIECOUNT 50
#define GHOSTCOUNT 4
#define CHERRYCOUNT 10
#define LIVECOUNT 2

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

class Player;
class Enemy;
struct Tile;

// Structure Definition
struct player
{
	// Data for the player
	Vector2* _playerPosition;
	Rect* _playerSourceRect;
	Texture2D* _playerTexture;
	int _playerDirection;
	int _playerFrame;
	int _playerCurrentFrameTime;
	float speedMultiplier;
	bool dead;
};
struct enemy
{
	// Data to represent Munchie	
	Rect* _enemyRectOne;
	Rect* _enemyRectTwo;
	Vector2* _enemyPosition;
	Texture2D* _enemyTexture;
	int _enemyFrame;
	int _enemyCurrentFrameTime;
	int _enemyFrameCount;
	int _enemyFrameTime;
	bool _okToPlace;
};
struct movingEnenmy
{
	Vector2* position;
	Texture2D* blueGhost;
	Texture2D* redGhost;
	Texture2D* orangeGhost;
	Texture2D* pinkGhost;
	Texture2D* vulnerableGhost;
	Rect* sourceRectOne;
	int direction;
	float speed;

};
struct powerup
{
	Rect* _powerupRectOne;
	Rect* _powerupRectTwo;
	Vector2* _powerupPosition;
	Texture2D* _powerupTexture;
	int _powerupFrame;
	int _powerupCurrentFrameTime;
	int _powerupFrameCount;
};
struct live
{
	Rect* _liveRectOne;
	Rect* _liveRectTwo;
	Vector2* _livePosition;
	Texture2D* _liveTexture;
	int _liveFrame;
	int _liveCurrentFrameTime;
	int _liveFrameCount;
};

struct menu
{
	// data for menu
	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	Vector2* _menuStringPosition;

	//data for pause menu
	bool _paused;
	bool _pKeyDown;

	// data for start menu
	Vector2* _startStringPosition;
	bool _started;
	bool _spaceKeyDown;

	// Data for game Background
	Texture2D* gameBackground;
	Rect* _backgroundRect;

};

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:

	// Data to represent Pacman
	player* _pacman;
	const float _cPlayerSpeed;
	const int _cPlayerFrameTime;


	// Data to represent Munchie	
	enemy* _munchies[MUNCHIECOUNT];
	const int _cMunchieFrameTime;


	// Data to represent Ghosts
	movingEnenmy* _ghosts[GHOSTCOUNT];


	// Data to represent Cherry
	powerup* _cherries[CHERRYCOUNT];
	const int _cCherryFrameTime;
	bool powerupActivated;
	int timeBetweenStates = 1000;

	//Data to represent heart
	live* _lives[LIVECOUNT];
	const int _cLiveFrameTime;


	// Data to represent Menu
	menu* _menu;
	int scoreCount;
	int liveCount;

	// Data to represent sounds
	SoundEffect* _pop;
	SoundEffect* _introMusic;
	SoundEffect* _chomp;
	SoundEffect* _death;
	SoundEffect* _ghostHunt;
	SoundEffect* _fruit;
	SoundEffect* _ghostDeath;
	SoundEffect* _siren;

	// Position for String
	Vector2* _stringPosition;
	Vector2* _stringPositionScore;
	Vector2* _stringPositionLives;

	//Input Methods	
	void Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState);

	//Check Methods	
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);
	void CheckViewportCollision();
	void checkGhostCollisions();
	void checkMunchieCollisions();
	void checkCherryCollisions();
	void checkLiveCollisions();

	//Update Methods
	void UpdatePacman(int elapsedTime);
	void UpdateMunchie(enemy*, int elapsedTime);
	void UpdateCherry(int elapsedTime);
	void UpdateGhost(movingEnenmy*, int elapsedTime);
	void UpdateLive(int elapsedTime);

	// Sound Effects
	int timeBeforeStart = 1000;
	bool gameStarted;

public:





	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);

	void DrawTiles();
};

