#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include "linux_parser.h"

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
    float prevtotaltime = 0;
    float previdlealltime = 0;

};

#endif