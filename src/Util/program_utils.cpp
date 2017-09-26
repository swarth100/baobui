#include "program_utils.hpp"

/* List of active programs to be rendered */
list<shared_ptr<Program>> programs;

/* */
shared_ptr<Component> topButton;
shared_ptr<Component> rightButton;
shared_ptr<Component> botButton;
shared_ptr<Component> leftButton;
bool isCubeFizzled = false;

/* */
shared_ptr<Component> targetButton;
list<shared_ptr<Component>> availableButtons;

/* */
shared_ptr<Component> companionCube;

/* */
shared_ptr<sf::Sound> backgroundTheme;
shared_ptr<sf::Sound> buttonUp;
shared_ptr<sf::Sound> buttonDown;
shared_ptr<sf::Sound> fizzle;

/* */
list<shared_ptr<sf::SoundBuffer>> buffer_list;

/* */
shared_ptr<Program> initProgram(const char* vs, const char* fs) {
	shared_ptr<Program> program = make_shared<Program>(vs, fs);

	programs.push_back(program);

	return program;
}

/* */
void attachUniforms(const char* name, GLfloat* uniform) {
	list<shared_ptr<Program>>::iterator it;
  for (it = programs.begin(); it != programs.end(); ++it){
		shared_ptr<Program> program = (*it);

		program->attachUniform(name, uniform);
	}
}

/* */
void drawAllPrograms() {
	list<shared_ptr<Program>>::iterator it;
  for (it = programs.begin(); it != programs.end(); ++it){
		shared_ptr<Program> program = (*it);

		program->draw();
	}
}

/* */
void updateCompanionCube(float elapsed_seconds) {
	companionCube->setPos(getCubePos(elapsed_seconds));
}

/* */
void updateButtonData(uint8_t* readData) {
	if (readData != NULL) {
		if (sizeof(readData) > 0) {
			updateButtons(leftButton, readData[3]);
			updateButtons(botButton, readData[2]);
			updateButtons(rightButton, readData[1]);
			updateButtons(topButton, readData[0]);

			/* */
			if (readData[4]) {
				if (!isCubeFizzled) {
					fizzle->play();
				}
				setCubePos(make_shared<Point>(0, 10, 15));
				isCubeFizzled = true;
			} else {
				isCubeFizzled = false;
			}
		}
		free (readData);
	}

	targetButton->updateSubCompTexture(1);
}

/* */
void updateButtons(shared_ptr<Component> btn, int val) {

	int textureVal = val*2;

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

	btn->updateSubCompTexture(textureVal);

	if (val == 1 && btn == targetButton) {
		targetButton = availableButtons.front();
		availableButtons.pop_front();
		availableButtons.push_back(btn);
	}
}

/* */
void generateModels() {
	/* Initialise Program for textured Objects */
	shared_ptr<Program> texturedProgram = initProgram("assets/test_vs.glsl", "assets/test2_fs.glsl");

	/* */
	shared_ptr<Texture> cubeTexture = make_shared<Texture>();
	cubeTexture->addTexture("assets/img/companionCube.png");

	/* */
	shared_ptr<Texture> wallTexture = make_shared<Texture>();
	wallTexture->addTexture("assets/img/grid3.png");
	wallTexture->addTexture("assets/img/grid3b.png");

	/* */
	shared_ptr<Texture> iconTexture = make_shared<Texture>();
	iconTexture->addTexture("assets/img/Companion-Cube-portal-2.jpg");
	iconTexture->addTexture("assets/img/Chamber_icon_cake_on.png");

	/* */
	shared_ptr<Texture> buttonTexture = make_shared<Texture>();
	buttonTexture->addTexture("assets/img/grid0Gray.png");
	buttonTexture->addTexture("assets/img/grid0Blue.png");
	buttonTexture->addTexture("assets/img/grid0Orange.png");

	/* */
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

	/* */
	availableButtons.push_back(rightButton);
	availableButtons.push_back(topButton);
	availableButtons.push_back(botButton);

	targetButton = leftButton;
	topButton->updateSubCompTexture(2);

	/* Initialise Program for blank objects. Templates and/or Grid */
	shared_ptr<Program> untexturedProgram = initProgram("assets/test2_vs.glsl", "assets/test_fs.glsl");

	/* Grid element */
	//untexturedProgram->generateGrid(100, 1.0f);
}

/* */
void initSound() {
	/* */
	backgroundTheme = loadSound("assets/audio/theme.wav", 25);
	backgroundTheme->setLoop(true);
	backgroundTheme->play();

	/* */
	buttonUp = loadSound("assets/audio/buttonUp.wav", 50);

	/* */
	buttonDown = loadSound("assets/audio/buttonDown.wav", 50);

	fizzle = loadSound("assets/audio/fizzle.wav", 100);
}

/* */
shared_ptr<sf::Sound> loadSound(const char* path, int volume) {
	shared_ptr<sf::SoundBuffer> buffer = make_shared<sf::SoundBuffer>();
  if (!buffer->loadFromFile(path)) {
		fprintf(stderr, "error loading sound file into buffer: %s", path);
    exit (-1);
	}

	buffer_list.push_back(buffer);

	shared_ptr<sf::Sound> sound = make_shared<sf::Sound>();;
	sound->setBuffer(*buffer);
	sound->setVolume(volume);
	return sound;
}
