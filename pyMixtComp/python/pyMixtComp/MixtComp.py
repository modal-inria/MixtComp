from copy import deepcopy
from multiprocessing import cpu_count

import numpy as np
from scipy.special import logsumexp
from sklearn.base import BaseEstimator

from pyMixtComp.bridge.bridge import multi_run_pmc_pool
from pyMixtComp.bridge.convert import convert, convert_data_to_dict
from pyMixtComp.bridge.utils import create_algo, format_data_basic_mode, format_model, impute_model, format_output_basic_mode
import pyMixtComp.plot as plot
import pyMixtComp.utils as utils


class MixtComp(BaseEstimator):
    def __init__(self, n_components, n_burn_in_iter=50, n_iter=50, n_gibbs_burn_in_iter=50, n_gibbs_iter=50,
                 n_init_per_class=50, n_sem_try=20, confidence_level=0.95,
                 ratio_stable_criterion=0.99, n_stable_criterion=20, n_init=1, n_core=None):
        self.n_components = n_components
        self.n_init = n_init

        self.n_core = np.min((cpu_count() - 1, n_init))
        if n_core is not None:
            self.n_core = np.min((self.n_core, n_core))

        self.n_burn_in_iter = n_burn_in_iter
        self.n_iter = n_iter
        self.n_gibbs_burn_in_iter = n_gibbs_burn_in_iter
        self.n_gibbs_iter = n_gibbs_iter
        self.n_init_per_class = n_init_per_class
        self.n_sem_try = n_sem_try
        self.confidence_level = confidence_level
        self.ratio_stable_criterion = ratio_stable_criterion
        self.n_stable_criterion = n_stable_criterion

    def fit(self, X, model=None):
        """ Estimate model parameters with the SEM algorithm.

        Parameters
        ----------
        X : dict, array or DataFrame
            [description]
        model : dict, optional
            dict containing the variables to use in the model, by default None.
            Each key corresponds to a variable name (in the case where X is a dict or a DataFrame), and each value to a model.
            Available models are: "Gaussian", "Multinomial", "Poisson", "NegativeBinomial", "Weibull", "Rank_ISR", "Func_CS"
            and "Func_SharedAlpha_CS". "Func_CS" and "Func_SharedAlpha_CS" require hyperparameters, their values must
            be {type: "Func_CS", paramStr: "nSub: a, nCoeff: b"} where a is the number of subregressions and b the the number
            of coefficients of each regression.
            None is tolerated when a numpy array or a DataFrame is provided.
            In the numpy array case, all variables are assumed to be gaussian. In the DataFrame case, model are imputed
            regarding the column type: float => Gaussian, int => Poisson, object => categorical.


        Returns
        -------
        MixtComp
            self

        Raises
        ------
        RuntimeError
            Error from the C++ algorithm.
        """
        self._basic_mode = (model is None)

        if self._basic_mode:
            model = impute_model(X)

        self.model_ = model
        format_model(self.model_)

        dat = {}
        if self._basic_mode:
            dat, self.dictionary_ = format_data_basic_mode(X, model, None)
        else:
            dat = convert_data_to_dict(X)

        algo = create_algo(self, dat, "learn")

        self.res_ = multi_run_pmc_pool(algo, dat, self.model_, {}, self.n_init, self.n_core)

        if "warnLog" in self.res_.keys():
            raise RuntimeError(self.res_["warnLog"])

        self._param = {"variable": {"param": deepcopy(self.res_["variable"]["param"])}}
        convert(self.res_)
        if self._basic_mode:
            format_output_basic_mode(self.res_, self.dictionary_)

        self.n_features_ = len(self.res_["variable"]["type"]) - 1
        self.n_samples_ = algo["nInd"]

        return self

    def predict(self, X):
        """Predict the labels for the data samples in X using trained model.

        Parameters
        ----------
        X : dict, array, DataFrame
            List of n_features-dimensional data points. Each row corresponds to a single data point.

        Returns
        -------
        array
            Component labels.
        """
        dat = {}
        if self._basic_mode:
            dat, _ = format_data_basic_mode(X, self.model_, self.dictionary_)
        else:
            dat = convert_data_to_dict(X)

        algo = create_algo(self, dat, "predict")

        self.res_predict_ = multi_run_pmc_pool(algo, dat, self.model_, self._param, self.n_init, self.n_core)
        convert(self.res_predict_)
        if self._basic_mode:
            format_output_basic_mode(self.res_predict_, self.dictionary_)

        return self.res_predict_["variable"]["data"]["z_class"]["completed"]

    def plot_discriminative_power_variable(self, class_id=None):
        """ Plot the discriminative power of variables

        Parameters
        ----------
        class_id : int, optional
            Class number (0, ...,  n_components - 1), by default None.
            If None, it returns the discriminative power of variables globally otherwise it returns the discriminative power
            of variables in the given class

        Returns
        -------
        Axes
            Barplot
        """
        return plot.plot_discriminative_power_variable(self.res_, class_id)

    def plot_discriminative_power_class(self):
        """Plot the discriminative power of classes

        Returns
        -------
        Axes
            barplot
        """
        return plot.plot_discriminative_power_class(self.res_)

    def plot_proportion(self):
        """ Plot a barplot with proportion's mixture

        Returns
        -------
        Axes
            barplot
        """
        return plot.plot_proportion(self.res_)

    def plot_class_similarity(self):
        """ Plot the similarity between classes

        Returns
        -------
        Axes
            Heatmap of the similarity

        Notes
        -----
        The similarities between classes k and g is defined by 1 - Sigma(k,g) where Sigma is:
        .. math::
        Sigma(k,g)^2 = (1/n) * \sum_{i=1}^n (P(Z_i=k|x_i) - P(Z_i=g|x_i))^2

        A high value (close to one) means that the classes are highly similar (high overlapping).
        A low value (close to zero) means that the classes are highly different (low overlapping).
        """
        return plot.plot_class_similarity(self.res_)

    def plot_variable_similarity(self):
        """ Plot the similarity between variables

        Returns
        -------
        Axes
            Heatmap of the similarity

        Notes
        -----
        The similarities between variables j and h is defined by Delta(j, h) where Delta is:
        .. math::
        Delta(j,h)^2 = 1 - \sqrt{(1/n) * \sum_{i=1}^n \sum_{k=1}^K (P(Z_i=k|x_{ij}) - P(Z_i=k|x_{ih}))^2}

        A high value (close to one) means that the variables provide the same information for the clustering task
        (i.e. similar partitions).
        A low value (close to zero) means that the variables provide some different information for the clustering task
        (i.e. different partitions).
        """
        return plot.plot_variable_similarity(self.res_)

    def plot_tik(self):
        """ Heatmap of the tik = P(Z_i=k|x_i)

        Observation are sorted according to the hard partition then for each component
        they are sorted by decreasing order of their tik

        Returns
        -------
        Axes
            Heatmap of the tik
        """
        return plot.plot_tik(self.res_)

    def plot_data_CI(self, var_name, class_ids=None, all=False):
        """ Mean and 95%-level confidence intervals per class

        Parameters
        ----------
        var_name : str
            Variable name
        class_ids : int, array-like of int, optional
            Classes to plot, by default None, all classes are plotted
        all : bool, optional
            If True, it adds the mean and 95%-level confidence interval for all data, by default False

        Returns
        -------
        Axes
            Plot Mean and 95%-level confidence intervals per class
        """
        return plot.plot_data_CI(self.res_, var_name, class_ids, all)

    def plot_data(self, var_name, class_ids=None, all=False):
        """ Boxplot per class

        Parameters
        ----------
        var_name : str
            Variable name
        class_ids : int, array-like of int, optional
            Classes to plot, by default None, all classes are plotted
        all : bool, optional
            If True, it adds the mean and 95%-level confidence interval for all data, by default False

        Returns
        -------
        Axes
            Boxplot (with 5%-25%-50%-75%-95% quantile) of the data
        """
        return plot.plot_data(self.res_, var_name, class_ids, all)

    def aic(self, X=None):
        """ Akaike information criterion for the current model on the input X.

        Parameters
        ----------
        X : DataFrame, dict or array, optional
            Data used to run predict(). If None the aic of the fitted model is returned

        Returns
        -------
        float
            AIC value
        """
        if X is None:
            return self.res_["mixture"]["lnObservedLikelihood"] - self.res_["mixture"]["nbFreeParameters"]
        else:
            _ = self.predict(X)
            return self.res_predict_["mixture"]["lnObservedLikelihood"] - self.res_predict_["mixture"]["nbFreeParameters"]

    def bic(self, X=None):
        """ Bayesian information criterion for the current model on the input X.

        Parameters
        ----------
        X : DataFrame, dict or array, optional
            Data used to run predict(). If None the bic of the fitted model is returned

        Returns
        -------
        float
            BIC value
        """
        if X is None:
            return self.res_["mixture"]["BIC"]
        else:
            _ = self.predict(X)
            return self.res_predict_["mixture"]["BIC"]

    def icl(self, X=None):
        """ Integrated Complete-data Likelihood for the current model on the input X.

        Parameters
        ----------
        X : DataFrame, dict or array, optional
            Data used to run predict(). If None the icl of the fitted model is returned

        Returns
        -------
        float
            ICL value
        """
        if X is None:
            return self.res_["mixture"]["ICL"]
        else:
            _ = self.predict(X)
            return self.res_predict_["mixture"]["ICL"]

    def predict_proba(self, X=None, log=True, empiric=False):
        """Predict posterior probability of each component given the data.

        Parameters
        ----------
        X : dict, array, DataFrame, optional
            List of n_features-dimensional data points. Each row corresponds to a single data point.
            If None, the data used in fit() are used.
        log : bool
            If True, the log probabilities are returned
        empiric : bool
            If True, the probabilities estimated during the Gibbs step are turned. Otherwise, the probabilities are computed
            with estimated parameters.

        Returns
        -------
        array
            Returns the probability given each sample.
        """
        if X is None:
            return utils.get_tik(self.res_, log, empiric)
        else:
            _ = self.predict(X)
            return utils.get_tik(self.res_predict_, log, empiric)

    def fit_predict(self, X, model=None):
        """Estimate model parameters using X and predict the labels for X.

        The method fits the model n_init times and sets the parameters with
        which the model has the largest likelihood.
        After fitting, it predicts the most probable label for the input data points.

        Parameters
        ----------
        X : dict, array, DataFrame
            List of n_features-dimensional data points. Each row corresponds to a single data point.

        Returns
        -------
        array
            Component labels.
        """
        _ = self.fit(X, model)

        return self.res_["variable"]["data"]["z_class"]["completed"]

    def score_samples(self, X=None):
        """Compute the weighted log probabilities for each sample.

        Parameters
        ----------
        X : dict, array, DataFrame, optional
            List of n_features-dimensional data points. Each row corresponds to a single data point.
            If None, the data used in fit() are used.

        Returns
        -------
        array
            Log probabilities of each data point in X.
        """
        if X is None:
            return logsumexp(self.res_["mixture"]["lnProbaGivenClass"], axis=1)
        else:
            _ = self.predict(X)
            return logsumexp(self.res_predict_["mixture"]["lnProbaGivenClass"], axis=1)

    def score(self, X=None):
        """Compute the per-sample average log-likelihood of the given data X.

        Parameters
        ----------
        X : dict, array, DataFrame, optional
            List of n_features-dimensional data points. Each row corresponds to a single data point.
            If None, the data used in fit() are used.

        Returns
        -------
        float
            Log likelihood of the mixture given X.
        """
        if X is None:
            return self.res_["mixture"]["lnObservedLikelihood"]
        else:
            _ = self.predict(X)
            return self.res_predict_["mixture"]["lnObservedLikelihood"]
