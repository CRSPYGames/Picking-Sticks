/*
**********************************************************************;
*	Project      : Pickin' Sticks
*
*	Program name : Main
*
*	Author		 : ResponsibleFile (Dawson McThay)
*
*	Date created : 09/15/2013
*
*	Purpose		 : I had always wanted to make a game, so I did.
*				 : For the record, I made this at 13. But out of respect for 13 year old me,
*				 : I refrained from changing texts, I felt it was charming. 
*				 : BOUNS: I didn't know what object-oriented programming was yet.
*
*	Remastered	 : 12/13/2019
*
**********************************************************************;
*/

// C++ Base Includes
#include <iostream>
#include <sstream>
#include <iomanip>

//API Includes
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

const std::string TITLE = "Pickin' Sticks SFML Version 1";

int main()
{
	// Window Settings
	sf::Vector2i WindowDimensions(1280, 720);
	sf::RenderWindow Window(sf::VideoMode(WindowDimensions.x, WindowDimensions.y), TITLE);
	Window.setKeyRepeatEnabled(false);

	// Animation Frame Switching (this was from a Rachel Singh tutorial back in '09)
	sf::Clock Clock;
	// I quite frankly have no idea where I yanked these numbers from, but these are the ones that felt right apparently
	float CurrentFrameCount = 0, FrameIncrement = 75, FramesPerSecond = 416;

	// Music
	sf::Music BackgroundMusic;
	sf::SoundBuffer Buffer;
	sf::Sound SoundEffect;

	//Background Music
	if(!BackgroundMusic.openFromFile("Sounds/Music/MusicSound.ogg"))
		std::cout << "OH NA NA NA" << std::endl; // lol - Dawson, 2019
	BackgroundMusic.setVolume(25);
	BackgroundMusic.setLoop(true);
	BackgroundMusic.play();

	// Sound Effect
    if (!Buffer.loadFromFile("Sounds/Effects/baseball_hit.wav"))
		return EXIT_FAILURE;
	SoundEffect.setBuffer(Buffer);

	// Player Properties
	// Sprite Textures
	sf::Texture PlayerTexture;
	if (!PlayerTexture.loadFromFile("Textures/Character.png"))
		return EXIT_FAILURE;
	sf::Sprite PlayerSprite;
	PlayerSprite.setTexture(PlayerTexture);
	PlayerSprite.setPosition(640 - 32, 360 - 16); 
	
	// Player Movement
	float CurSpeed = 0;
	const float MaxSpeed = 3.5f;
	int SticksCollected = 0;
	
	// Player Animation
	enum Directions
	{
		Down = 0,
		Left = 1,
		Right = 2,
		Up = 3,
	};
	// X = Frame | Enum = Direction
	sf::Vector2i CurrentAnimation(1, Down);

	// The Cinematic Black Bars
	sf::RectangleShape CinematicBars(sf::Vector2f(WindowDimensions.x, WindowDimensions.y/20));
	CinematicBars.setFillColor(sf::Color::Black);

	// The Looping Background Texture
	sf::Texture BackgroundTexture;
	if (!BackgroundTexture.loadFromFile("Textures/Grass.png"));
	sf::RectangleShape BG(sf::Vector2f(WindowDimensions.x,  WindowDimensions.y + WindowDimensions.y/20.0f));
	BackgroundTexture.setRepeated(true);
	BackgroundTexture.setTexture(&BackgroundTexture);

	// Branch Asset 
	sf::Texture BranchTexture;
	sf::Sprite BranchSprite;
	if (!BranchTexture.loadFromFile("Textures/Branch.png"))
		return EXIT_FAILURE;
	BranchSprite.setTexture(BranchTexture);
	BranchSprite.setScale(0.06f, 0.06f);

	// Random Branch Position
	std::srand((unsigned)time(NULL));
	sf::Vector2f RandomPos(std::rand() % WindowDimensions.x - 32, std::rand() % (WindowDimensions.y - 64) + (WindowDimensions.y / 20));
	BranchSprite.setPosition(RandomPos);

	// Well, The Font
	sf::Font Font;
	if (!Font.loadFromFile("Fonts/Font.ttf"));
	
	// Ranking System
	enum Ranks
	{
		Bad = 0,
		Ok = 10,
		Awesome = 20,
		Posh = 30,
		SWANKY = 40,
		Evil = 100
	};
	Ranks CurRank = Bad;

	// The Game Loop babyyyyy
	while (Window.isOpen())
	{
		sf::Event Event;

		std::string curSticksPickedString = std::to_string(SticksCollected);
		std::string curMoveSpeedString = std::to_string(CurSpeed);
		std::string curRankString;

		// A stick is collected if it overlaps with the player. 
		if (PlayerSprite.getPosition().x < BranchSprite.getPosition().x + BranchSprite.getGlobalBounds().width &&
			PlayerSprite.getPosition().x + PlayerSprite.getLocalBounds().width  > BranchSprite.getPosition().x&&
			PlayerSprite.getPosition().y < BranchSprite.getPosition().y + BranchSprite.getGlobalBounds().height &&
			PlayerSprite.getPosition().y + PlayerSprite.getLocalBounds().height  > BranchSprite.getPosition().y)
		{
			sf::Vector2f RandomPos(std::rand() % WindowDimensions.x - 32, std::rand() % (WindowDimensions.y - 128) + (WindowDimensions.y / 20));
			BranchSprite.setPosition(RandomPos);
			SticksCollected++;
			SoundEffect.play();
		}

		// Progression System
		if (SticksCollected >= 0)
		{
			CurRank = Bad;
			curRankString = "..meh";
			CurSpeed = 0.1f * MaxSpeed;
		}

		else if (SticksCollected >= 10)
		{
			CurRank = Ok;
			curRankString = "Not Bad";
			CurSpeed = 0.2f * MaxSpeed;
		}

		else if (SticksCollected >= 20)
		{
			CurRank = Awesome;
			curRankString = "Pretty Good";
			CurSpeed = 0.4f * MaxSpeed;
		}

		else if (SticksCollected >= 30)
		{
			CurRank = Posh;
			curRankString = "Awesome!";
			CurSpeed = 0.8f * MaxSpeed;
		}

		else if (SticksCollected >= 40)
		{
			CurRank = SWANKY;
			curRankString = "ULTRA COMBO!!!";
			CurSpeed = MaxSpeed;
		}

		else if (SticksCollected >= 100)
		{
			CurRank = Evil;
			curRankString = "TROLOLOL!!!!";
			CurSpeed = MaxSpeed * 2.5f;
		}

		/*
			// This was here to test if the stick collection actually worked
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
			{
				SticksCollected++;
			}
		*/

		// Setting Screen Text
		// The # of Sticks Picked Up Thus Far
		sf::Text ScoreText;
		ScoreText.setFont(Font);
		ScoreText.setString("Score: " + curSticksPickedString);
		ScoreText.setCharacterSize(34);
		ScoreText.setColor(sf::Color::White);
		ScoreText.setStyle(sf::Text::Bold);
		ScoreText.setPosition(0 + WindowDimensions.x / 8.0f, 0);

		// Current Speed
		sf::Text SpeedText;
		SpeedText.setFont(Font);
		SpeedText.setString("Current Speed: " + curMoveSpeedString);
		SpeedText.setCharacterSize(34);
		SpeedText.setColor(sf::Color::White);
		SpeedText.setStyle(sf::Text::Bold);
		SpeedText.setPosition((ScoreText.getPosition().x + ScoreText.getScale().x) + ScoreText.getGlobalBounds().width + 55, 0);

		// Current Rank
		sf::Text RankText;
		RankText.setFont(Font);
		RankText.setString("Current Rank: " + curRankString);
		RankText.setCharacterSize(34);
		RankText.setColor(sf::Color::White);
		RankText.setStyle(sf::Text::Bold);
		RankText.setPosition((SpeedText.getPosition().x + SpeedText.getScale().x) + SpeedText.getGlobalBounds().width + 55, 0);

		// Input 
		while (Window.pollEvent(Event))
		{
			if (Event.type == sf::Event::Closed)
				Window.close();
		}

		// Sets the Direction
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			CurrentAnimation.y = Down;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			CurrentAnimation.y = Left;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			CurrentAnimation.y = Right;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			CurrentAnimation.y = Up;

		// Attaches Movement
		switch (CurrentAnimation.y)
		{
			case Down:
				// As long as the sprite is within the realm of the screen, you may move. 
				if (PlayerSprite.getPosition().y <= (Window.getSize().y - PlayerSprite.getPosition().y / 32) - 32)
				{
					PlayerSprite.move(0, CurSpeed);
				}
				else
				{
					// Otherwise, stop the animation
					CurrentFrameCount = 0;
					CurrentAnimation.x = 0;
				}
				break;

			case Left:
				if (PlayerSprite.getPosition().x >= (Window.getSize().x - Window.getSize().x))
				{
					PlayerSprite.move(-CurSpeed, 0);
				}
				else
				{
					CurrentFrameCount = 0;
					CurrentAnimation.x = 0;
				}
				break;

			case Right:
				if (PlayerSprite.getPosition().x <= (Window.getSize().x - PlayerSprite.getPosition().x / 32))
				{
					PlayerSprite.move(CurSpeed, 0);
				}
				else
				{
					CurrentFrameCount = 0;
					CurrentAnimation.x = 0;
				}
				break;

			case Up:
				if (PlayerSprite.getPosition().y >= ((Window.getSize().y - Window.getSize().y) + WindowDimensions.y / 20))
				{
					PlayerSprite.move(0, -CurSpeed);
				}
				else
				{
					CurrentFrameCount = 0;
					CurrentAnimation.x = 0;
				}
				break;

			default:
				PlayerSprite.move(0, 0);
				break;
		}

		// Animation Switching

		// This adds a delta time (frameSpeed * Clock.restart().asSeconds()) to the current frame
		// and if it exceeds the switch counter, then the animation increases by one.
		// However, if the current frame exceeds the length of the texture width, then reset it
		CurrentFrameCount += FramesPerSecond * Clock.restart().asSeconds();
		if (CurrentFrameCount >= FrameIncrement)
		{
			CurrentFrameCount = 0;
			CurrentAnimation.x++;
			if (CurrentAnimation.x * 32 >= PlayerTexture.getSize().x)
				CurrentAnimation.x = 0;
		}
		// The rendering of the sprite sheet should take the current coordinates, multiply it by 32 because that's the width of each sprite
		// then render only that 32x32 portion of it.
		PlayerSprite.setTextureRect(sf::IntRect(CurrentAnimation.x * 32, CurrentAnimation.y * 32, 32, 32));

		// Drawing To Screen
		Window.draw(BackgroundTexture);
		Window.draw(BranchSprite);
		Window.draw(PlayerSprite);
		Window.draw(CinematicBars);

		Window.draw(ScoreText);
		Window.draw(SpeedText);
		Window.draw(RankText);

		Window.display();
	}
	return EXIT_SUCCESS;
}

