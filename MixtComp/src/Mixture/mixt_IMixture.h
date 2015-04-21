/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2014

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
 *  Created on: July 2, 2014
 *  Authors:    Serge IOVLEFF <serge.iovleff@inria.fr>
 *              Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/


#ifndef MIXT_IMIXTURE_H
#define MIXT_IMIXTURE_H

namespace mixt
{

class IMixture
{
  public:
    /**Constructor with identification character
     * @param idName Identification string of Mixture allocated by framework.
     * @param nbCluster number of cluster
     */
    IMixture(std::string const& idName) :
      idName_(idName)
    {};
    /**copy constructor.
     * @note The pointer on the composer is not copied and is set to 0: it have
     * to be set again.
     * @param mixture the mixture to copy */
    IMixture(IMixture const& mixture) :
      idName_(mixture.idName_)
    {};
    /** Virtual destructor. */
    virtual ~IMixture()
    {};

    /** return the Id of the mixture */
    std::string const& idName() const
    {
      return idName_;
    }

    /** @brief This function must be defined for simulation of all the latent
     * variables and/or missing data excluding class labels. The class labels
     * will be simulated by the framework itself because to do so we have to
     * take into account all the mixture laws.
     */
    virtual void samplingStep(int ind)
    = 0;
    /** @brief This function is equivalent to mStep and must be defined to update
     *  parameters.
     */
    virtual std::string mStep()
    = 0;
    /** @brief This function should be used to store results during the burn-in period.
     * @param iteration Provides the iteration number in the burn-in
     * period.
     */
    virtual void storeSEMBurnIn(int iteration,
                               int iterationMax)
    = 0;
    /** @brief This function should be used to store any intermediate results
     * during various iterations after the burn-in period.
     * @param iteration Provides the iteration number beginning after the burn-in
     * period.
     */
    virtual void storeSEMRun(int iteration,
                              int iterationMax)
    = 0;
    /** @brief This step can be used to store data. This is usually called after the long algo, to
     * store data generated using the estimated parameters
     */
    virtual void storeGibbsRun(int sample,
                           int iteration,
                           int iterationMax)
    = 0;

    /** This function must be defined to return the observed likelihood
     * @return the value of the observed likelihood in log scale
     */
    virtual Real lnCompletedLikelihood(int i, int k)
    = 0;
    /** This function must be defined to return the observed likelihood
     * @return the value of the observed likelihood in log scale
     */
    virtual Real lnObservedLikelihood(int i, int k)
    = 0;
    /** This function must return the number of free parameters.
     *  @return Number of free parameters
     */
    virtual int nbFreeParameter() const
    = 0;
    /** This function can be used to write summary of parameters on to the output stream.
     *  @param out Stream where you want to write the summary of parameters.
     */
    virtual void writeParameters(std::ostream& out) const
    = 0;
    /** This function must be implemented to export data
     */
    virtual void exportDataParam() const
    = 0;

  protected:
    /** Id name of the mixture */
    std::string idName_;
};

} // namespace mixt

#endif /* MIXT_IMIXTURE_H */
