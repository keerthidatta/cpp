#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // Done: See src/processor.cpp

  // Done: Declare any necessary private members
 private:
   float prevIdle_ {0.0};
   float prevNonIdle_ {0.0};
};

#endif