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
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 **/

#ifndef TIMER_H_
#define TIMER_H_

#include <boost/date_time/posix_time/posix_time.hpp>
#include <LinAlg/Typedef.h>

#include <string>


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

#endif /* TIMER_H_ */
