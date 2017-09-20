#ifndef ARDUINO_POINT_H
#define ARDUINO_POINT_H

#include "../Util/Util.hpp"
#include "DeltaObject.hpp"

/* Arduino points store servo configurations for given Points on the board.
   They will compute and also stores scaled deltas. */
class ArduinoPoint {
public:
  /* Enum to determine the position of the point on the board.
     By convention Top is +10X, Bot it -10Y. Center is (0X,0Y)*/
  enum Cardinals {
    CENTER,
    TOP,
    RIGHT,
    BOT,
    LEFT
  };

  /* Public fields */
  int servo1, servo2, servo3, servo4;
  shared_ptr<DeltaObject> delta;

  ArduinoPoint::Cardinals cardinal;

  /* Constructor */
  ArduinoPoint(int, int, int, int, ArduinoPoint::Cardinals);

  /* Public methods */
  void computeDeltas(shared_ptr<ArduinoPoint>);
  void setExtensionAngle(int angle);
  void print();

  uint8_t* createBuffer();
};

#endif
