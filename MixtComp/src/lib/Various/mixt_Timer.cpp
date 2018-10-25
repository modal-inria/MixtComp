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

Timer::Timer() {
	creationTime_ = boost::posix_time::microsec_clock::local_time();
	lastTopTime_ = boost::posix_time::microsec_clock::local_time();
}

Timer::Timer(const std::string& timerName) :
		timerName_(timerName) {
	std::cout << timerName_ << ", start" << std::endl;

	creationTime_ = boost::posix_time::microsec_clock::local_time();
	lastTopTime_ = boost::posix_time::microsec_clock::local_time();
}

void Timer::iteration(Index iteration, Index iterationMax) {
	if (iteration == 0) {
		std::cout << timerName_ << ", it.: 1" << "/" << iterationMax + 1 << std::endl;
		firstIterationTime_ = boost::posix_time::microsec_clock::local_time();
	} else {
		Time currTime = boost::posix_time::microsec_clock::local_time();
		TimeDuration dt = currTime - firstIterationTime_;
		Real timePerIt = dt.total_milliseconds() / 1000. / Real(iteration);

		std::cout << timerName_ << ", it.: " << iteration + 1 << "/" << iterationMax + 1 << ", mean / it.: " << timePerIt << ", est. rem.: " << (iterationMax - iteration + 1) * timePerIt << std::endl;
	}
}

Real Timer::top(std::string message) {
	Time currTime = boost::posix_time::microsec_clock::local_time();

	TimeDuration dtLastTop = currTime - lastTopTime_;
	Real sinceLastTop = dtLastTop.total_milliseconds() / 1000.;

	TimeDuration dtCreation = currTime - creationTime_;
	Real sinceCreation = dtCreation.total_milliseconds() / 1000.;

	std::cout << timerName_ << ", " << message << ", since last top: " << sinceLastTop << ", since creation: " << sinceCreation << std::endl;

	lastTopTime_ = boost::posix_time::microsec_clock::local_time();
	return sinceLastTop;
}

void Timer::setName(std::string timerName) {
	std::cout << timerName << ", start" << std::endl;
	timerName_ = timerName;
}

Real Timer::finish() {
	Time currTime = boost::posix_time::microsec_clock::local_time();
	TimeDuration dtCreation = currTime - creationTime_;
	Real sinceCreation = dtCreation.total_milliseconds() / 1000.;

	std::cout << timerName_ << ", finished. Total time elapsed: " << sinceCreation << std::endl;

	return sinceCreation;
}

} // namespace mixt
