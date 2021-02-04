#include "Pacman.h"

#include <sstream>

#include <time.h>

#include <iostream>

#include <fstream>

#include <string>

using namespace Audio;

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPlayerSpeed(0.2f), _cPlayerFrameTime(250), _cMunchieFrameTime(500 * MUNCHIECOUNT), _cCherryFrameTime(500), _cLiveFrameTime(500)
{
	//Initialise member variables
	_pacman = new player();

	_menu = new menu();

	_menu->_paused = false;
	_menu->_pKeyDown = false;

	_menu->_started = true;
	_menu->_spaceKeyDown = false;

	_pacman->_playerDirection = 0;
	_pacman->_playerCurrentFrameTime = 0;
	_pacman->_playerFrame = 0;
	_pacman->speedMultiplier = 1.0f;
	_pacman->dead = false;

	for (int i = 0; i < MUNCHIECOUNT; i++)
	{

		_munchies[i] = new enemy;
		_munchies[i]->_enemyFrameCount = rand() % 1;
		_munchies[i]->_enemyCurrentFrameTime = 0;
		_munchies[i]->_enemyFrame = rand() % 500 + 50;
		_munchies[i]->_okToPlace = true;
	}

	for (int i = 0; i < CHERRYCOUNT; i++)
	{
		_cherries[i] = new powerup();

		_cherries[i]->_powerupCurrentFrameTime = 0;
		_cherries[i]->_powerupFrameCount = 0;
		powerupActivated = false;
	}
	for (int i = 0; i < LIVECOUNT; i++)
	{
		_lives[i] = new live();
		_lives[i]->_liveCurrentFrameTime = 0;
		_lives[i]->_liveFrameCount = 0;
	}


	scoreCount = 0;
	liveCount = 3;

	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i] = new movingEnenmy();
		_ghosts[i]->direction = 0;
		_ghosts[i]->speed = 0.2f;
	}

	// Load Sounds
	_pop = new SoundEffect();
	_introMusic = new SoundEffect();
	_chomp = new SoundEffect();
	_death = new SoundEffect();
	_ghostHunt = new SoundEffect();
	_fruit = new SoundEffect();
	_ghostDeath = new SoundEffect();
	_siren = new SoundEffect();

	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();


	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete _pacman->_playerTexture;
	delete _pacman->_playerSourceRect;
	delete _pacman->_playerPosition;
	delete _pacman;

	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		delete _munchies[i]->_enemyTexture;
		delete _munchies[i]->_enemyPosition;
		delete _munchies[i]->_enemyRectOne;
		delete _munchies[i]->_enemyRectOne;
		delete _munchies;
	}
	delete[] _munchies;

	delete _ghosts[0]->blueGhost;
	delete _ghosts[0]->redGhost;
	delete _ghosts[0]->orangeGhost;
	delete _ghosts[0]->pinkGhost;
	delete _ghosts[0]->vulnerableGhost;

	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		delete _ghosts[i]->position;
		delete _ghosts[i]->sourceRectOne;
		delete _ghosts[i];
	}
	delete[] _ghosts;

	for (int i = 0; i < CHERRYCOUNT; i++)
	{
		delete _cherries[i]->_powerupTexture;
		delete _cherries[i]->_powerupPosition;
		delete _cherries[i]->_powerupRectOne;
		delete _cherries[i]->_powerupRectTwo;
		delete _cherries;
	}

	for (int i = 0; i < LIVECOUNT; i++)
	{
		delete _lives[i]->_liveTexture;
		delete _lives[i]->_livePosition;
		delete _lives[i]->_liveRectOne;
		delete _lives[i]->_liveRectTwo;
		delete _lives;

	}

	delete _menu->gameBackground;
	delete _menu->_backgroundRect;

	delete _pop;
	delete _introMusic;
	delete _chomp;
	delete _death;
	delete _ghostHunt;
	delete _fruit;
	delete _ghostDeath;
	delete _siren;
}

void Pacman::LoadContent()
{
	// Load Pacman
	_pacman->_playerTexture = new Texture2D();
	_pacman->_playerTexture->Load("Textures/Pacman.tga", false);
	_pacman->_playerPosition = new Vector2(496.2f, 560.0f);
	_pacman->_playerSourceRect = new Rect(0.0f, 0.0f, 32, 32);


	// Load Munchie
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->_enemyTexture = new Texture2D;
		_munchies[i]->_enemyTexture->Load("Textures/Munchie.png", false);
		_munchies[i]->_enemyRectOne = new Rect(0.0f, 0.0f, 12, 12);
		_munchies[i]->_enemyRectTwo = new Rect(12.0f, 0.0f, 12, 12);
		_munchies[i]->_enemyPosition = new Vector2(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight());
	}


	// Load Cherry	
	for (int i = 0; i < CHERRYCOUNT; i++)
	{
		_cherries[i]->_powerupTexture = new Texture2D();
		_cherries[i]->_powerupTexture->Load("Textures/Cherry.png", true);
		_cherries[i]->_powerupRectOne = new Rect(0.0f, 0.0f, 32, 32);
		_cherries[i]->_powerupRectTwo = new Rect(32.0f, 0.0f, 32, 32);
		_cherries[i]->_powerupPosition = new Vector2(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight());
	}

	// Load Lives
	for (int i = 0; i < LIVECOUNT; i++)
	{
		_lives[i]->_liveTexture = new Texture2D();
		_lives[i]->_liveTexture->Load("Textures/Heart.png", false);
		_lives[i]->_liveRectOne = new Rect(0.0f, 0.0f, 32, 32);
		_lives[i]->_liveRectTwo = new Rect(32.0f, 0.0f, 32, 32);
		_lives[i]->_livePosition = new Vector2(492.8f, 269.8f);
	}


	// Load Ghosts

	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i]->blueGhost = new Texture2D;
		_ghosts[i]->blueGhost->Load("Textures/BlueGhost.png", false);

		_ghosts[i]->redGhost = new Texture2D;
		_ghosts[i]->redGhost->Load("Textures/RedGhost.png", false);

		_ghosts[i]->orangeGhost = new Texture2D;
		_ghosts[i]->orangeGhost->Load("Textures/OrangeGhost.png", false);

		_ghosts[i]->pinkGhost = new Texture2D;
		_ghosts[i]->pinkGhost->Load("Textures/PinkGhost.png", false);

		_ghosts[i]->vulnerableGhost = new Texture2D;
		_ghosts[i]->vulnerableGhost->Load("Textures/VulnerableGhost.png", false);


		_ghosts[0]->position = new Vector2(420.0f, 340.0f);
		_ghosts[1]->position = new Vector2(460.0f, 340.0f);
		_ghosts[2]->position = new Vector2(535.0f, 340.0f);
		_ghosts[3]->position = new Vector2(585.0f, 340.0f);


		//0 = blue
		//1 = red
		//2 = orange
		//3 = pink

		_ghosts[i]->sourceRectOne = new Rect(0.0f, 0.0f, 20, 20);
	}
	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);
	_stringPositionScore = new Vector2(10.0f, 50.0f);
	_stringPositionLives = new Vector2(10.0f, 75.0f);

	// Set Menu Parameters
	_menu->_menuBackground = new Texture2D();
	_menu->_menuBackground->Load("Textures/Transparency.png", false);
	_menu->_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menu->_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
	_menu->_startStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.5f, Graphics::GetViewportHeight() / 2.5f);

	_menu->gameBackground = new Texture2D;
	_menu->gameBackground->Load("Textures/background.png", false);
	_menu->_backgroundRect = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());

	_pop->Load("Sounds/pop.wav");
	_introMusic->Load("Sounds/intro.wav");
	_chomp->Load("Sounds/chomp.wav");
	_death->Load("Sounds/death.wav");
	_fruit->Load("Sounds/fruit.wav");
	_ghostHunt->Load("Sounds/powerup.wav");
	_ghostDeath->Load("Sounds/ghostDeath.wav");
	_siren->Load("Sounds/siren.wav");
	_siren->SetLooping(true);

}

void Pacman::Update(int elapsedTime)
{
	Audio::Play(_siren);
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	Input::MouseState* mouseState = Input::Mouse::GetState();

	if (keyboardState->IsKeyDown(Input::Keys::SPACE) && !_menu->_spaceKeyDown)
	{
		_menu->_spaceKeyDown = true;
		Audio::Play(_introMusic);
		_menu->_started = !_menu->_started;
	}

	if (!_menu->_started)
	{
		CheckPaused(keyboardState, Input::Keys::P);
		if (!_menu->_paused)
		{

			Input(elapsedTime, keyboardState, mouseState);
			UpdatePacman(elapsedTime);
			UpdateCherry(elapsedTime);
			UpdateLive(elapsedTime);
			for (int i = 0; i < GHOSTCOUNT; i++)
			{
				UpdateGhost(_ghosts[i], elapsedTime);
			}
			for (int i = 0; i < MUNCHIECOUNT; i++)
			{
				UpdateMunchie(_munchies[i], elapsedTime);
			}

			CheckViewportCollision();
			checkMunchieCollisions();
			checkGhostCollisions();
			checkCherryCollisions();
			checkLiveCollisions();
		}
	}
}

void Pacman::Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState)
{
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	bool otherKeyPress = false;

	float pacmanSpeed = _cPlayerSpeed * elapsedTime * _pacman->speedMultiplier;

	// User Input
	if (keyboardState->IsKeyDown(Input::Keys::D) && otherKeyPress == false)
	{
		_pacman->_playerPosition->X += pacmanSpeed;
		otherKeyPress = true;
		_pacman->_playerDirection = 0;
	}

	if (keyboardState->IsKeyDown(Input::Keys::A) && otherKeyPress == false)
	{
		_pacman->_playerPosition->X -= pacmanSpeed;
		otherKeyPress = true;
		_pacman->_playerDirection = 2;

	}

	if (keyboardState->IsKeyDown(Input::Keys::W) && otherKeyPress == false)
	{
		_pacman->_playerPosition->Y -= pacmanSpeed;
		otherKeyPress = true;
		_pacman->_playerDirection = 3;
	}

	if (keyboardState->IsKeyDown(Input::Keys::S) && otherKeyPress == false)
	{
		_pacman->_playerPosition->Y += pacmanSpeed;
		otherKeyPress = true;
		_pacman->_playerDirection = 1;
	}

	if (keyboardState->IsKeyDown(Input::Keys::LEFTSHIFT))
	{
		_pacman->speedMultiplier = 2.0f;
	}
	else
	{
		_pacman->speedMultiplier = 1.0f;
	}

}

void Pacman::UpdatePacman(int elapsedTime)
{

	_pacman->_playerCurrentFrameTime += elapsedTime;

	if (_pacman->_playerCurrentFrameTime > _cPlayerFrameTime)
	{
		_pacman->_playerFrame++;

		if (_pacman->_playerFrame >= 2)
		{
			_pacman->_playerFrame = 0;
		}

		_pacman->_playerCurrentFrameTime = 0;

	}

	_pacman->_playerSourceRect->X = _pacman->_playerSourceRect->Width * _pacman->_playerFrame;
	_pacman->_playerSourceRect->Y = _pacman->_playerSourceRect->Height * _pacman->_playerDirection;
}

void Pacman::UpdateCherry(int elapsedTime)
{
	for (int i = 0; i < CHERRYCOUNT; i++)
	{

		_cherries[i]->_powerupCurrentFrameTime += elapsedTime;

		if (_cherries[i]->_powerupCurrentFrameTime > _cCherryFrameTime)
		{
			_cherries[i]->_powerupFrameCount++;

			if (_cherries[i]->_powerupFrameCount >= 2)
			{
				_cherries[i]->_powerupFrameCount = 0;
			}

			_cherries[i]->_powerupCurrentFrameTime = 0;

		}
	}




}

void Pacman::UpdateGhost(movingEnenmy* ghost, int elapsedTime)
{


	// Direction 0 = right
	// Direction 1 = left

	// Direction 2 = down
	// Direction 3 = up

	// Ghosts Following Player

	int _distanceX = _pacman->_playerPosition->X - ghost->position->X;
	int _distanceY = _pacman->_playerPosition->Y - ghost->position->Y;
	bool isMoving = false;

	if (_distanceX > 0 && isMoving == false)
	{
		ghost->position->X += 1;
		isMoving = true;
		ghost->sourceRectOne = new Rect(0.0f, 0.0f, 20, 20);
	}
	else if (_distanceX < 0 && isMoving == false)
	{
		ghost->position->X -= 1;
		isMoving = true;
		ghost->sourceRectOne = new Rect(20.0f, 0.0f, 20, 20);
	}

	if (_distanceY > 0 && isMoving == false)
	{
		ghost->position->Y += 1;
		isMoving = true;
		ghost->sourceRectOne = new Rect(65.0f, 0.0f, 20, 20);
	}
	else if (_distanceY < 0 && isMoving == false)
	{
		ghost->position->Y;
		isMoving = true;
		ghost->sourceRectOne = new Rect(41.0f, 0.0f, 20, 20);
	}

}

void Pacman::UpdateMunchie(enemy*, int elapsedTime)
{
	// Munchie Animation

	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->_enemyCurrentFrameTime += elapsedTime;

		if (_munchies[i]->_enemyCurrentFrameTime > _cMunchieFrameTime)
		{
			_munchies[i]->_enemyFrameCount++;

			if (_munchies[i]->_enemyFrameCount >= 2)
			{
				_munchies[i]->_enemyFrameCount = 0;
			}

			_munchies[i]->_enemyCurrentFrameTime = 0;

		}
	}
}

void Pacman::UpdateLive(int elapsedTime)
{
	for (int i = 0; i < LIVECOUNT; i++)
	{
		_lives[i]->_liveCurrentFrameTime += elapsedTime;

		if (_lives[i]->_liveCurrentFrameTime > _cLiveFrameTime)
		{
			_lives[i]->_liveFrameCount++;

			if (_lives[i]->_liveFrameCount >= 2)
			{
				_lives[i]->_liveFrameCount = 0;
			}

			_lives[i]->_liveCurrentFrameTime = 0;

		}
	}
}

void Pacman::CheckViewportCollision()
{





	// Colliding with walls
	if (_pacman->_playerPosition->X + _pacman->_playerSourceRect->Width >= Graphics::GetViewportWidth() && _pacman->dead == false) // Wrap around right to left
	{
		_pacman->_playerPosition->X = 0 - _pacman->_playerSourceRect->Width;
	}
	else if (_pacman->_playerPosition->X + _pacman->_playerSourceRect->Width <= 0 && _pacman->dead == false) // Wrap around left to right
	{
		_pacman->_playerPosition->X = Graphics::GetViewportWidth() - _pacman->_playerSourceRect->Width;
	}

	if (_pacman->_playerPosition->Y + _pacman->_playerSourceRect->Height <= 0 && _pacman->dead == false)
	{
		_pacman->_playerPosition->Y = 0 - _pacman->_playerSourceRect->Height;
	}
	if (_pacman->_playerPosition->Y + _pacman->_playerSourceRect->Height >= Graphics::GetViewportHeight() && _pacman->dead == false) // Hits Bottom Wall
	{
		_pacman->_playerPosition->Y = Graphics::GetViewportHeight() - _pacman->_playerSourceRect->Height;
	}





	/*	if (_pacman->_playerPosition->Y + _pacman->_playerSourceRect->Height >= Graphics::GetViewportHeight()) // Wrap around top to bot
		{
			_pacman->_playerPosition->Y = 0 - _pacman->_playerSourceRect->Height;
		}
		else if (_pacman->_playerPosition->Y + _pacman->_playerSourceRect->Height <= 0) // Wrap around bot to top
		{
			_pacman->_playerPosition->Y = Graphics::GetViewportHeight() - _pacman->_playerSourceRect->Height;
		} */

}

void Pacman::CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey)
{
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	if (keyboardState->IsKeyDown(Input::Keys::P) && !_menu->_pKeyDown)
	{
		_menu->_pKeyDown = true;
		_menu->_paused = !_menu->_paused;
	}
	if (keyboardState->IsKeyUp(Input::Keys::P))
	{
		_menu->_pKeyDown = false;
	}
}

void Pacman::checkMunchieCollisions()
{
	int left1 = _pacman->_playerPosition->X;

	int left2 = 0;

	int right1 = _pacman->_playerPosition->X + _pacman->_playerSourceRect->Width;

	int right2 = 0;

	int top1 = _pacman->_playerPosition->Y;

	int top2 = 0;

	int bottom1 = _pacman->_playerPosition->Y + _pacman->_playerSourceRect->Height;

	int bottom2 = 0;

	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		bottom2 = _munchies[i]->_enemyPosition->Y + _munchies[i]->_enemyRectOne->Height;

		left2 = _munchies[i]->_enemyPosition->X;

		right2 = _munchies[i]->_enemyPosition->X + _munchies[i]->_enemyRectOne->Width;

		top2 = _munchies[i]->_enemyPosition->Y;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			scoreCount += 100;
			Audio::Play(_pop);
			_munchies[i]->_enemyPosition = new Vector2(-1000.0f, -1000.0f);

		}

	}



}

void Pacman::checkCherryCollisions()
{
	int left1 = _pacman->_playerPosition->X;

	int left2 = 0;

	int right1 = _pacman->_playerPosition->X + _pacman->_playerSourceRect->Width;

	int right2 = 0;

	int top1 = _pacman->_playerPosition->Y;

	int top2 = 0;

	int bottom1 = _pacman->_playerPosition->Y + _pacman->_playerSourceRect->Height;

	int bottom2 = 0;


	for (int i = 0; i < CHERRYCOUNT; i++)
	{
		bottom2 = _cherries[i]->_powerupPosition->Y + _cherries[i]->_powerupRectOne->Height;

		left2 = _cherries[i]->_powerupPosition->X;

		right2 = _cherries[i]->_powerupPosition->X + _cherries[i]->_powerupRectOne->Width;

		top2 = _cherries[i]->_powerupPosition->Y;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			powerupActivated = true;
			Audio::Play(_fruit);
			Audio::Play(_ghostHunt);
			_cherries[i]->_powerupPosition = new Vector2(-3000.0f, -3000.0f);
			i = CHERRYCOUNT;
		}
	}
}

void Pacman::checkGhostCollisions()
{
	int left1 = _pacman->_playerPosition->X;

	int left2 = 0;

	int right1 = _pacman->_playerPosition->X + _pacman->_playerSourceRect->Width;

	int right2 = 0;

	int top1 = _pacman->_playerPosition->Y;

	int top2 = 0;

	int bottom1 = _pacman->_playerPosition->Y + _pacman->_playerSourceRect->Height;

	int bottom2 = 0;


	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		bottom2 = _ghosts[i]->position->Y + _ghosts[i]->sourceRectOne->Height;

		left2 = _ghosts[i]->position->X;

		right2 = _ghosts[i]->position->X + _ghosts[i]->sourceRectOne->Width;

		top2 = _ghosts[i]->position->Y;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			if (powerupActivated == true)
			{
				scoreCount += 500;
				_ghosts[i]->position = new Vector2(377.0f, 260.0f);
				Audio::Play(_ghostDeath);
				i = GHOSTCOUNT;
			}
			else if (powerupActivated == false)
			{
				Audio::Play(_death);
				liveCount--;
				_pacman->_playerPosition = new Vector2(496.2f, 560.0f);
				if (liveCount <= 0)
				{
					_pacman->_playerPosition = new Vector2(-1000.0f, -1000.0f);
					_pacman->dead = true;
					liveCount = 0;
				}
				i = GHOSTCOUNT;
			}
		}
	}
}

void Pacman::checkLiveCollisions()
{
	int left1 = _pacman->_playerPosition->X;

	int left2 = 0;

	int right1 = _pacman->_playerPosition->X + _pacman->_playerSourceRect->Width;

	int right2 = 0;

	int top1 = _pacman->_playerPosition->Y;

	int top2 = 0;

	int bottom1 = _pacman->_playerPosition->Y + _pacman->_playerSourceRect->Height;

	int bottom2 = 0;


	for (int i = 0; i < LIVECOUNT; i++)
	{
		bottom2 = _lives[i]->_livePosition->Y + _lives[i]->_liveRectOne->Height;

		left2 = _lives[i]->_livePosition->X;

		right2 = _lives[i]->_livePosition->X + _lives[i]->_liveRectOne->Width;

		top2 = _lives[i]->_livePosition->Y;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			Audio::Play(_fruit);
			liveCount += 1;
			scoreCount += 1000;
			_lives[i]->_livePosition = new Vector2(-2000.0f, -2000.0f);
			i = LIVECOUNT;

		}
	}
}

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	std::stringstream scoreCounter;
	std::stringstream liveCounter;

	// stream << "Pacman X: " << _pacman->_playerPosition->X << " Y: " << _pacman->_playerPosition->Y;
	scoreCounter << "Score: " << scoreCount;
	liveCounter << "Lives: " << liveCount;

	SpriteBatch::BeginDraw(); // Starts Drawing

	SpriteBatch::Draw(_menu->gameBackground, _menu->_backgroundRect, nullptr);


	// Drawing Pacman
	if (!_pacman->dead)
	{
		SpriteBatch::Draw(_pacman->_playerTexture, _pacman->_playerPosition, _pacman->_playerSourceRect); // Draws Pacman
	}




	// Drawing Ghosts


	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		if (powerupActivated == true)
		{

			SpriteBatch::Draw(_ghosts[i]->vulnerableGhost, _ghosts[i]->position, _ghosts[i]->sourceRectOne);
			timeBetweenStates--;
			if (timeBetweenStates == 1)
			{
				powerupActivated = false;
				timeBetweenStates = 1000;
			}

		}

		if (powerupActivated == false)
		{
			SpriteBatch::Draw(_ghosts[0]->blueGhost, _ghosts[0]->position, _ghosts[0]->sourceRectOne);
			SpriteBatch::Draw(_ghosts[1]->redGhost, _ghosts[1]->position, _ghosts[1]->sourceRectOne);
			SpriteBatch::Draw(_ghosts[2]->orangeGhost, _ghosts[2]->position, _ghosts[2]->sourceRectOne);
			SpriteBatch::Draw(_ghosts[3]->pinkGhost, _ghosts[3]->position, _ghosts[3]->sourceRectOne);
		}
	}

	//0 = blue
	//1 = red
	//2 = orange
	//3 = pink


// Drawing Munchies
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		if (_munchies[i]->_enemyFrameCount == 0)
		{
			// Draws Red Munchie
			SpriteBatch::Draw(_munchies[i]->_enemyTexture, _munchies[i]->_enemyPosition, _munchies[i]->_enemyRectOne, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
		}
		else
		{
			// Draw Blue Munchie
			SpriteBatch::Draw(_munchies[i]->_enemyTexture, _munchies[i]->_enemyPosition, _munchies[i]->_enemyRectTwo, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

			_munchies[i]->_enemyFrameCount++;

			if (_munchies[i]->_enemyFrameCount >= 60)
				_munchies[i]->_enemyFrameCount = 0;
		}
	}

	// Drawing Cherries
	for (int i = 0; i < CHERRYCOUNT; i++)
	{
		if (_cherries[i]->_powerupFrameCount == 0)
		{
			SpriteBatch::Draw(_cherries[i]->_powerupTexture, _cherries[i]->_powerupPosition, _cherries[i]->_powerupRectOne, Vector2::Zero, 0.6f, 0.0f, Color::White, SpriteEffect::NONE);
		}
		else
		{
			SpriteBatch::Draw(_cherries[i]->_powerupTexture, _cherries[i]->_powerupPosition, _cherries[i]->_powerupRectTwo, Vector2::Zero, 0.6f, 0.0f, Color::White, SpriteEffect::NONE);

			_cherries[i]->_powerupFrameCount++;

			if (_cherries[i]->_powerupFrameCount >= 60)
				_cherries[i]->_powerupFrameCount = 0;
		}
	}

	// Drawing Lives

	for (int i = 0; i < LIVECOUNT; i++)
	{
		if (_lives[i]->_liveFrameCount == 0)
		{
			SpriteBatch::Draw(_lives[i]->_liveTexture, _lives[i]->_livePosition, _lives[i]->_liveRectOne, Vector2::Zero, 0.6f, 0.0f, Color::White, SpriteEffect::NONE);
		}
		else
		{
			SpriteBatch::Draw(_lives[i]->_liveTexture, _lives[i]->_livePosition, _lives[i]->_liveRectTwo, Vector2::Zero, 0.6f, 0.0f, Color::White, SpriteEffect::NONE);

			_lives[i]->_liveFrameCount++;

			if (_lives[i]->_liveFrameCount >= 60)
				_lives[i]->_liveFrameCount = 0;
		}
	}

	// Drawing Menus
	if (_menu->_paused == true)
	{
		std::stringstream menuStream;
		menuStream << "PAUSED!";
		SpriteBatch::Draw(_menu->_menuBackground, _menu->_menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->_menuStringPosition, Color::Red);
	}
	if (_menu->_started == true)
	{
		std::stringstream startStream;
		startStream << "Press Space to Start!";
		SpriteBatch::Draw(_menu->_menuBackground, _menu->_menuRectangle, nullptr);
		SpriteBatch::DrawString(startStream.str().c_str(), _menu->_startStringPosition, Color::Yellow);

	}



	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	SpriteBatch::DrawString(scoreCounter.str().c_str(), _stringPositionScore, Color::Green);
	SpriteBatch::DrawString(liveCounter.str().c_str(), _stringPositionLives, Color::Green);
	SpriteBatch::EndDraw(); // Ends Drawing
}