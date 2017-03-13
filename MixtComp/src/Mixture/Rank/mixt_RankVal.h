/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 26, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_RANKVAL_H
#define MIXT_RANKVAL_H

#include <iostream>
#include "../LinAlg/mixt_LinAlg.h"

namespace mixt
{

/**
 * Both ordering (position -> modality) and ranking (modality -> position) are used in the RankCluster
 * algorithm. RankVal stores and update both representations, allowing easy operations on rank values. It
 * could be used to store both completed values of individuals or central rank in the parameters.
 */
class RankVal
{
  public:
    RankVal();

    RankVal(int nbPos);

    /** Set ordering via initializer list */
    RankVal(const std::initializer_list<int>& il);

    void operator=(const RankVal& rv);

    bool operator==(const RankVal& rv) const;

    bool operator<(const RankVal& rv) const;

    void setNbPos(int nbPos);

    int nbPos() const {return nbPos_;}

    /** Note that no check is performed on sizes to increase speed */
    template<typename T>
    void setO(const T& data)
    {
      typename T ::const_iterator itD  = data     .begin();
      typename T ::const_iterator itDE = data     .end  ();
      Vector<int>::iterator       itV  = ordering_.begin();

      for (;
           itD != itDE;
           ++itD, ++itV)
      {
        *itV = *itD;
      }

      switchRepresentation(ordering_,
                           ranking_);
    }

    /** Note that no check is performed on sizes to increase speed */
    template<typename T>
    void setR(const T& data)
    {
      typename T ::const_iterator itD  = data    .begin();
      typename T ::const_iterator itDE = data    .end  ();
      Vector<int>::iterator       itV  = ranking_.begin();

      for (;
           itD != itDE;
           ++itD, ++itV)
      {
        *itV = *itD;
      }

      switchRepresentation(ranking_,
                           ordering_);
    }

    void switchRepresentation(const Vector<int>& mu ,
                                    Vector<int>& muP) const;

    /**
     * Permutation of contiguous elements
     * @param firstElem index of the first element in ordering representation */
    void permutation(int firstElem);

    const Vector<int>& o() const {return ordering_;}

    const Vector<int>& r() const {return ranking_;}

    friend std::ostream& operator<<(std::ostream& os, const RankVal& rv);

    std::string str() const;
  private:
    /** */
    int nbPos_;

    /** ordering representation */
    Vector<int> ordering_;

    /** ranking representation */
    Vector<int> ranking_;
};

std::ostream& operator<<(std::ostream& os, const RankVal& rv);

} // namespace mixt

#endif // MIXT_RANKVAL_H
