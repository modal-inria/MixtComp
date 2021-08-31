from copy import deepcopy

from sklearn.base import BaseEstimator

import pyMixtComp.pyMixtCompBridge as pyMixtCompBridge
from pyMixtComp.bridge.convert import convert, convert_data_to_dict
from pyMixtComp.bridge.utils import create_algo, format_model, create_model


class MixtComp(BaseEstimator):
    def __init__(self, n_components, n_burn_in_iter=50, n_iter=50, n_gibbs_burn_in_iter=50, n_gibbs_iter=50,
                 n_init_per_class=50, n_sem_try=20, confidence_level=0.95,
                 ratio_stable_criterion=0.99, n_stable_criterion=20):
        self.n_components = n_components

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
        dat = convert_data_to_dict(X)
        algo = create_algo(self, dat, "learn")

        self.model = model
        if self.model is None:
            self.model = create_model(var_names=list(dat.keys()))
        else:
            format_model(self.model)

        self.res = pyMixtCompBridge.pmc(algo, dat, self.model, {})

        if "warnLog" in self.res.keys():
            raise RuntimeError(self.res["warnLog"])

        self._param = {"variable": {"param": deepcopy(self.res["variable"]["param"])}}
        convert(self.res)

        return self

    def predict(self, X):
        dat = convert_data_to_dict(X)
        algo = create_algo(self, dat, "predict")

        self.res_predict = pyMixtCompBridge.pmc(algo, dat, self.model, self._param)

        convert(self.res_predict)

        return self.res_predict["variable"]["data"]["z_class"]["completed"]
