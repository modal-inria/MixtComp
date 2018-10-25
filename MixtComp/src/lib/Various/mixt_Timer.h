/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: November 6, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 **/

#ifndef MIXT_TIMER_H_
#define MIXT_TIMER_H_

#include <boost/date_time/posix_time/posix_time.hpp>

#include <string>

#include <LinAlg/mixt_Typedef.h>

namespace mixt {

/**
 * Timer can be used both:
 * - to provide information on iterations via the iteration method
 * - to provide information since creation via the top method
 *
 * https://kuniganotas.wordpress.com/2011/01/14/measuring-time-with-boost-library/
 */
class Timer {
public:
	typedef boost::posix_time::ptime Time;
	typedef boost::posix_time::time_duration TimeDuration;

	Timer();

	Timer(const std::string& timerName);

	/** Estimated time to finish all iterations */
	void iteration(Index iteration, Index iterationMax);

	/**
	 * Time elapsed since creation of timer, in seconds
	 *
	 * @param message provides a context for the output. */
	Real top(std::string message);

	void setName(std::string timerName);

	/**
	 * End of use of the timer, display message with total time spent since creation.
	 */
	Real finish();
private:
	std::string timerName_;

	/** Time at creation. */
	Time creationTime_;

	/** Time since first iteration. */
	Time firstIterationTime_;

	/** Time since the last top, or creation, which one is the more recent. */
	Time lastTopTime_;
};

} // namespace mixt

#endif /* MIXT_CONSTANTS_H_ */
