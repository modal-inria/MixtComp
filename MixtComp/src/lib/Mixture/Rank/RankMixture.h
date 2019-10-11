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
 *  Created on: September 23, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef LIB_MIXTURE_RANK_RANKMIXTURE_H
#define LIB_MIXTURE_RANK_RANKMIXTURE_H

#include <set>
#include <vector>
#include <utility>
#include <Data/ConfIntDataStat.h>
#include <IO/IOFunctions.h>
#include <IO/NamedAlgebra.h>
#include <IO/SpecialStr.h>
#include "RankClass.h"
#include "RankLikelihood.h"
#include "RankParser.h"
#include "RankStat.h"


namespace mixt {
/**
 * RankMixture contains an array of RankClass. Each RankClass will have the responsibility to perform
 * estimation of parameters and computation of the probability of individuals that belong to it.
 * */
template<typename Graph>
class RankMixture: public IMixture {
public:
	typedef std::pair<MisType, std::vector<int> > MisVal;

	RankMixture(const Graph& data, const Graph& param, Graph& out, std::string const& idName, Index nClass, Index nObs, Real confidenceLevel, const std::string& paramStr) :
			IMixture(idName, "Rank_ISR", nClass, nObs), nbPos_(0), facNbMod_(0.), confidenceLevel_(confidenceLevel), dataG_(data), paramG_(param), outG_(out), mu_(nClass), pi_(nClass), piParamStat_(
					pi_, confidenceLevel) {
		class_.reserve(nClass);
		muParamStat_.reserve(nClass);
		for (int k = 0; k < nClass; ++k) {
			class_.emplace_back(data_, mu_(k), pi_(k)); // doing that means that classInd_, mu_ and pi_ must not be resized in order to avoid incorrect behaviour at runtime
			muParamStat_.emplace_back(mu_(k), confidenceLevel);
		}

		acceptedType_.resize(nb_enum_MisType_);
		acceptedType_ << true,   // present_,
		true,  // missing_,
		true,  // missingFiniteValues_,
		false,  // missingIntervals_,
		false,  // missingLUIntervals_,
		false; // missingRUIntervals
	}

	void sampleUnobservedAndLatent(Index i, Index k) {
		data_(i).sampleY(mu_(k), pi_(k));
		data_(i).sampleX(mu_(k), pi_(k));
	}

	/** Note that MixtureComposer::checkNbIndPerClass already enforce that there is at least one observation per class, in order to properly estimate the proportions. */
	std::string checkSampleCondition(const Vector<std::set<Index> >& classInd) const {
		if (degeneracyAuthorizedForNonBoundedLikelihood)
			return "";

		for (int k = 0; k < nClass_; ++k) {
			bool Geq0 = true; // are all comparisons incorrect ? This would lead to pi = 1 in a maximum likelihood estimation and is to be avoided.
			bool GeqA = true; // are all comparisons correct ? This would lead to pi = 1 in a maximum likelihood estimation and is to be avoided.

			for (std::set<Index>::const_iterator it = classInd(k).begin(), itE = classInd(k).end(); it != itE; ++it) {
				int A, G;
				data_(*it).AG(mu_(k), A, G);
				if (A == 0) {
					GeqA = false;
				} else if (A == G) {
					Geq0 = false;
				} else {
					GeqA = false;
					Geq0 = false;
				}

				if (Geq0 == false && GeqA == false) { // there is enough variability on the validities of comparisons in this class to ensure that pi will be estimated inside the open support
					goto itKEnd;
				}
			}

			return "Error in variable: " + idName_
					+ " with Rank_ISR model. The comparisons are uniformly correct or invalid in at least one class. If the number of different observed values is quite low, try using a categorical model instead."
					+ eol;

			itKEnd: ; // jumping here means that the return above is skipped, for the current class
		}

		return "";
	}

	/**
	 * One of the peculiarity of the ISR model is that the space of ranks on which to optimize the likelihood is very large. At each mStep, several candidates
	 * mu are sampled, and the one which maximizes the likelihood of the observations is used as the estimated parameter. The sampling method used is a
	 * Gibbs sampler, which is a Markov Chain sampler that needs to be initialized. This is slightly different to the Ordinal model case, where the initialization
	 * of the parameters is only here to ensure that all individuals are valid (not all z at 0). In the Rank model initialization, mu is chosen among all the
	 * observed values of the class, while pi is initialized to a "neutral" value.
	 * */
	std::string mStep(const Vector<std::set<Index> >& classInd) {
		for (int k = 0; k < nClass_; ++k) {
			class_[k].mStep(classInd(k));
		}

		return "";
	}

	void storeSEMRun(Index iteration, Index iterationMax) {
		for (int k = 0; k < nClass_; ++k) {
			muParamStat_[k].sampleValue(iteration, iterationMax);
		}
		piParamStat_.sampleParam(iteration, iterationMax);

		if (iteration == iterationMax) // at last iteration, compute the observed probability distribution logProba_
				{
			for (int k = 0; k < nClass_; ++k) {
				muParamStat_[k].setExpectationParam(); // estimate mu parameter using mode / expectation
			}
			piParamStat_.setExpectationParam(); // estimate pi parameter using mode / expectation
		}
	}

	void storeGibbsRun(Index i, Index iteration, Index iterationMax) {
		if (!data_(i).allPresent()) {
			dataStat_[i].sampleValue(iteration, iterationMax);

			if (iteration == iterationMax) { // at last iteration, compute the observed probability distribution logProba_
				dataStat_[i].setExpectationParam();
			}
		}
	}

	Real lnCompletedProbability(Index i, Index k) const {
		return class_[k].lnCompletedProbabilityInd(i);
	}

	Real lnObservedProbability(Index i, Index k) const {
		return class_[k].lnObservedProbability(i);
	}

	void initData(Index i) {
		data_(i).removeMissing();
	}

	/**
	 * Parameters are initialized using dummy values
	 */
	void initParam() {
		std::vector<Index> v(nbPos_);
		std::iota(v.begin(), v.end(), 0);
		RankVal r(nbPos_);
		r.setO(v);

		for (Index k = 0; k < nClass_; ++k) {
			mu_(k) = r;
			pi_(k) = 0.75;
		}
	}

	void initializeMarkovChain(Index i, Index k) {
		for (Index n = 0; n < nbGibbsIniISR; ++n) {
			sampleUnobservedAndLatent(i, k);
		}
	}

	Index nbFreeParameter() const {
		return nClass_; // only the continuous pi_ parameter is taken into account, not the discrete mu_ parameter
	}

	void writeParameters() const {
		std::stringstream sstm;
		for (int k = 0; k < nClass_; ++k) {
			sstm << "Class: " << k << std::endl;
			sstm << "mu: " << mu_(k) << std::endl;
			sstm << "pi: " << pi_(k) << std::endl;
		}

#ifdef MC_VERBOSE
		std::cout << sstm.str() << std::endl;
#endif
	}

	std::string setDataParam(RunMode mode) {
		std::string warnLog;
		std::vector<std::string> dataStr;

		dataG_.get_payload( { }, idName_, dataStr);

		warnLog += parseRankStr(dataStr, minModality, nbPos_, data_); // convert the vector of strings to ranks
		warnLog += checkMissingType();
		if (warnLog.size() > 0) {
			return warnLog;
		}

		if (mode == prediction_) { // prediction mode
			paramG_.get_payload( { idName_ }, "paramStr", paramStr_); // overwrite paramStr_ obtained from desc

			for (Index k = 0; k < nClass_; ++k) {
				NamedMatrix<int> rank;
				paramG_.get_payload( { idName_, "mu", "stat", "k: " + std::to_string(k + minModality) }, "rank", rank);
				rank.mat_ -= minModality;
				mu_(k).setNbPos(rank.mat_.cols());
				mu_(k).setO(rank.mat_.row(0)); // the most probable rank has been written in the first line of the rank matrix
			}

			NamedMatrix<Real> pi;
			paramG_.get_payload( { idName_, "pi" }, "stat", pi);
			pi_ = pi.mat_.col(0); // the two other columns are dedicated to quantiles

			for (int k = 0; k < nClass_; ++k) {
				muParamStat_[k].setParamStorage();
			}
			piParamStat_.setParamStorage();
		}

		if (paramStr_.size() == 0) { // if paramStr_ not provided, must be generated from the data, for future use and export for prediction
			std::stringstream sstm;
			sstm << "nModality: " << nbPos_;
			paramStr_ = sstm.str();
		} else {
			int nPosStr = -1;

			std::string nModStr = std::string("nModality: *") + strPositiveInteger; // parse paramStr here. If empty, deduce from data, if not empty, check that data UPPER BOUND is compatible with this information
			std::regex nModRe(nModStr);
			std::smatch matchesVal;

			if (std::regex_match(paramStr_, matchesVal, nModRe)) { // value is present
				nPosStr = str2type<int>(matchesVal[1].str());
			} else {
				std::stringstream sstm;
				sstm << "Variable: " << idName_ << " parameter string is not in the correct format, which should be \"nModality: x\" " << "with x the number of modalities in the variable."
						<< std::endl;
				warnLog += sstm.str();
			}

			if (nbPos_ != nPosStr) {
				std::stringstream sstm;
				sstm << "Variable: " << idName_ << " has " << nPosStr << " modalities per rank in its descriptor (or the descriptor from learning, in case of prediction) " << "but has " << nbPos_
						<< " modalities in its data. Those two numbers must be equal." << std::endl;
				warnLog += sstm.str();
			}
		}

		dataStat_.reserve(nInd_);
		for (int i = 0; i < nInd_; ++i) {
			dataStat_.emplace_back(data_(i).xModif(), confidenceLevel_);
		}

		return warnLog;
	}

	void exportDataParam() const {
		NamedMatrix<int> exportData(nInd_, nbPos_, false);
		for (Index i = 0; i < nInd_; ++i) {
			exportData.mat_.row(i) = data_(i).x().o() + minModality;
		}
		outG_.add_payload( { "variable", "data", idName_ }, "completed", exportData);

		outG_.add_payload( { "variable", "param", idName_ }, "paramStr", paramStr_);

		for (Index k = 0; k < nClass_; ++k) { // since the param is described by a composite data type, it is simpler to regroup by class
			const std::list<std::pair<RankVal, Real>>& muStatStorage = muParamStat_[k].statStorageMu();
			Index nRank = muStatStorage.size();
			NamedMatrix<int> rank(nRank, nbPos_, false);
			NamedVector<Real> proba(nRank, false);

			Index i = 0;
			for (std::list<std::pair<RankVal, Real>>::const_iterator it = muStatStorage.begin(), itEnd = muStatStorage.end(); it != itEnd; ++it) {
				rank.mat_.row(i) = it->first.o() + minModality;
				proba.vec_(i) = it->second;
				++i;
			}

			outG_.add_payload( { "variable", "param", idName_, "mu", "stat", "k: " + std::to_string(k + minModality) }, "rank", rank);
			outG_.add_payload( { "variable", "param", idName_, "mu", "stat", "k: " + std::to_string(k + minModality) }, "proba", proba);
		}

		for (Index k = 0; k < nClass_; ++k) { // log of mu is also exported class by class
			const Vector<RankVal>& muLogStorage = muParamStat_[k].logStorageMu();
			Index nIt = muLogStorage.size();

			NamedMatrix<int> rank(nIt, nbPos_, false);

			for (Index i = 0; i < nIt; ++i) {
				rank.mat_.row(i) = muLogStorage(i).o() + minModality;
			}

			outG_.add_payload( { "variable", "param", idName_, "mu", "log" }, "k: " + std::to_string(k + minModality), rank);
		}

		Index nStat = piParamStat_.getStatStorage().cols();
		std::vector<std::string> colNames;
		quantileNames(nStat, confidenceLevel_, colNames);
		NamedMatrix<Real> piStatStorage = NamedMatrix<Real>( { piParamNames(), colNames, piParamStat_.getStatStorage() });
		outG_.add_payload( { "variable", "param", idName_, "pi" }, "stat", piStatStorage);

		NamedMatrix<Real> piLogStorage = {piParamNames(), std::vector<std::string>(), piParamStat_.getLogStorage() };
		outG_.add_payload( { "variable", "param", idName_, "pi" }, "log", piLogStorage);
	}

	void computeObservedProba() {
		for (int k = 0; k < nClass_; ++k) {
			class_[k].computeObservedProba();
		}
	}

	bool sampleApproximationOfObservedProba() {
		return true;
	}
private:
	std::string checkMissingType() {
		std::string warnLog;

		std::list<int> listInd;
		for (int i = 0; i < nInd_; ++i) {
			if (!data_(i).checkMissingType(acceptedType_)) {
				listInd.push_back(i);
			}
		}

		if (listInd.size() > 0) {
			std::stringstream sstm;
			sstm << "Rank variable " << idName_ << " contains individual described by missing data type not implemented yet. " << "The list of problematic individuals is: " << itString(listInd)
					<< std::endl;
			warnLog += sstm.str();
		}

		return warnLog;
	}

	std::vector<std::string> muParamNames() const {
		std::vector<std::string> names(nClass_);
		for (int k = 0; k < nClass_; ++k) {
			std::stringstream sstm;
			sstm << "k: " << k + minModality << ", mu";
			names[k] = sstm.str();
		}
		return names;
	}

	std::vector<std::string> piParamNames() const {
		std::vector<std::string> names(nClass_);
		for (int k = 0; k < nClass_; ++k) {
			std::stringstream sstm;
			sstm << "k: " << k + minModality << ", pi";
			names[k] = sstm.str();
		}
		return names;
	}

	int nbPos_;
	Real facNbMod_;

	Real confidenceLevel_;

	const Graph& dataG_;
	const Graph& paramG_;
	Graph& outG_;

	Vector<RankVal> mu_;
	Vector<Real> pi_;
	Vector<RankIndividual> data_;

	/** RankLikelihood object, used to for the harmonic mean estimation of the observed likelihood. */
	RankLikelihood rankLikelihood_;

	/** Matrix containing observed log probability distribution, used in harmonic mean estimation
	 * of the observed probability
	 * Individual in rows
	 * Classes in columns */
	Matrix<Real> observedProbaHMean_;

	std::vector<RankClass> class_;

	/** Each element of the vector keeps track of statistics for one particular mu */
	std::vector<RankStat> muParamStat_;

	/** Each element of the vector keeps track of statistics for one particular individual */
	std::vector<RankStat> dataStat_;

	/** Compute the statistics on pi parameter */
	ConfIntParamStat<Vector<Real> > piParamStat_;

	Vector<bool> acceptedType_;

	std::string paramStr_;
};

} // namespace mixt

#endif // RANKMIXTURE_H
