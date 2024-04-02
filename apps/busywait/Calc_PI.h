// Task to Calculate Pi using the Francois Viete formula
// 
// This is meant as a simple example of using multiple
// cores where your UI is running in core 1 and
// something else is running in core 0.
//
#ifndef PI_H
#define PI_H

#include <BigNumber.h>

namespace Calc_PI {

  volatile bool  m_bFinish;
  BigNumber m_PI(0);   // Calculated PI value

  char* getPI() {
     return m_PI.toString(); // must free result!
  }

  bool isFinish() {
    return m_bFinish;
  }
  
///
/// Calculate Pi using the Francois Viete formula 
/// pi_task is designed to run in a separate core of the ESP32
///
/// \param[in]  arg:         Integer that specifies maximum number of iterations
///
/// \return NONE
/// 
/// When the calculation is completed it will set the boolean m_bFinish
/// to true and will make pi available as a char array by getPI() function.
/// The task is deleted at the end and can be restarted.
/// Since BigNumber uses malloc for pi's assignment you should used
/// free() after accessing pi.
///
  void pi_task(void *arg) {

    int nMaxIterations = (int)arg;
    Serial.print("ENTER Calc_PI::pi_task MaxIterations="); Serial.println(nMaxIterations);
    Serial.print("pi_task running on core ");
    Serial.println(xPortGetCoreID());

    BigNumber m_two (2);       // used as a constant value of 2
    BigNumber m_s(0);          // used in iteration
    BigNumber m_t(1);          // used in iteration
    
    m_bFinish = false;
    // Francois Viete formula
    for (int i=0; i<nMaxIterations; i++) {
//      Serial.print("Calc_PI::loop = "); Serial.println(i);
      BigNumber r = m_s + m_two; // temporary value
//      Serial.print("r = m_s + m_two = "); Serial.println(r);
      m_s = r.sqrt ();
      m_t *= m_s / m_two;
      m_PI = m_two / m_t;      // calculate pi
      // Note that this runs very fast so delay here for our demo
      delay(50);
    }
    m_bFinish = true;
    Serial.println("EXIT Calc_PI::pi_thread");
    delay(50); //Wait a little before killing task
    vTaskDelete( NULL );      // kill task
  }

};

#endif
