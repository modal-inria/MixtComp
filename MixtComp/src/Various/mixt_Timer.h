/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2013-2014

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 *  Project:    MixtComp
 *  Created on: November 6, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 **/

#ifndef MIXT_TIMER_H_
#define MIXT_TIMER_H_

#include <ctime>
#include <string>

namespace mixt
{

class Timer
{
  public:
    Timer(std::string timerName = "Timer");
    ~Timer();

    /** Estimated time to finish all iterations */
    void iteration(int iteration,
                   int iterationMax);
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
