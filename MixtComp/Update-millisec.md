et le temps de calcule dtu mesures juste en secondes, si tu passes par boost tu peux le faire en millisecondes (je l'avais fait sur arcelor)
#include "boost/date_time.hpp"
void Timer::start()
{
    timeDuration_ = 0.;
    timerStart_ = boost::posix_time::microsec_clock::local_time();
}


void Timer::stop()
{
    timerStop_ = boost::posix_time::microsec_clock::local_time();

    boost::posix_time::time_duration duration = timerStop_ - timerStart_;

    timeDuration_ = duration.total_milliseconds()/1000.;
}
avec un truc comme ça
