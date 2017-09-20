#include "arduino_utils.hpp"

shared_ptr<ReferencePoint> lowerLevel;
shared_ptr<ReferencePoint> middleLevel;
shared_ptr<ReferencePoint> upperLevel;

int serialFd;

bool isSerialReady;

void setupArduino() {
  /* Bottom Level */
	shared_ptr<ArduinoPoint> lowerCenter = make_shared<ArduinoPoint>(95, 90, 75, 45, ArduinoPoint::CENTER);
	lowerLevel = make_shared<ReferencePoint>(lowerCenter, ReferencePoint::LOWER);

	lowerLevel->attachPoint(make_shared<ArduinoPoint>(90, 25, 130, 55, ArduinoPoint::TOP));
	lowerLevel->attachPoint(make_shared<ArduinoPoint>(95, 65, 90, 0, ArduinoPoint::RIGHT));
	lowerLevel->attachPoint(make_shared<ArduinoPoint>(110, 140, 80, 45, ArduinoPoint::BOT));
	lowerLevel->attachPoint(make_shared<ArduinoPoint>(110, 70, 90, 100, ArduinoPoint::LEFT));

	/* Middle level */
	shared_ptr<ArduinoPoint> middleCenter = make_shared<ArduinoPoint>(70, 90, 20, 45, ArduinoPoint::CENTER);
	middleLevel = make_shared<ReferencePoint>(middleCenter, ReferencePoint::MIDDLE);

	middleLevel->attachPoint(make_shared<ArduinoPoint>(20, 10, 50, 55, ArduinoPoint::TOP));
	middleLevel->attachPoint(make_shared<ArduinoPoint>(85, 70, 35, 0, ArduinoPoint::RIGHT));
	middleLevel->attachPoint(make_shared<ArduinoPoint>(90, 140, 20, 45, ArduinoPoint::BOT));
	middleLevel->attachPoint(make_shared<ArduinoPoint>(85, 75, 35, 100, ArduinoPoint::LEFT));

	/* Upper level */
	shared_ptr<ArduinoPoint> upperCenter = make_shared<ArduinoPoint>(50, 100, 0, 45, ArduinoPoint::CENTER);
	upperLevel = make_shared<ReferencePoint>(upperCenter, ReferencePoint::HIGHER);

	upperLevel->attachPoint(make_shared<ArduinoPoint>(0, 10, 30, 55, ArduinoPoint::TOP));
	upperLevel->attachPoint(make_shared<ArduinoPoint>(60, 70, 20, 0, ArduinoPoint::RIGHT));
	upperLevel->attachPoint(make_shared<ArduinoPoint>(90, 140, 0, 45, ArduinoPoint::BOT));
	upperLevel->attachPoint(make_shared<ArduinoPoint>(60, 70, 20, 100, ArduinoPoint::LEFT));
}

/* */
shared_ptr<ReferencePoint> determineArduinoDeltas(shared_ptr<Point> point) {
  shared_ptr<DeltaObject> retValue;

  shared_ptr<ReferencePoint> lowerRef;
  shared_ptr<ReferencePoint> upperRef;

  /* */
  if (point->z <= 10) {
    lowerRef = lowerLevel;
    upperRef = middleLevel;
  } else {
    lowerRef = middleLevel;
    upperRef = upperLevel;
  }

  shared_ptr<DeltaObject> lowerDelta = lowerRef->getValues(point);
  shared_ptr<DeltaObject> upperDelta = upperRef->getValues(point);

  float ZRatio = fmod(point->z, 5);
  float reverseRatio = (5 - ZRatio) / 5;
  ZRatio /= 5;

  shared_ptr<DeltaObject> innerDelta = getDeltaZDiff(lowerDelta, upperDelta, ZRatio, reverseRatio);

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

/* */
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
int
set_interface_attribs (int fd, int speed, int parity)
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

void
set_blocking (int fd, int should_block)
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

/* */
void setupSerial(const char* portname) {
  serialFd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
  isSerialReady = true;
	if (serialFd < 0)
	{
    fprintf(stderr, "Serial Port Error %d\n", errno);
    fprintf(stderr, "Could not open %s: %s", portname, strerror (errno));
    isSerialReady = false;
	}

  if (isSerialReady) {
  	set_interface_attribs (serialFd, B115200, 0);  // set speed to 115,200 bps, 8n1 (no parity)
  	set_blocking (serialFd, 0);                // set no blocking
  }
}

/* */
void sendByteData(uint8_t* data) {
  if (isSerialReady) {
    /* */
  	uint8_t bytes_to_send[1];
  	bytes_to_send[0] = 200;

    /* */
    write (serialFd, bytes_to_send, 1);
  	usleep ((1 + 25) * 100);

    /* */
  	write (serialFd, data, 5);
  	usleep ((5 + 25) * 100);

    free(data);
  }
}

/* */
void readByteData() {
  if (isSerialReady) {
    int bytes_avail;
    ioctl(serialFd, FIONREAD, &bytes_avail);
    if (bytes_avail >= 6) {
      uint8_t auth [1];
      read (serialFd, auth, sizeof auth);

      if (auth[0] == 201) {
        uint8_t buf [5];
        read (serialFd, buf, sizeof buf);  // read up to 100 characters if ready to read
        printf("%i %i %i %i %i\n", buf[0], buf[1], buf[2], buf[3], buf[4]);

      }
    }
  }
}
