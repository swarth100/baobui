#include "program_utils.hpp"

/* List of active programs to be rendered */
list<shared_ptr<Program>> programs;

/* Fields holding instances of the button components. The components trigger
   texture updates and are linked to sound effects */
shared_ptr<Component> topButton;
shared_ptr<Component> rightButton;
shared_ptr<Component> botButton;
shared_ptr<Component> leftButton;
bool isCubeFizzled = false;

/* Targetbutton stores the button chosen as a target for the cube */
shared_ptr<Component> targetButton;

/* The availableButtons list is iterated across when selecting a new button to
   select as a targetButton */
list<shared_ptr<Component>> availableButtons;

/* Holds a refererence to the companion cube object moved by BAoBOS */
shared_ptr<Component> companionCube;

/* Sound components */
shared_ptr<sf::Sound> backgroundTheme;
shared_ptr<sf::Sound> buttonUp;
shared_ptr<sf::Sound> buttonDown;
shared_ptr<sf::Sound> fizzle;
shared_ptr<sf::Sound> fizzleGlados;
shared_ptr<sf::Sound> introGlados;

/* List of sounds associated with successful step-by-step pacing of the test
   chamber. */
list<shared_ptr<sf::Sound>> successSounds;

/* List of active sound buffers. Sound buffers must be kept alive for the whole
   duration of the program or no audio will be played */
list<shared_ptr<sf::SoundBuffer>> buffer_list;

/* Create a new program instance, initialise it and return it */
shared_ptr<Program>
initProgram(const char* vs, const char* fs) {
	shared_ptr<Program> program = make_shared<Program>(vs, fs);

	/* Program is added to the list of active programs */
	programs.push_back(program);

	return program;
}

/* Given a uniform's name it attaches it to all available programs */
void
attachUniforms(const char* name, GLfloat* uniform) {
	list<shared_ptr<Program>>::iterator it;

	/* Iterate through all available programs */
  for (it = programs.begin(); it != programs.end(); ++it){
		shared_ptr<Program> program = (*it);

		/* Attach the uniform to the program */
		program->attachUniform(name, uniform);
	}
}

/* Issues a draw request to all programs */
void drawAllPrograms() {
	list<shared_ptr<Program>>::iterator it;

	/* Iterate through all programs */
  for (it = programs.begin(); it != programs.end(); ++it){
		shared_ptr<Program> program = (*it);

		/* Issue draw requests to all programs */
		program->draw();
	}
}

/* Updates the companion cube's position given an elapsed amount of seconds */
void updateCompanionCube(float elapsed_seconds) {
	companionCube->setPos(getCubePos(elapsed_seconds));
}

/* Updates the button components based on the arduino input data */
void updateButtonData(uint8_t* readData) {
	if (readData != NULL) {
		/* Only read the data if it's of non-zero size */
		if (sizeof(readData) > 0) {
			updateButtons(leftButton, readData[3]);
			updateButtons(botButton, readData[2]);
			updateButtons(rightButton, readData[1]);
			updateButtons(topButton, readData[0]);

			/* At data index 4 is located the detection of the contraption's floor.
			 	 When it detects a hit it must fizzle the companion cube and display it
				 back in the origin */
			if (readData[4]) {
				if (!isCubeFizzled) {
					fizzle->play();
					fizzleGlados->play();

					/* Make the cube drop from the ceiling */
					setCubePos(make_shared<Point>(0, 10, 15));
					isCubeFizzled = true;
				}
			} else {
				isCubeFizzled = false;
			}
		}
		free (readData);
	}

	/* Set the current target cube's texture to blue */
	targetButton->updateSubCompTexture(1);
}

/* Handle individual texture updates for buttons */
void updateButtons(shared_ptr<Component> btn, int val) {

	int textureVal = val*2;

	/* Change the texture of selected components to orange and plays selection
	  sound clip */
	if ((textureVal != btn->getTextureIndex()) || (btn->getTextureIndex() == 1 && textureVal == 2))  {
		switch (textureVal) {
			case 0:
				if(btn->getTextureIndex() != 1)
					buttonUp->play();
				break;
			case 2:
				buttonDown->play();
				break;
		}
	}

	/* Make texture updates cascade through the Component */
	btn->updateSubCompTexture(textureVal);

	/* Minimal game logic to switch around selected target cubes */
	if (val == 1 && btn == targetButton) {
		/* Pops the first available button from the list and pushes the current (now
	     old target) at the end. This way the code permanently cycles through
			 possible button choices */
		targetButton = availableButtons.front();
		availableButtons.pop_front();
		availableButtons.push_back(btn);

		/* Audio clips are cycled through in a similar fascion. */
		shared_ptr<sf::Sound> successAudioClip = successSounds.front();
		successSounds.pop_front();
		successAudioClip->play();

		/* The current playing audio clip can also be re-pushed onto the list */
		successSounds.push_back(successAudioClip);
	}
}

/* General function to generate all the virtual models present in the world.
   It operates using a number of textures and two main programs.
	 The main textured program will accept Components linked with textures.
	 The secondary unntextured program is mainly used for debugging purposes.
	 - - -
	 Certain generated Components are linked together into Buttons.
	 Buttons have common functionality as they share texture indexes. A change in
	 texture index is cascaded through the entire button and all of its Components
	 making texture-switching easier. */
void generateModels() {
	/* Initialise Program for textured Objects */
	shared_ptr<Program> texturedProgram = initProgram("assets/test_vs.glsl", "assets/test2_fs.glsl");

	/* Initialises companion cube texture */
	shared_ptr<Texture> cubeTexture = make_shared<Texture>();
	cubeTexture->addTexture("assets/img/companionCube.png");

	/* Initialises floor textures for wall components */
	shared_ptr<Texture> wallTexture = make_shared<Texture>();
	wallTexture->addTexture("assets/img/grid3.png");
	wallTexture->addTexture("assets/img/grid3b.png");

	/* Initialises textures for icons displayed on the starting pod */
	shared_ptr<Texture> iconTexture = make_shared<Texture>();
	iconTexture->addTexture("assets/img/Companion-Cube-portal-2.jpg");
	iconTexture->addTexture("assets/img/Chamber_icon_cake_on.png");

	/* Button textures evaluate the three different states of a button.
	   The gray texture is to be displayed in its inactive state.
		 The blue texture is to be displayed when it's selected as the target.
		 The orange texture indicates selection (triggered by hardware) */
	shared_ptr<Texture> buttonTexture = make_shared<Texture>();
	buttonTexture->addTexture("assets/img/grid0Gray.png");
	buttonTexture->addTexture("assets/img/grid0Blue.png");
	buttonTexture->addTexture("assets/img/grid0Orange.png");

	/* Following a similar fascion from the buttons, signal-light textures also
	   come in three colours */
	shared_ptr<Texture> signalTexture = make_shared<Texture>();
	signalTexture->addTexture("assets/img/portalSignalLightGray.png");
	signalTexture->addTexture("assets/img/portalSignalLightBlue.png");
	signalTexture->addTexture("assets/img/portalSignalLightOrange.png");

	/* ----- RIGHT BUTTON GENERATION ----- */
	texturedProgram->generateTexturedPrism(10, 10, 50, make_shared<Point>(10, 0, -26), wallTexture, 5);

	rightButton = make_shared<Component>();

	/* Actual "button" component */
	rightButton->addSubComponent(texturedProgram->generateTexturedPrism(5, 5, 2, make_shared<Point>(10, 0, -1.25f), buttonTexture, 1.25f));

	/* Side coloured trail */
	rightButton->addSubComponent(texturedProgram->generateTexturedPrism(0.5f, 0.5f, 0, make_shared<Point>(6.875f, -0.6f, -0.99f), signalTexture, 0));
	rightButton->addSubComponent(texturedProgram->generateTexturedPrism(0.5f, 0.5f, 0, make_shared<Point>(5.625f, -0.6f, -0.99f), signalTexture, 0));
	for (float i = -1; i >= -30; i-= 1.25f) {
	 	rightButton->addSubComponent(texturedProgram->generateTexturedPrism(0, 0.5f, 0.5f, make_shared<Point>(4.99f, -0.6f,  i - (1.25f/2)), signalTexture, 0));
	}

	/* ----- TOP BUTTON GENERATION ----- */
	texturedProgram->generateTexturedPrism(10, 10, 50, make_shared<Point>(0, 10, -26), wallTexture, 5);

	/* Companion Cube icon */
	texturedProgram->generateTexturedPrism(1.25f, 1.25f, 0.01f, make_shared<Point>(-3.13f, 6.85f, -0.99f), iconTexture, 0);

	/* Cake icon */
	shared_ptr<Component> cakeIcon = texturedProgram->generateTexturedPrism(1.25f, 1.25f, 0.01f, make_shared<Point>(3.13f, 6.85f, -0.99f), iconTexture, 0);
	cakeIcon->setTextureIndex(1);

	topButton = make_shared<Component>();

	/* Actual "button" component */
	topButton->addSubComponent(texturedProgram->generateTexturedPrism(5, 5, 2, make_shared<Point>(0, 10, -1.25f), buttonTexture, 1.25f));

	/* Side coloured trail */
	topButton->addSubComponent(texturedProgram->generateTexturedPrism(0.5f, 0.5f, 0, make_shared<Point>(-0.6f, 6.875f, -0.99f), signalTexture, 0));
	topButton->addSubComponent(texturedProgram->generateTexturedPrism(0.5f, 0.5f, 0, make_shared<Point>(-0.6f, 5.625f, -0.99f), signalTexture, 0));
	for (float i = -1; i >= -30; i-= 1.25f) {
	 	topButton->addSubComponent(texturedProgram->generateTexturedPrism(0.5f, 0, 0.5f, make_shared<Point>(-0.6f, 4.99f,  i - (1.25f/2)), signalTexture, 0));
	}

	/* ----- LEFT BUTTON GENERATION ----- */
	texturedProgram->generateTexturedPrism(10, 10, 50, make_shared<Point>(-10, 0, -26), wallTexture, 5);

	leftButton = make_shared<Component>();

	/* Actual "button" component */
	leftButton->addSubComponent(texturedProgram->generateTexturedPrism(5, 5, 2, make_shared<Point>(-10, 0, -1.25f), buttonTexture, 1.25f));

	/* Side coloured trail */
	leftButton->addSubComponent(texturedProgram->generateTexturedPrism(0.5f, 0.5f, 0, make_shared<Point>(-6.875f, -0.6f, -0.99f), signalTexture, 0));
	leftButton->addSubComponent(texturedProgram->generateTexturedPrism(0.5f, 0.5f, 0, make_shared<Point>(-5.625f, -0.6f, -0.99f), signalTexture, 0));
	for (float i = -1; i >= -30; i-= 1.25f) {
	 	leftButton->addSubComponent(texturedProgram->generateTexturedPrism(0, 0.5f, 0.5f, make_shared<Point>(-4.99f, -0.6f,  i - (1.25f/2)), signalTexture, 0));
	}

	/* ----- BOT BUTTON GENERATION ----- */
	texturedProgram->generateTexturedPrism(10, 10, 50, make_shared<Point>(0, -10, -26), wallTexture, 5);

	botButton = make_shared<Component>();

	/* Actual "button" component */
	botButton->addSubComponent(texturedProgram->generateTexturedPrism(5, 5, 2, make_shared<Point>(0, -10, -1.25f), buttonTexture, 1.25f));

	/* Side coloured trail */
	botButton->addSubComponent(texturedProgram->generateTexturedPrism(0.5f, 0.5f, 0, make_shared<Point>(-0.6f, -6.875f, -0.99f), signalTexture, 0));
	botButton->addSubComponent(texturedProgram->generateTexturedPrism(0.5f, 0.5f, 0, make_shared<Point>(-0.6f, -5.625f, -0.99f), signalTexture, 0));
	for (float i = -1; i >= -30; i-= 1.25f) {
	 	botButton->addSubComponent(texturedProgram->generateTexturedPrism(0.5f, 0, 0.5f, make_shared<Point>(-0.6f, -4.99f,  i - (1.25f/2)), signalTexture, 0));
	}

	/* Background gray component */
	shared_ptr<Component> background = texturedProgram->generateTexturedPrism(200, 200, 0, make_shared<Point>(0, 0, -16), wallTexture, 1);
	background->setTextureIndex(1);

	/* Companion cube */
	companionCube = texturedProgram->generateTexturedPrism(2, 2, 2, make_shared<Point>(0, 0, 0.75f), cubeTexture, 0);

	/* All initialised buttons are pushed onto the buttons list */
	availableButtons.push_back(rightButton);
	availableButtons.push_back(topButton);
	availableButtons.push_back(botButton);

	/* The top button is chosen as the first selected one.
	   Its texture is also set manually.
		 This is done to prevent the program from issuing a buttonDown sound when
		 retrieving the selected status data from arduino. */
	targetButton = leftButton;
	topButton->updateSubCompTexture(2);

	/* Initialise Program for blank objects. Templates and/or Grid */
	shared_ptr<Program> untexturedProgram = initProgram("assets/test2_vs.glsl", "assets/test_fs.glsl");

	/* Grid element --DEBUG */
	//untexturedProgram->generateGrid(100, 1.0f);
}

/* Initialises all sound components.
   Sound instances must be kept in memory for the whole duration of the play.
	 The following elements are all initialised at the top of this file.
	 TODO: Should be refactored into a seperate structure */
void initSound() {
	/* Intro sounds are played when entering the game.
	   GLaDOS voicelines are always played at full volume. */
	introGlados = loadSound("assets/audio/introComplete.wav", 100);
	introGlados->play();

	/* The portal theme song will be played in a loop constantly at a lower vol */
	backgroundTheme = loadSound("assets/audio/theme.wav", 25);
	backgroundTheme->setLoop(true);
	backgroundTheme->play();

	/* ButtonUp and ButtonDown sunds are played when triggered by the arduino
	   board. Placing the physical companionCUube onto one of the platforms will
		 trigger the cound to be played. */
	buttonUp = loadSound("assets/audio/buttonUp.wav", 50);
	buttonDown = loadSound("assets/audio/buttonDown.wav", 50);

	/* Fizzleing occurs when touching the bottom base element.
	   GLaDOS voicelines are played concurrently. */
	fizzle = loadSound("assets/audio/fizzle.wav", 100);
	fizzleGlados = loadSound("assets/audio/fizzleCubeComplete.wav", 100);

	/* Success Sounds are played in a loop upon successful placemenets of the
	   companionCube onto one of the target Platforms */
	successSounds.push_back(loadSound("assets/audio/testing01.wav", 100));
	successSounds.push_back(loadSound("assets/audio/testing02.wav", 100));
	successSounds.push_back(loadSound("assets/audio/testing03.wav", 100));
	successSounds.push_back(loadSound("assets/audio/testResults01.wav", 100));
}

/* loadSouns is a helper routine to load a .wav file from memory given its path.
   Paths must be relative to the callee of the function. */
shared_ptr<sf::Sound> loadSound(const char* path, int volume) {
	shared_ptr<sf::SoundBuffer> buffer = make_shared<sf::SoundBuffer>();

	/* Error handling */
	if (!buffer->loadFromFile(path)) {
		fprintf(stderr, "error loading sound file into buffer: %s", path);
    exit (-1);
	}

	/* Buffers must be kept in memory or the audio stored in them cannot be played. */
	buffer_list.push_back(buffer);

	/* A new sound object is initialised, linked to the buffer and returned */
	shared_ptr<sf::Sound> sound = make_shared<sf::Sound>();;
	sound->setBuffer(*buffer);

	/* Sound volume is set to the provided value */
	sound->setVolume(volume);
	return sound;
}
