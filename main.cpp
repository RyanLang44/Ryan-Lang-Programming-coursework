#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one


#include <windows.h>
#include "windowOGL.h"
#include "GameConstants.h"
#include "cWNDManager.h"
#include "cInputMgr.h"
#include "cSoundMgr.h"
#include "cFontMgr.h"
#include "cSprite.h"
#include "asteroidsGame.h"
#include <sstream>
#include "cButton.h"

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR cmdLine,
                   int cmdShow)
{

    //Set our window settings
    const int windowWidth = 1024;
    const int windowHeight = 768;
    const int windowBPP = 16;



    //This is our window
	static cWNDManager* pgmWNDMgr = cWNDManager::getInstance();

	// This is the input manager
	static cInputMgr* theInputMgr = cInputMgr::getInstance();

	// This is the sound manager
	static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();

	// This is the Font manager
	static cFontMgr* theFontMgr = cFontMgr::getInstance();

	//The example OpenGL code
    windowOGL theOGLWnd;
	
    //Attach our the OpenGL window
	pgmWNDMgr->attachOGLWnd(&theOGLWnd);

	// Attach the keyboard manager
	pgmWNDMgr->attachInputMgr(theInputMgr);

    //Attempt to create the window
	if (!pgmWNDMgr->createWND(windowWidth, windowHeight, windowBPP))
    {
        //If it fails

        MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	if (!theOGLWnd.initOGL(windowWidth, windowHeight)) //Initialize our example
    {
        MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	//Clear key buffers
	theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);

	/* initialize random seed: */
	srand((unsigned int)time(NULL));

	// Create vector array of textures
	LPCSTR texturesToUse[] = { "Images\\asteroid1.png", "Images\\asteroid2.png", "Images\\asteroid3.png", "Images\\asteroid4.png", "Images\\bullet.png" };
	for (int tCount = 0; tCount < 5; tCount++)
	{
		theGameTextures.push_back(new cTexture());
		theGameTextures[tCount]->createTexture(texturesToUse[tCount]);
	}

	// load game sounds
	// Load Sound
	LPCSTR gameSounds[3] = { "Audio/who10Edit.wav", "Audio/laser.wav", "Audio/explosion.wav" };

	//theSoundMgr->add("Theme", gameSounds[0]);
	theSoundMgr->add("Shot", gameSounds[1]);
	theSoundMgr->add("Explosion", gameSounds[2]);

	// load game fontss
	// Load Fonts
	LPCSTR gameFonts[2] = { "Fonts/digital-7.ttf", "Fonts/space age.ttf" };

	theFontMgr->addFont("SevenSeg", gameFonts[0], 24);
	theFontMgr->addFont("Space", gameFonts[1], 24);

	// Create vector array of textures

	for (int astro = 0; astro < 50; astro++)
	{
		theAsteroids.push_back(new cAsteroid);
		theAsteroids[astro]->setSpritePos(glm::vec2(windowWidth + (rand() % 5 + 1), windowHeight / (rand() % 5 + 1)));
		theAsteroids[astro]->setSpriteTranslation(glm::vec2((-200.0f, 0.0f)));
		int randAsteroid = rand() % 4;
		theAsteroids[astro]->setTexture(theGameTextures[randAsteroid]->getTexture());
		theAsteroids[astro]->setTextureDimensions(theGameTextures[randAsteroid]->getTWidth(), theGameTextures[randAsteroid]->getTHeight());
		theAsteroids[astro]->setSpriteCentre();
		theAsteroids[astro]->setAsteroidVelocity(glm::vec2(glm::vec2(0.0f, 0.0f)));
		theAsteroids[astro]->setActive(true);
		theAsteroids[astro]->setMdlRadius();
	}

	cTexture rocketTxt;
	rocketTxt.createTexture("Images\\spaceship.png");
	cRocket rocketSprite;
	rocketSprite.attachInputMgr(theInputMgr); // Attach the input manager to the sprite
	rocketSprite.setSpritePos(glm::vec2(70.0f, 380.0f));
	rocketSprite.setTexture(rocketTxt.getTexture());
	rocketSprite.setTextureDimensions(rocketTxt.getTWidth(), rocketTxt.getTHeight());
	rocketSprite.setSpriteCentre();
	rocketSprite.setRocketVelocity(glm::vec2(0.0f, 0.0f));
	rocketSprite.setSpriteRotation(0);
	//rocketSprite.setSpriteTranslation(glm::vec2(25.0f, -25.0f));
	// Attach sound manager to rocket sprite
	rocketSprite.attachSoundMgr(theSoundMgr);

	// Create vector array of textures
	vector<cTexture*> textureBkgList;
	LPCSTR bkgTexturesToUse[] = { "Images/outer space.png", "Images/Title Screen.png", "Images/end screen.png" };
	for (int tCount = 0; tCount < 3; tCount++)
	{
		textureBkgList.push_back(new cTexture());
		textureBkgList[tCount]->createTexture(bkgTexturesToUse[tCount]);
	}

	cTexture transSprite;
	transSprite.createTexture("Images/blank.png");

	cBkGround spriteBkgd;
	spriteBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteBkgd.setTexture(textureBkgList[0]->getTexture());
	spriteBkgd.setTextureDimensions(textureBkgList[0]->getTWidth(), textureBkgList[0]->getTHeight());

	cBkGround spriteStartBkgd;
	spriteStartBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteStartBkgd.setTexture(textureBkgList[1]->getTexture());
	spriteStartBkgd.setTextureDimensions(textureBkgList[1]->getTWidth(), textureBkgList[1]->getTHeight());

	cBkGround spriteEndBkgd;
	spriteEndBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteEndBkgd.setTexture(textureBkgList[2]->getTexture());
	spriteEndBkgd.setTextureDimensions(textureBkgList[2]->getTWidth(), textureBkgList[2]->getTHeight());


	
		// Create vector array of button textures
		vector<cTexture*> btnTextureList;
		LPCSTR btnTexturesToUse[] = { "Images/Buttons/exitBtn.png", "Images/Buttons/instructionsBtn.png", "Images/Buttons/loadBtn.png", "Images/Buttons/playBtn.png", "Images/Buttons/saveBtn.png", "Images/Buttons/settingsBtn.png" };
		for (int tCount = 0; tCount < 6; tCount++)
		{
			btnTextureList.push_back(new cTexture());
			btnTextureList[tCount]->createTexture(btnTexturesToUse[tCount]);
		}

		cButton exitButton;
		exitButton.attachInputMgr(theInputMgr);
		exitButton.setTexture(btnTextureList[0]->getTexture());
		exitButton.setTextureDimensions(btnTextureList[0]->getTWidth(), btnTextureList[0]->getTHeight());

		cButton playButton;
		playButton.attachInputMgr(theInputMgr);
		playButton.setTexture(btnTextureList[3]->getTexture());
		playButton.setTextureDimensions(btnTextureList[3]->getTWidth(), btnTextureList[3]->getTHeight());

		// strings for text display
		string outputMsg;
		string strMsg[] = { "ROCKET FLIGHT", "SPACE!", "PEW PEW", "Thanks for playing!", "See you again soon!" };

	
		//declaration of the Score
		int theScore = 0;
		string strScore = "";

		//starting game state
		gameState theGameState = MENU;
		btnTypes theBtnType = EXIT;

		//This is the mainloop, we render frames until isRunning returns false
		while (pgmWNDMgr->isWNDRunning())
		{
			pgmWNDMgr->processWNDEvents(); //Process any window events

			//We get the time that passed since the last frame
			float elapsedTime = pgmWNDMgr->getElapsedSeconds();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			switch (theGameState)
			{
			case MENU:
				spriteStartBkgd.render();

				playButton.setSpritePos(glm::vec2(400.0f, 300.0f));
				exitButton.setSpritePos(glm::vec2(400.0f, 375.0f));
				playButton.render();
				exitButton.render();

				theGameState = playButton.update(theGameState, PLAYING);
				exitButton.update();


				outputMsg = strMsg[0];
				theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(10, 15, 0.0f));
				outputMsg = strMsg[0];
				theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(100, 100, 0.0f));
				outputMsg = strMsg[1];
				theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(100, 150, 0.0f));
				
				if (exitButton.getClicked())
				{
					SendMessage(pgmWNDMgr->getWNDHandle(), WM_CLOSE, NULL, NULL);
				}
				break;

			case PLAYING:
				spriteBkgd.render();
				rocketSprite.update(elapsedTime);
				rocketSprite.render();
				strScore = "Score: " + to_string(theScore);
				theFontMgr->getFont("Space")->printText(strScore.c_str(), FTPoint(500.0f, 15.0f, 0.0f));

				{
					vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin();
					while (asteroidIterator != theAsteroids.end())
					{
						if ((*asteroidIterator)->isActive() == false)
						{
							asteroidIterator = theAsteroids.erase(asteroidIterator);
							theScore++;
						}
						else
						{
							(*asteroidIterator)->update(elapsedTime);
							(*asteroidIterator)->render();
							++asteroidIterator;
						}
					}
				}
				// render button and reset clicked to false
				exitButton.setSpritePos(glm::vec2(740.0f, 575.0f));
				exitButton.render();
				theGameState = exitButton.update(theGameState, END);

				outputMsg = strMsg[2];
				theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(10, 15, 0.0f));

				break;

			case END:
				spriteEndBkgd.render();

				playButton.setClicked(false);
				exitButton.setClicked(false);

				playButton.setSpritePos(glm::vec2(400.0f, 300.0f));
				exitButton.setSpritePos(glm::vec2(400.0f, 375.0f));
				playButton.render();
				exitButton.render();

				theGameState = playButton.update(theGameState, PLAYING);
				exitButton.update();

				outputMsg = strMsg[2];
				theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(10, 15, 0.0f));
				outputMsg = strMsg[3];
				theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(100, 100, 0.0f));
				outputMsg = strMsg[4];
				theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(100, 150, 0.0f));
				if (exitButton.getClicked())
				{
					SendMessage(pgmWNDMgr->getWNDHandle(), WM_CLOSE, NULL, NULL);
				}

				break;
			}

		

		pgmWNDMgr->swapBuffers();
		theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);   /// theInputMgr->KEYS_DOWN_BUFFER  | theInputMgr->KEYS_PRESSED_BUFFER 
    }

	theOGLWnd.shutdown(); //Free any resources
	pgmWNDMgr->destroyWND(); //Destroy the program window

    return 0; //Return success
}
