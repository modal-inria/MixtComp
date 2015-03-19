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
    IMixture(std::string const& idName,
             Vector<int> const* p_zi,
             int nbCluster) :
      idName_(idName),
      nbCluster_(nbCluster),
      p_zi_(p_zi)
    {};
    /**copy constructor.
     * @note The pointer on the composer is not copied and is set to 0: it have
     * to be set again.
     * @param mixture the mixture to copy */
    IMixture(IMixture const& mixture) :
      idName_(mixture.idName_),
      nbCluster_(mixture.nbCluster_),
      p_zi_(mixture.p_zi_)
    {};
    /** Virtual destructor. */
    virtual ~IMixture()
    {};

    /** return the Id of the mixture */
    inline std::string const& idName() const { return idName_;}
    /** @return Number of cluster. */
    inline int nbCluster() const  { return nbCluster_;}


    /** @brief This function must be defined in derived class for initialization
     *  of the mixture parameters.
     *  This method should create any container needed by the model, resize
     *  them and initialize them.
     *  Since this method can be used when create is called, its main
     *  purpose should be to reset the mixture parameters, while leaving the
     *  data unchanged.
     *  This function will be called once the model is created and data is set.
     */
    virtual void initializeStep() = 0;
    /** @brief This function must be defined for simulation of all the latent
     * variables and/or missing data excluding class labels. The class labels
     * will be simulated by the framework itself because to do so we have to
     * take into account all the mixture laws.
     */
    virtual void samplingStep(int i) {/**Do nothing by default*/};
    /** @brief This function is equivalent to mStep and must be defined to update
     *  parameters.
     */
    virtual std::string mStep() = 0;
    /** @brief This function should be used to store results during the burn-in period.
     * @param iteration Provides the iteration number in the burn-in
     * period.
     */
    virtual void storeSEMBurnIn(int iteration,
                               int iterationMax) {/**Do nothing by default*/}
    /** @brief This function should be used to store any intermediate results
     * during various iterations after the burn-in period.
     * @param iteration Provides the iteration number beginning after the burn-in
     * period.
     */
    virtual void storeSEMRun(int iteration,
                              int iterationMax) {/**Do nothing by default*/}
    /** @brief This step can be used to store data. This is usually called after the long algo, to
     * store data generated using the estimated parameters
     */
    virtual void storeGibbsRun(int sample,
                           int iteration,
                           int iterationMax)
    {/**Do nothing by default*/}
    /** @brief This step can be used by developer to finalize any thing. It will
     *  be called only once after we finish running the estimation algorithm.
     */
    virtual void finalizeStep() {/**Do nothing by default*/}
    /** This function must be defined to return the posterior probability (PDF)
     * for corresponding sample_num and Cluster_num.
     * @param sample_num Sample number
     * @param Cluster_num Cluster number
     * @return the value of component probability in log scale
     */
    virtual double lnComponentProbability(int sample_num, int Cluster_num) = 0;
    /** This function must be defined to return the observed likelihood
     * @return the value of the observed likelihood in log scale
     */
    virtual void lnCompletedLikelihood(Matrix<Real>* lnComp) = 0;
    /** This function must be defined to return the observed likelihood
     * @return the value of the observed likelihood in log scale
     */
    virtual void lnObservedLikelihood(Matrix<Real>* lnComp) = 0;
    /** This function must return the number of free parameters.
     *  @return Number of free parameters
     */
    virtual int nbFreeParameter() const = 0;
    /** This function must return the number of variables (columns).
     *  @return Number of variables
     */
    virtual int nbVariable() const = 0;
    /** This function can be used to write summary of parameters on to the output stream.
     *  @param out Stream where you want to write the summary of parameters.
     */
    inline virtual void writeParameters(std::ostream& out) const
    {
#ifdef MC_DEBUG
    std::cout << _T("You need to override this method in your mixture!");
#endif
    }

    /** This function must be implemented to export data
     */
    virtual void exportDataParam() const = 0;

  protected:
    /** This function can be used in derived classes to get number of samples.
     *  @return Number of samples.
     */
    int nbSample() const;
    /** This function can be used in derived classes to get class labels from the framework.
     * @return Pointer to class labels.
     */
    int const* classLabels() const;
    /** This function can be used in derived classes to get proportions from the framework.
     *  @return Pointer to proportions.
     */
    Real const* proportions() const;
    /** This function can be used in derived classes to get the posterior probabilities
     *  from the framework.
     *  @return Pointer to tik.
     */
    Real const** posteriorProbabilities() const;
    /** This function can be used in derived classes to get class labels from the framework.
     *  @return Pointer to zi.
     */
    Vector<int> const* p_zi() const {return p_zi_;};

    /** Id name of the mixture */
    std::string idName_;
    /** number of cluster */
    int nbCluster_;

    /** Pointer to the zik class label */
    Vector<int> const* p_zi_;
};

} // namespace mixt

#endif /* MIXT_IMIXTURE_H */
