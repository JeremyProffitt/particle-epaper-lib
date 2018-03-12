/* ePaper library by Jeremy Proffitt <proffitt.jeremy@gmail.com>
 */

#include "ePaper.h"

/**
 * Constructor.
 */
EPaper::EPaper()
{
  // be sure not to call anything that requires hardware be initialized here, put those in begin()
}

/**
 * Example method.
 */
void EPaper::begin()
{
    // initialize hardware
    Serial.println("called begin");
}

/**
 * Example method.
 */
void EPaper::process()
{
    // do something useful
    Serial.println("called process");
    doit();
}

/**
* Example private method
*/
void EPaper::doit()
{
    Serial.println("called doit");
}
