from copy import deepcopy
from multiprocessing import cpu_count

import numpy as np
from pandas.core.frame import DataFrame
from scipy.special import logsumexp
from sklearn.base import BaseEstimator
from sklearn.utils.validation import check_is_fitted, check_scalar

from pyMixtComp.bridge.bridge import multi_run_pmc_pool
from pyMixtComp.bridge.convert import convert, convert_data_to_dict
from pyMixtComp.bridge.utils import create_algo, format_data_basic_mode, format_model, impute_model, format_output_basic_mode
import pyMixtComp.plot as plot
import pyMixtComp.utils as utils


class MixtComp(BaseEstimator):
    """ Mixture model

    Estimate the parameter of a mixture model or predict the cluster of new samples.
    It manages heterogeneous data as well as missing and incomplete data.
    It uses a SEM-Gibbs algorithm to estimate parameters and impute missing data.

    Parameters
    ----------
    n_components : int
        The number of mixture components.
    n_burn_in_iter : int, optional
        Number of iterations of the burn-in part of the SEM algorithm, by default 50
    n_iter : int, optional
        Number of iterations of the SEM algorithm, by default 50
    n_gibbs_burn_in_iter : int, optional
        Number of iterations of the burn-in part of the Gibbs algorithm, by default 50
    n_gibbs_iter : int, optional
        Number of iterations of the Gibbs algorithm, by default 50
    n_init_per_class : int, optional
        Number of individuals used to initialize each cluster, by default 50
    n_sem_try : int, optional
        Number of try of the algorithm for avoiding an error, by default 20
    confidence_level : float, optional
        Confidence level for confidence bounds for parameter estimation, by default 0.95
    ratio_stable_criterion : float, optional
        Required proportion of the partition to stay stable to stop earlier the SEM, by default 0.99
    n_stable_criterion : int, optional
        Number of consecutive iterations of stability to stop earlier the SEM, by default 20
    n_init : int, optional
        The number of initializations to perform, by default 1. The best results are kept.
    n_core : int, optional
        Number of cores used to parallelize initializations, by default None

    Attributes
    ----------
    model_ : dict
        Model used for fitting
    res_ : dict
        Output of a fit
    res_predict_ : dict
        Output of a predict
    n_features_ : int
        Number of variables (features)
    n_samples_ : int
        Number of individuals (samples)

    Notes
    -----
    ### Data format (X)

    Data can be provided as:
    - a dict where each key is a variable name and each value is a 1d-array.
    - a DataFrame where each columns is a variable
    - a numpy array

    Values can be given as a string in order to provide information about incomplete data.
    Otherwise, data are automatically converted to string.

    The format for the different model are:
    - Gaussian data:
    Gaussian data are real values with the dot as decimal separator.
    Missing data are indicated by a `?`. Partial data can be provided through intervals denoted by
    `[a:b]` where `a` (resp. `b`) is a real or `-inf` (resp. `+inf`).

    - Categorical Data:
    Categorical data must be consecutive integer with 0 as minimal value. Missing data are indicated by a `?`.
    For partial data, a list of possible values can be provided by `{a_1,...,a_j}`,
    where `a_i` denotes a categorical value.

    - Poisson and NegativeBinomial Data:
    Poisson and NegativeBinomial data must be positive integer. Missing data are indicated by a `?`.
    Partial data can be provided through intervals denoted by
    `[a:b]` where `a` and `b` are  positive integers. `b` can be `+inf`.

    - Weibull Data:
    Weibull data are real positive values with the dot as decimal separator.
    Missing data are indicated by a `?`. Partial data can be provided through intervals denoted by
    `[a:b]` where `a` and `b` are  positive reals. `b` can be `+inf`.

    - Rank data:
    The format of a rank is: `o_1, ..., o_j` where `o_1` is an integer corresponding to the number of the object ranked
    in 1st position.
    For example: 3,1,0,2 means that the object number 3 is ranked first then the object number 1 is in second position
    and so on.
    Missing data can be specified by replacing and object by a `?` or a list of potential object, for example:
    `3, {1 2}, {1 0}, ?` means that the object ranked in second position is either the object number 1 or the object number 2,
    then the object ranked in third position is either the object 1 or 0 and the last one can be anything.
    A totally missing rank is specified by `?,?,...,?`

    - Functional data:
    The format of a functional data is: `time_1:value_1,..., time_j:value_j`.
    Between individuals, functional data can have different length and different time.
    `i` is the number of subregressions in a functional data and `k` the number of coefficients
    of each regression (2 = linear, 3 = quadratic, ...). Missing data are not supported.

    - z_class:
    To perform a (semi-)supervised clustering, user can add a variable named `z_class` (with eventually some missing values)
    with "LatentClass" as model. Missing data are indicated by a `?`.
    For partial data, a list of possible values can be provided by `{a_1,...,a_j}`, where `a_i` denotes a class number.

    ### model

    `model` is a dict indicating which model is used for each variable. Each key is a variable name.
    All variables listed in the `model` object must be in the `X` object. `model` can contain less variables than `X`.
    An element of the dict is the model's name to use (see below for the list of available models).
    For example, `model = {"real1": "Gaussian", "counting1": "Poisson"}` indicates a mixture model with 2 variables named
    real1 and counting1 with Gaussian and Poisson as model.

    Some models require hyperparameters. In this case, the model is described by a dict of 2 elements: "type" containing
    the model name and "paramStr" containing the hyperparameters.
    For example: `model = {"func1": {"type": "Func_CS", "paramStr": "nSub: 4, nCoeff: 2"}, "counting1": "Poisson"}`.
    If `model` is None, data are supposed to be provided in a DataFrame or a dict with dtypes
    (float, int, object, np.nan as missing value for float).
    Models will be imputed as follows: "Gaussian" for float, "Multinomial" for object and "Poisson" for int.

    Eight models are available in MixtComp: Gaussian, Multinomial, Poisson, NegativeBinomial, Weibull, Func_CS,
    Func_SharedAlpha_CS, Rank_ISR.
    Func_CS and Func_SharedAlpha_CS models require hyperparameters: the number of subregressions of functional and the number
    of coefficients of each subregression. These hyperparameters are specified by: `nSub: i, nCoeff: k` in the `paramStr` field
    of the `model` object. Func_SharedAlpha_CS is a variant of the Func_CS model with the alpha parameter shared between
    clusters. It means that the start and end of each subregression will be the same across the clusters.

    Examples
    --------
    >>> import matplotlib.pyplot as plt
    >>> from pyMixtComp import MixtComp
    >>> from pyMixtComp.data import load_prostate
    >>> data, model = load_prostate()
    >>> data.head()

    Prostate data are mixed data (real and categorical). All variables are stored as object, "?" indicates missing values.
    And the model dict indicates which model used for every variable.

    >>> model
    >>> mod = MixtComp(n_components=2, n_init=5)
    >>> mod.fit(data[:-50], model)

    Plot the discriminative power of variables to find the variables that participate the most at the clustering task.

    >>> mod.plot_discriminative_power_variable()
    >>> plt.show()

    SG and AP are the most discriminant variables.

    >>> mod.get_param("SG")

    Predict the cluster of the 50 last data

    >>> mod.predict(data[-50:], model)
    """
    def __init__(self, n_components, n_burn_in_iter=50, n_iter=50, n_gibbs_burn_in_iter=50, n_gibbs_iter=50,
                 n_init_per_class=50, n_sem_try=20, confidence_level=0.95,
                 ratio_stable_criterion=0.99, n_stable_criterion=20, n_init=1, n_core=None):

        self._check_init(n_components, n_burn_in_iter, n_iter, n_gibbs_burn_in_iter, n_gibbs_iter,
                         n_init_per_class, n_sem_try, confidence_level, ratio_stable_criterion,
                         n_stable_criterion, n_init, n_core)

        self.n_components = n_components
        self.n_init = n_init

        self.n_core = 1
        if n_core is None:
            self.n_core = np.min((np.max((1, cpu_count() - 1)), n_init))
        else:
            self.n_core = np.min((n_init, n_core, cpu_count()))

        self.n_burn_in_iter = n_burn_in_iter
        self.n_iter = n_iter
        self.n_gibbs_burn_in_iter = n_gibbs_burn_in_iter
        self.n_gibbs_iter = n_gibbs_iter
        self.n_init_per_class = n_init_per_class
        self.n_sem_try = n_sem_try
        self.confidence_level = confidence_level
        self.ratio_stable_criterion = ratio_stable_criterion
        self.n_stable_criterion = n_stable_criterion

    def _check_init(self, n_components, n_burn_in_iter, n_iter, n_gibbs_burn_in_iter, n_gibbs_iter,
                    n_init_per_class, n_sem_try, confidence_level, ratio_stable_criterion,
                    n_stable_criterion, n_init, n_core):
        check_scalar(n_components, min_val=1, name="n_components", target_type=int)
        check_scalar(n_burn_in_iter, min_val=1, name="n_burn_in_iter", target_type=int)
        check_scalar(n_iter, min_val=1, name="n_iter", target_type=int)
        check_scalar(n_gibbs_burn_in_iter, min_val=1, name="n_gibbs_burn_in_iter", target_type=int)
        check_scalar(n_gibbs_iter, min_val=1, name="n_gibbs_iter", target_type=int)
        check_scalar(n_init_per_class, min_val=1, name="n_init_per_class", target_type=int)
        check_scalar(n_sem_try, min_val=1, name="n_sem_try", target_type=int)
        check_scalar(confidence_level, min_val=0, max_val=1, name="confidence_level", target_type=float)
        check_scalar(ratio_stable_criterion, min_val=0, max_val=1, name="ratio_stable_criterion", target_type=float)
        check_scalar(n_stable_criterion, min_val=1, name="n_stable_criterion", target_type=int)
        check_scalar(n_init, min_val=1, name="n_init", target_type=int)
        if n_core is not None:
            check_scalar(n_core, min_val=1, name="n_core", target_type=int)

    def _check_X(self, X):
        if not (isinstance(X, dict) | isinstance(X, np.ndarray) | isinstance(X, DataFrame)):
            raise TypeError("X must be a dict with variable names as key, an array or a DataFrame.")

    def _check_model(self, model):
        if not (isinstance(model, dict) | (model is None)):
            raise TypeError("model must be a dict with variable names as key.")

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
        self._check_X(X)
        self._check_model(model)

        self._basic_mode = (model is None)

        if self._basic_mode:
            model = impute_model(X)

        self.model_ = model
        format_model(self.model_)

        dat = {}
        if self._basic_mode:
            dat, self.dictionary_ = format_data_basic_mode(X, self.model_, None)
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
        check_is_fitted(self, attributes="res_")
        self._check_X(X)

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
        check_is_fitted(self, attributes="res_")
        return plot.plot_discriminative_power_variable(self.res_, class_id)

    def plot_discriminative_power_class(self):
        """Plot the discriminative power of classes

        Returns
        -------
        Axes
            barplot
        """
        check_is_fitted(self, attributes="res_")
        return plot.plot_discriminative_power_class(self.res_)

    def plot_proportion(self):
        """ Plot a barplot with proportion's mixture

        Returns
        -------
        Axes
            barplot
        """
        check_is_fitted(self, attributes="res_")
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
        check_is_fitted(self, attributes="res_")
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
        check_is_fitted(self, attributes="res_")
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
        check_is_fitted(self, attributes="res_")
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
        check_is_fitted(self, attributes="res_")
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
        check_is_fitted(self, attributes="res_")
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
        check_is_fitted(self, attributes="res_")
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
        check_is_fitted(self, attributes="res_")
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
        check_is_fitted(self, attributes="res_")
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
        check_is_fitted(self, attributes="res_")
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
        check_is_fitted(self, attributes="res_")
        if X is None:
            return self.res_["mixture"]["lnObservedLikelihood"]
        else:
            _ = self.predict(X)
            return self.res_predict_["mixture"]["lnObservedLikelihood"]

    def get_param(self, var_name):
        """ Get the estimated parameters

        Parameters
        ----------
        var_name : str
            variable name to get params

        Returns
        -------
        DataFrame, dict
            The parameters are mainly returned as a DataFrame with each row corresponding to a component
        """
        check_is_fitted(self, attributes="res_")
        return utils.get_param(self.res_, var_name)
