/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>
 **/

/*
 *  Project:    MixtComp
 *  Created on: November 6, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

/** @file Timer.cpp
 *  @brief In this file we define various constants
 **/

#include <Various/Timer.h>
#include <iostream>

namespace mixt {

Timer::Timer() {
	creationTime_ = boost::posix_time::microsec_clock::local_time();
	lastTopTime_ = boost::posix_time::microsec_clock::local_time();
}

Timer::Timer(const std::string& timerName) :
		timerName_(timerName) {
#ifdef MC_VERBOSE
	std::cout << timerName_ << ", start" << std::endl;
#endif

	creationTime_ = boost::posix_time::microsec_clock::local_time();
	lastTopTime_ = boost::posix_time::microsec_clock::local_time();
}

void Timer::iteration(Index iteration, Index iterationMax) {
	if (iteration == 0) {
#ifdef MC_VERBOSE
		std::cout << timerName_ << ", it.: 1" << "/" << iterationMax + 1 << std::endl;
#endif
		firstIterationTime_ = boost::posix_time::microsec_clock::local_time();
	} else {
		Time currTime = boost::posix_time::microsec_clock::local_time();
		TimeDuration dt = currTime - firstIterationTime_;
		Real timePerIt = dt.total_milliseconds() / 1000. / Real(iteration);

#ifdef MC_VERBOSE
		std::cout << timerName_ << ", it.: " << iteration + 1 << "/" << iterationMax + 1 << ", mean / it.: " << timePerIt << ", est. rem.: " << (iterationMax - iteration + 1) * timePerIt << std::endl;
#endif
	}
}

Real Timer::top(std::string message) {
	Time currTime = boost::posix_time::microsec_clock::local_time();

	TimeDuration dtLastTop = currTime - lastTopTime_;
	Real sinceLastTop = dtLastTop.total_milliseconds() / 1000.;

	TimeDuration dtCreation = currTime - creationTime_;
	Real sinceCreation = dtCreation.total_milliseconds() / 1000.;

#ifdef MC_VERBOSE
	std::cout << timerName_ << ", " << message << ", since last top: " << sinceLastTop << ", since creation: " << sinceCreation << std::endl;
#endif

	lastTopTime_ = boost::posix_time::microsec_clock::local_time();
	return sinceLastTop;
}

void Timer::setName(std::string timerName) {
#ifdef MC_VERBOSE
	std::cout << timerName << ", start" << std::endl;
#endif
	timerName_ = timerName;
}

Real Timer::finish() {
	Time currTime = boost::posix_time::microsec_clock::local_time();
	TimeDuration dtCreation = currTime - creationTime_;
	Real sinceCreation = dtCreation.total_milliseconds() / 1000.;

#ifdef MC_VERBOSE
	std::cout << timerName_ << ", finished. Total time elapsed: " << sinceCreation << std::endl;
#endif

	return sinceCreation;
}

} // namespace mixt
