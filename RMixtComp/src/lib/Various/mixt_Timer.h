/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: November 6, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 **/

#ifndef MIXT_TIMER_H_
#define MIXT_TIMER_H_

#include <ctime>
#include <string>

namespace mixt {

class Timer {
public:
	Timer(std::string timerName = "Timer");

	/** Estimated time to finish all iterations */
	void iteration(int iteration, int iterationMax);

	/** Time elapsed since creation of timer */
	double top(std::string message);

	void setName(std::string timerName);

private:
	std::string timerName_;

	std::time_t creationTime_;

	std::time_t startTime_;

	std::time_t topTime_;
};

} // namespace mixt

#endif /* MIXT_CONSTANTS_H_ */
