#include "arduino_utils.hpp"

/* Pointer references to the three height layers of the robotic arm.
   Could be stored in a list, yet iteration would just provide greater overhead */
shared_ptr<ReferencePoint> lowerLevel;
shared_ptr<ReferencePoint> middleLevel;
shared_ptr<ReferencePoint> upperLevel;

/* Serial port file descriptor used for handling */
int serialFd;

/* Boolean to keep track of serial successful initialisation status */
bool isSerialReady;

/* Keeps track of time intervals inbetween serial writes.
   Necessary to synch read and write time intervals */
double updateTime = 0;
float writeTimeInterval = 0.05f;

/* Helper function to initialise all virtual arduino compoonents.
   Initialises the various movement field locations and packages them up
   correctly into ReferencePoint pointers. */
void setupArduino() {
  /* Bottom Level */
	shared_ptr<ArduinoPoint> lowerCenter = make_shared<ArduinoPoint>(95, 90, 75, 45, ArduinoPoint::CENTER);
	lowerLevel = make_shared<ReferencePoint>(lowerCenter, ReferencePoint::LOWER);

	lowerLevel->attachPoint(make_shared<ArduinoPoint>(70, 25, 130, 55, ArduinoPoint::TOP));
	lowerLevel->attachPoint(make_shared<ArduinoPoint>(95, 65, 90, 0, ArduinoPoint::RIGHT));
	lowerLevel->attachPoint(make_shared<ArduinoPoint>(110, 140, 80, 45, ArduinoPoint::BOT));
	lowerLevel->attachPoint(make_shared<ArduinoPoint>(110, 70, 90, 100, ArduinoPoint::LEFT));

	/* Middle level */
	shared_ptr<ArduinoPoint> middleCenter = make_shared<ArduinoPoint>(70, 90, 20, 45, ArduinoPoint::CENTER);
	middleLevel = make_shared<ReferencePoint>(middleCenter, ReferencePoint::MIDDLE);

	middleLevel->attachPoint(make_shared<ArduinoPoint>(20, 10, 70, 55, ArduinoPoint::TOP));
	middleLevel->attachPoint(make_shared<ArduinoPoint>(85, 70, 35, 0, ArduinoPoint::RIGHT));
	middleLevel->attachPoint(make_shared<ArduinoPoint>(90, 140, 20, 45, ArduinoPoint::BOT));
	middleLevel->attachPoint(make_shared<ArduinoPoint>(85, 75, 35, 100, ArduinoPoint::LEFT));

	/* Upper level */
	shared_ptr<ArduinoPoint> upperCenter = make_shared<ArduinoPoint>(50, 100, 0, 45, ArduinoPoint::CENTER);
	upperLevel = make_shared<ReferencePoint>(upperCenter, ReferencePoint::HIGHER);

	upperLevel->attachPoint(make_shared<ArduinoPoint>(0, 10, 50, 55, ArduinoPoint::TOP));
	upperLevel->attachPoint(make_shared<ArduinoPoint>(60, 70, 20, 0, ArduinoPoint::RIGHT));
	upperLevel->attachPoint(make_shared<ArduinoPoint>(90, 140, 0, 45, ArduinoPoint::BOT));
	upperLevel->attachPoint(make_shared<ArduinoPoint>(60, 70, 20, 100, ArduinoPoint::LEFT));
}

/* The following function creates a new ReferencePoint that holds the correct
   servo-motor angle positions given the virtual position assumed by the camera
   on the board.
   Delta values are scaled on all three dimensions.
   By default the various height layers (Z-axis) are at heights 5 - 10 - 15 */
shared_ptr<ReferencePoint> determineArduinoDeltas(shared_ptr<Point> point) {
  /* The following pointers hold the references to the two ReferencePoint layers
     that will be used to compute the servo-motor angles */
  shared_ptr<ReferencePoint> lowerRef;
  shared_ptr<ReferencePoint> upperRef;

  /* According on the z-axis coordinate switch between the three stored layers */
  if (point->z <= 10) {
    lowerRef = lowerLevel;
    upperRef = middleLevel;
  } else {
    lowerRef = middleLevel;
    upperRef = upperLevel;
  }

  /* Determine and retrieve the inner delta values of the Reference Points */
  shared_ptr<DeltaObject> lowerDelta = lowerRef->getValues(point);
  shared_ptr<DeltaObject> upperDelta = upperRef->getValues(point);

  /* ZRatio and reverseRatio are used the scale the two layers inverse-proportionally */
  float ZRatio = fmod(point->z, 5);
  float reverseRatio = (5 - ZRatio) / 5;
  ZRatio /= 5;

  /* Retrieves a scaled DeltaObject given the above ratios */
  shared_ptr<DeltaObject> innerDelta = getDeltaZDiff(lowerDelta, upperDelta, ZRatio, reverseRatio);

  /* The following operations retrieve the servo-motor positions.
     They then apply the scaled deltas to the central positions obtaining the
     resulting values.
     Such values are then packaged firstly into a new ArduinoPoint and then into
     a new ReferencePoint object. */
  shared_ptr<ArduinoPoint> lowerCenter = lowerRef->getInnerArduinoData();
  shared_ptr<ArduinoPoint> upperCenter = upperRef->getInnerArduinoData();

  int newServo1 = lowerCenter->servo1 * reverseRatio + upperCenter->servo1 * ZRatio;
  int newServo2 = lowerCenter->servo2 * reverseRatio + upperCenter->servo2 * ZRatio;
  int newServo3 = lowerCenter->servo3 * reverseRatio + upperCenter->servo3 * ZRatio;
  int newServo4 = lowerCenter->servo4 * reverseRatio + upperCenter->servo4 * ZRatio;

  shared_ptr<ArduinoPoint> newPosition = make_shared<ArduinoPoint>(newServo1, newServo2, newServo3, newServo4, ArduinoPoint::CENTER);
  newPosition->delta = innerDelta;

  return make_shared<ReferencePoint>(newPosition, ReferencePoint::MIDDLE);
}

/* This functionn scales two DeltaObject values according to two different
   ratios. The sum of the two ratios must be 1.
   It returns a new DeltaObject storing the sum of the scaled deltas. */
shared_ptr<DeltaObject> getDeltaZDiff(shared_ptr<DeltaObject> lowerDelta, shared_ptr<DeltaObject> upperDelta, float ZRatio, float reverseRatio) {
  int newDelta1 = lowerDelta->getDelta1() * reverseRatio + upperDelta->getDelta1() * ZRatio;
  int newDelta2 = lowerDelta->getDelta2() * reverseRatio + upperDelta->getDelta2() * ZRatio;
  int newDelta3 = lowerDelta->getDelta3() * reverseRatio + upperDelta->getDelta3() * ZRatio;
  int newDelta4 = lowerDelta->getDelta4() * reverseRatio + upperDelta->getDelta4() * ZRatio;

  return make_shared<DeltaObject>(newDelta1, newDelta2, newDelta3, newDelta4, true);
}

/* ---------------------------- SERIAL PORT --------------------------------- */

/* The following functions to set up serial communication were taken from:
https://stackoverflow.com/questions/6947413/how-to-open-read-and-write-from-serial-port-in-c
 */

/* Helper function which sets the attributes for serial port communication */
int set_interface_attribs (int fd, int speed, int parity)
{
	struct termios tty;
	memset (&tty, 0, sizeof tty);
	if (tcgetattr (fd, &tty) != 0)
	{
		fprintf( stderr, "error %d from tcgetattr", errno);
	  return -1;
	}

	cfsetospeed (&tty, speed);
	cfsetispeed (&tty, speed);

	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
	// disable IGNBRK for mismatched speed tests; otherwise receive break
	// as \000 chars
	tty.c_iflag &= ~IGNBRK;         // disable break processing
	tty.c_lflag = 0;                // no signaling chars, no echo,
	                                // no canonical processing
	tty.c_oflag = 0;                // no remapping, no delays
	tty.c_cc[VMIN]  = 0;            // read doesn't block
	tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

	tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
	                                // enable reading
	tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
	tty.c_cflag |= parity;
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CRTSCTS;

	if (tcsetattr (fd, TCSANOW, &tty) != 0)
	{
	  fprintf( stderr, "error %d from tcsetattr", errno);
	  return -1;
	}
	return 0;
}

/* Helper funtion to set blocking serial port communication */
void set_blocking (int fd, int should_block)
{
	struct termios tty;
	memset (&tty, 0, sizeof tty);
	if (tcgetattr (fd, &tty) != 0)
	{
    fprintf( stderr, "error %d from tggetattr", errno);
    return;
	}

	tty.c_cc[VMIN]  = should_block ? 1 : 0;
	tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

	if (tcsetattr (fd, TCSANOW, &tty) != 0)
    fprintf(stderr, "error %d setting term attributes", errno);
}

/* Serial port setup function.
   Given a port name it opens the port if available.
   Should no device be listening on said port the program keeps running but all
   serial communication will be disabled. */
void setupSerial(const char* portname) {
  serialFd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
  isSerialReady = true;

  /* Open will return -1 as a filedescriptor should there be an error */
	if (serialFd < 0)
	{
    fprintf(stderr, "Serial Port Error %d\n", errno);
    fprintf(stderr, "Could not open %s: %s\n", portname, strerror (errno));
    isSerialReady = false;
	}

  /* If serial has been opened correctly proceed to set Baudrate and blocking */
  if (isSerialReady) {
    /* BaudRate is set to 115200 bps with no parity check */
  	set_interface_attribs (serialFd, B115200, 0);

    /* Serial communication will be non blocking */
  	set_blocking (serialFd, 0);
  }
}

/* Sends 5 serialised bytes of data over the serial port.
   Bytes are uint8_t values allowed between 0 <= x < 200 */
void sendByteData(uint8_t* data, double timeData) {
  /* Keep track of time intervals inbetween updates */
  double deltaTime = timeData - updateTime;

  /* Serial must be intialised */
  if (isSerialReady && deltaTime >= writeTimeInterval) {
    /* An extra byte is sent prior to the 5 data bytes.
       The value 200 is sent to authenticate the data as sent from the PC. */
  	uint8_t bytes_to_send[1];
  	bytes_to_send[0] = 200;

    /* The authentication byte is written over the serial port */
    write (serialFd, bytes_to_send, 1);
  	usleep ((1 + 25) * 100);

    /* The actual serialised data is written over the serial port.
       A minimal sleep delay is necessary to process the write command */
  	write (serialFd, data, 5);
  	usleep ((5 + 25) * 100);

    /* The data byte was malloced and must be freed */
    free(data);

    /* Update the last-update.time with the new timeData */
    updateTime = timeData;
  }
}

/* Reads and returns 5 bytes of data from the Serial Port */
uint8_t* readByteData() {
  if (isSerialReady) {
    /* Determines the number of bytes pending on the serial port */
    int bytes_avail;
    ioctl(serialFd, FIONREAD, &bytes_avail);

    /* Should there be more than 6 bytes reading can proceed */
    if (bytes_avail >= 6) {
      /* The authentication byte is read */
      uint8_t auth [1];
      read (serialFd, auth, sizeof auth);

      /* We only accept data sent by Arduino with auth code 201 */
      if (auth[0] == 201) {
        /* Allocates a new buffer to store the data to be read */
        uint8_t* buf = (uint8_t*) malloc(sizeof (uint8_t) * 5);
        read (serialFd, buf, sizeof buf);  // read up to 100 characters if ready to read

        /* Serial Read data is printed to stout */
        printf("%i %i %i %i %i\n", buf[0], buf[1], buf[2], buf[3], buf[4]);

        /* Return the newly initialised buffer. Must be freed later */
        return buf;
      }
    }
  }

  /* Returns NULL when no read can be performed */
  return NULL;
}
