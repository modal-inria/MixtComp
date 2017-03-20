/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: November 6, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

/** @file mixt_Constant.cpp
 *  @brief In this file we define various constants
 **/

#include <iostream>
#include "mixt_Timer.h"

namespace mixt
{

Timer::Timer(std::string timerName) :
    timerName_(timerName),
    startTime_(-1)
{
#ifdef MC_VERBOSE
  std::cout << timerName_ << ", timer created" << std::endl;
#endif

  std::time(&creationTime_);
  std::time(&topTime_);
};

void Timer::iteration(int iteration,
                      int iterationMax)
{
  if (iteration == 0)
  {
#ifdef MC_VERBOSE
    std::cout << timerName_ << " timer initializing" << std::endl;
#endif

    std::time(&startTime_);
  }
  else
  {
    std::time_t currTime;
    std::time(&currTime);

#ifdef MC_TIMERVERBOSE
    double timePerIt = std::difftime(currTime, startTime_) / (double)iteration;
    std::cout << timerName_ << " timer" << std::endl;
    std::cout << "iteration: " << iteration << "/" << iterationMax << std::endl;
    std::cout << "Mean time per iteration: " << timePerIt << std::endl;
    std::cout << "Estimated remaining time: " << (iterationMax - iteration + 1) * timePerIt << std::endl;
#endif
  }
}

double Timer::top(std::string message)
{
  std::time_t currTime;
  std::time(&currTime);
  double lastTopTime = std::difftime(currTime, topTime_);

#ifdef MC_VERBOSE
  std::cout << timerName_ << " timer, " << message
            << ", time since last top: " << lastTopTime
            << " s, time since creation: " << std::difftime(currTime, creationTime_) << " s" << std::endl;
#endif

  std::time(&topTime_);
  return lastTopTime;
}

void Timer::setName(std::string timerName)
{
  timerName_ = timerName;
}

} // namespace mixt
