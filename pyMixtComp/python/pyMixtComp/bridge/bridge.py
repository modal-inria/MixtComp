"""
Run the c++ executable with python parameters
"""
from functools import partial
from multiprocessing import Pool

import numpy as np

from .. import pyMixtCompBridge


def run_pmc(i, algo, data, model, param):

    return pyMixtCompBridge.pmc(algo, data, model, param)


def multi_run_pmc_pool(algo, data, model, param, n_run, n_core):
    """ Run pmc multiple times

    The best model according to the observed loglikelihood is kept

    Parameters
    ----------
    algo : dict
        Algorithm parameters
    data : dict
        Each key corresponds to a variable with data as a list of str
    model : dict
        Model used for each parameters
    param : dict
        dict with learnt model
    n_run : int
        Number of runs
    n_core : int
        Number of cores for multiprocessing
    """
    p = Pool(n_core)
    run_pmc_i = partial(run_pmc, algo=algo, data=data, model=model, param=param)
    out = p.map(run_pmc_i, range(n_run))

    loglikelihood = np.full((n_run, ), -np.inf)
    for i in range(n_run):
        if "warnLog" not in out[i].keys():
            loglikelihood[i] = out[i]["mixture"]["lnObservedLikelihood"]

    ind_max = np.argmax(loglikelihood)

    return out[ind_max]
