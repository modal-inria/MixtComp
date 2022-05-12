import re

import numpy as np
import pandas as pd
from scipy.special import logsumexp


def get_tik(res, log=False, empiric=False):
    """ Get the posterior probability of each component given the data

    Parameters
    ----------
    res : dict
        output of multi_run_pmc_pool
    log : bool, optional
        It True, the log-tik are returned, by default False
    empiric : bool, optional
        If True, the probabilities estimated during the Gibbs step are turned. Otherwise, the probabilities are computed
        with estimated parameters.

    Returns
    -------
    array
        posterior probability
    """
    if empiric:
        tik = res["variable"]["data"]["z_class"]["stat"]
        if log:
            tik = np.log(tik)

        return tik.values

    log_prob_norm = logsumexp(res["mixture"]["lnProbaGivenClass"], axis=1)

    log_tik = res["mixture"]["lnProbaGivenClass"] - log_prob_norm[:, np.newaxis]

    if not log:
        return np.exp(log_tik)

    return log_tik


def get_partition(res, empiric=False):
    """ Get component labels.

    Parameters
    ----------
    res : dict
        output of multi_run_pmc_pool
    empiric : bool, optional
        If True, the component labels are the ones estimated during the Gibbas algorithm. Otherwise, they are estimated using
        posterior probabilities.

    Returns
    -------
    array
        Component labels.
    """
    if empiric:
        return res["variable"]["data"]["z_class"]["completed"]
    else:
        tik = get_tik(res, log=True, empiric=False)
        partition = tik.argmax(axis=1)
        if "dictionary" in res["algo"]:
            if "z_class" in res["algo"]["dictionary"]:
                label = np.array([x.replace("k: ", "") for x in tik.columns])
                partition = label[partition]

        return partition


def get_param(res, var_name):
    """ Get the estimated parameters

    Parameters
    ----------
    res : dict
        output of multi_run_pmc_pool
    var_name : str
        Variable name to get parameters

    Returns
    -------
    DataFrame, dict
        The parameters are mainly returned as a DataFrame with each row corresponding to a component

    Raises
    ------
    ValueError
        Wrong variable names
    ValueError
        Not yet implemented for model ...
    """
    if var_name not in res["variable"]["type"]:
        raise ValueError("Wrong variable names.")

    model = res["variable"]["type"][var_name]

    n_class = res["algo"]["nClass"]

    if model == "Gaussian":
        return _get_param_numerical(res["variable"]["param"][var_name], n_class, ["mean", "sd"])
    elif model == "Multinomial":
        return _get_param_multinomial(res["variable"]["param"][var_name], n_class)
    elif model == "Poisson":
        return _get_param_numerical(res["variable"]["param"][var_name], n_class, ["lambda"])
    elif model == "Weibull":
        return _get_param_numerical(res["variable"]["param"][var_name], n_class, ["k (shape)", "lambda (scale)"])
    elif model == "NegativeBinomial":
        return _get_param_numerical(res["variable"]["param"][var_name], n_class, ["n", "p"])
    elif model == "Rank_ISR":
        return _get_param_Rank_ISR(res["variable"]["param"][var_name], n_class)
    elif (model == "Func_CS") | (model == "Func_SharedAlpha_CS"):
        return _get_param_Func_CS(res["variable"]["param"][var_name], n_class)
    elif model == "LatentClass":
        return _get_param_numerical(res["variable"]["param"][var_name], n_class, ["pi"])
    else:
        raise ValueError("Not yet implemented for model " + model)


def _get_param_numerical(param, n_class, col_names):
    """ Intern function to get numerical parameters """
    try:
        index = [re.search("k: (.+?),", x).group(1) for x in param["stat"].index[::len(col_names)]]
    except AttributeError:
        index = [re.search("k: (.+?)", x).group(1) for x in param["stat"].index[::len(col_names)]]

    out = param["stat"]["median"].values.reshape(n_class, len(col_names))
    out = pd.DataFrame(out, columns=col_names, index=index)

    return out


def _get_param_multinomial(param, n_class):
    """ Intern function to get multinomial parameters """
    n_modalities = int(param["stat"].shape[0] / n_class)

    modalities = [re.sub("k: .*, modality: ", "", x) for x in param["stat"].index[:n_modalities]]
    return _get_param_numerical(param, n_class, modalities)


def _get_param_Func_CS(param, n_class):
    n_sub = int(re.search("nSub:(.+?),", param["paramStr"]).group(1))
    n_coeff = int(re.search("nCoeff:(.+?)$", param["paramStr"]).group(1))

    return {"alpha": _get_param_numerical(param["alpha"], n_class,
                                          ["s: " + str(i) + str(j) for i in range(n_sub) for j in [", alpha0", ", alpha1"]]),
            "beta": _get_param_numerical(
                param["beta"], n_class,
                ["s: " + str(i) + str(j) for i in range(n_sub) for j in [", c: " + str(i) for i in range(n_coeff)]]),
            "sd": _get_param_numerical(param["sd"], n_class, ["s: " + str(i) for i in range(n_sub)])}


def _get_param_Rank_ISR(param, n_class):
    n_modality = int(param["paramStr"].replace("nModality:", ""))
    mu = np.empty((0, n_modality), dtype=int)
    for value in param["mu"]["stat"].values():
        mu = np.vstack((mu, value["rank"]))

    return {"pi": _get_param_numerical(param["pi"], n_class, ["pi"]), "mu": mu}
