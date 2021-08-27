from sklearn.base import BaseEstimator

import pyMixtComp.pyMixtCompBridge as pyMixtCompBridge
from pyMixtComp.bridge.convert import convert
from pyMixtComp.bridge.utils import create_algo


class MixtComp(BaseEstimator):
    def __init__(self, n_component, n_burn_in_iter=50, n_iter=50, n_gibbs_burn_in_iter=50, n_gibbs_iter=50,
                 n_init_per_class=50, n_sem_try=20, confidence_level=0.95,
                 ratio_stable_criterion=0.99, n_stable_criterion=20, n_run=1, verbose=True):
        self.n_component = n_component
        self.n_run = n_run
        self.verbose = verbose

        self.n_burn_in_iter = n_burn_in_iter
        self.n_iter = n_iter
        self.n_gibbs_burn_in_iter = n_gibbs_burn_in_iter
        self.n_gibbs_iter = n_gibbs_iter
        self.n_init_per_class = n_init_per_class
        self.n_sem_try = n_sem_try
        self.confidence_level = confidence_level
        self.ratio_stable_criterion = ratio_stable_criterion
        self.n_stable_criterion = n_stable_criterion

    def fit(self, X, model):
        algo = create_algo(self, X, "learn")

        self.res = pyMixtCompBridge.pmc(algo, X, model, {})
        convert(self.res)
        return self
