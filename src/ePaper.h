#pragma once

/* ePaper library by Jeremy Proffitt <proffitt.jeremy@gmail.com>
 */

// This will load the definition for common Particle variable types
#include "Particle.h"

// This is your main class that users will import into their application
class EPaper
{
public:
  /**
   * Constructor
   */
  EPaper();

  /**
   * Example method
   */
  void begin();

  /**
   * Example method
   */
  void process();

private:
  /**
   * Example private method
   */
  void doit();
};
