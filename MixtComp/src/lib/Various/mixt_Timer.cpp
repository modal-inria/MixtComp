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

namespace mixt {

Timer::Timer(std::string timerName) :
		timerName_(timerName) {
#ifdef MC_VERBOSE
	std::cout << timerName_ << ", timer created." << std::endl;
#endif

	creationTime_ = boost::posix_time::microsec_clock::local_time();
	lastTopTime_ = boost::posix_time::microsec_clock::local_time();
}
;

void Timer::iteration(Index iteration, Index iterationMax) {
	if (iteration == 0) {
		std::cout << timerName_ << ", timer initializing first iteration." << std::endl;
		firstIterationTime_ = boost::posix_time::microsec_clock::local_time();
	} else {
		Time currTime = boost::posix_time::microsec_clock::local_time();
		TimeDuration dt = currTime - firstIterationTime_;
		Real timePerIt = dt.total_milliseconds() / 1000. / Real(iteration);

		std::cout << timerName_ << " timer" << std::endl;
		std::cout << "iteration: " << iteration << "/" << iterationMax << std::endl;
		std::cout << "Mean time per iteration: " << timePerIt << std::endl;
		std::cout << "Estimated remaining time: " << (iterationMax - iteration + 1) * timePerIt << std::endl;
	}
}

Real Timer::top(std::string message) {
	Time currTime = boost::posix_time::microsec_clock::local_time();

	TimeDuration dtLastTop = currTime - lastTopTime_;
	Real sinceLastTop = dtLastTop.total_milliseconds() / 1000.;

	TimeDuration dtCreation = currTime - creationTime_;
	Real sinceCreation = dtCreation.total_milliseconds() / 1000.;

#ifdef MC_VERBOSE
	std::cout << timerName_ << " timer, " << message
	<< ", time since last top: " << lastTopTime
	<< " s, time since creation: " << sinceCreation << " s" << std::endl;
#endif

	lastTopTime_ = boost::posix_time::microsec_clock::local_time();
	return sinceLastTop;
}

void Timer::setName(std::string timerName) {
	timerName_ = timerName;
}

} // namespace mixt
