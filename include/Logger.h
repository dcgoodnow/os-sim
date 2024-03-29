/* Logger.h
 *
 * Last Modified: Sat 25 Apr 2015 02:03:37 AM PDT
*/
#ifndef __LOGGER_H
#define __LOGGER_H

#include <string>

/* Logger
 * ======
 *
 * This is an abstract class for logging messages. It allows users of this class
 * to abstract the methods for logging. Through polymorphism, this class allows 
 * users to dynamically write to different outputs without writing separate code
 * for each message.
*/
class Logger
{
   protected:
      pthread_mutex_t m_OutputMutex;

   public:
      //Prints a message
      virtual void print(const std::string& message) = 0;

      //Prints a message and appends a newline character (\n)
      virtual void println(const std::string& message) = 0;

      //Destructor needed to allow child classes to implement destructors. 
      virtual ~Logger(){}
};


#endif // __LOGGER_H
