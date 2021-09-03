from copy import deepcopy

import numpy as np

import pyMixtComp.pyMixtCompBridge as pyMixtCompBridge


def multi_run_pmc(algo, data, model, param, n_run):
    best_res = {"warnLog": "MixtComp failed for all runs."}
    best_crit = - np.inf
    for i in range(n_run):
        res = pyMixtCompBridge.pmc(algo, data, model, param)
        crit = - np.inf
        if "warnLog" not in res.keys():
            crit = res["mixture"]["lnObservedLikelihood"]

        if crit >= best_crit:
            best_res = deepcopy(res)
            best_crit = crit

    return best_res
