from collections import Counter
import re

import matplotlib.pyplot as plt
import matplotlib.patches as patches
import numpy as np
import pandas as pd
from scipy.stats import norm, poisson, nbinom, weibull_min
import seaborn as sns

from .univariate_boxplot import barplot_per_class_categorical
from .functional import extract_CI_Func_CS, plot_functional_data


def plot_data_CI(res, var_name, class_ids=None, all=False, ax=None, **kwargs):
    """ Plot Mean and 95%-level confidence intervals per class

    Parameters
    ----------
    res : dict
        output of multi_run_pmc_pool
    var_name : str
        Variable name
    class_ids : int, array-like of int, optional
        Classes to plot, by default None, all classes are plotted
    all : bool, optional
        If True, it adds the mean and 95%-level confidence interval for all data, by default False
    ax: Axes
        Matplotlib axis object, by default None
    **kwargs
        For functional data: add_obs (default=False) adds observations on the plot,
        add_CI (default=True) adds confidence intervals for means.

    Returns
    -------
    Axes
        Plot Mean and 95%-level confidence intervals per class

    Raises
    ------
    ValueError
        The given var_name does not exist
    NotImplementedError
        The plot function associated with the model of var_name does not exist
    """
    if not isinstance(all, bool):
        raise TypeError("all mus be a boolean.")
    if not isinstance(var_name, str):
        raise TypeError("var_name mus be a string.")

    if var_name not in res["variable"]["type"].keys():
        raise ValueError(var_name + " variable does not exist in the mixture model.")

    if isinstance(class_ids, int) | isinstance(class_ids, str):
        class_ids = [class_ids]

    model = res["variable"]["type"][var_name]

    if model == "Gaussian":
        return plot_CI_numerical(extract_CI_gaussian(res, var_name, class_ids, all),
                                 var_name, ax)
    elif model == "Multinomial":
        return barplot_per_class_categorical(extract_CI_multinomial(res, var_name, class_ids, all),
                                             var_name, ax)
    elif model == "Poisson":
        return plot_CI_numerical(extract_CI_poisson(res, var_name, class_ids, all),
                                 var_name, ax)
    elif model == "NegativeBinomial":
        return plot_CI_numerical(extract_CI_nbinom(res, var_name, class_ids, all),
                                 var_name, ax)
    elif model == "Weibull":
        return plot_CI_numerical(extract_CI_weibull(res, var_name, class_ids, all),
                                 var_name, ax)
    elif (model == "Func_CS") | (model == "Func_SharedAlpha_CS"):
        return plot_functional_data(extract_CI_Func_CS(res, var_name, class_ids), res, var_name, ax=ax, **kwargs)
    else:
        raise NotImplementedError("Not yet implemented for model " + model)


def plot_CI_numerical(ci, var_name, ax=None):
    color = sns.color_palette()
    ax = ax or plt.figure().add_subplot(1, 1, 1)
    i = 0
    for _, row in ci.iterrows():
        ax.add_patch(patches.Rectangle((row.iloc[1], i), width=row.iloc[2]-row.iloc[1], alpha=0.5,
                                       height=0.8, fill=True, edgecolor="black", lw=1.5, facecolor=color[i % len(color)]))
        plt.scatter([row.iloc[0]], [i + 0.4], c="black", s=10, axes=ax)
        i += 1

    ax.set_xlabel(var_name)
    ax.set_ylabel("Class")
    ax.set_yticks([j + 0.4 for j in range(i)], ci.index.to_list())

    return ax


def extract_CI_gaussian(res, var_name, class_ids=None, all=False):
    """ Compute mean and 95% CI of the estimated gaussian distribution """
    if class_ids is None:
        class_ids = [x.replace("k: ", "") for x in res["mixture"]["IDClass"].index]

    theta = res["variable"]["param"][var_name]["stat"]["median"].values.reshape(-1, 2)

    ci = pd.DataFrame({"mean": theta[:, 0],
                       "lower": norm(loc=theta[:, 0], scale=theta[:, 1]).ppf(0.025),
                       "upper": norm(loc=theta[:, 0], scale=theta[:, 1]).ppf(0.975)},
                      index=[x.replace("k:", "Class") for x in res["mixture"]["IDClass"].index])

    ci = ci.loc[["Class " + str(id) for id in class_ids]]

    if all:
        ci = ci.append(pd.DataFrame(
            [[np.mean(res["variable"]["data"][var_name]["completed"]),
             np.quantile(res["variable"]["data"][var_name]["completed"], 0.025),
             np.quantile(res["variable"]["data"][var_name]["completed"], 0.975)]],
            columns=["mean", "lower", "upper"], index=["all"]))

    return ci


def extract_CI_poisson(res, var_name, class_ids=None, all=False):
    """ Compute mean and 95% CI of the estimated poisson distribution """
    if class_ids is None:
        class_ids = [x.replace("k: ", "") for x in res["mixture"]["IDClass"].index]
    theta = res["variable"]["param"][var_name]["stat"]["median"].values.reshape(-1, 1)

    ci = pd.DataFrame({"mean": theta[:, 0],
                       "lower": poisson(theta[:, 0]).ppf(0.025),
                       "upper": poisson(theta[:, 0]).ppf(0.975)},
                      index=[x.replace("k:", "Class") for x in res["mixture"]["IDClass"].index])

    ci = ci.loc[["Class " + str(id) for id in class_ids]]
    if all:
        ci = ci.append(pd.DataFrame(
            [[np.mean(res["variable"]["data"][var_name]["completed"]),
             np.quantile(res["variable"]["data"][var_name]["completed"], 0.025),
             np.quantile(res["variable"]["data"][var_name]["completed"], 0.975)]],
            columns=["mean", "lower", "upper"], index=["all"]))

    return ci


def extract_CI_nbinom(res, var_name, class_ids=None, all=False):
    """ Compute mean and 95% CI of the estimated negative binomial distribution """
    if class_ids is None:
        class_ids = [x.replace("k: ", "") for x in res["mixture"]["IDClass"].index]

    theta = res["variable"]["param"][var_name]["stat"]["median"].values.reshape(-1, 2)

    ci = pd.DataFrame({"mean": nbinom.mean(theta[:, 0], theta[:, 1]),
                       "lower": nbinom(theta[:, 0], theta[:, 1]).ppf(0.025),
                       "upper": nbinom(theta[:, 0], theta[:, 1]).ppf(0.975)},
                      index=[x.replace("k:", "Class") for x in res["mixture"]["IDClass"].index])

    ci = ci.loc[["Class " + str(id) for id in class_ids]]
    if all:
        ci = ci.append(pd.DataFrame(
            [[np.mean(res["variable"]["data"][var_name]["completed"]),
              np.quantile(res["variable"]["data"][var_name]["completed"], 0.025),
              np.quantile(res["variable"]["data"][var_name]["completed"], 0.975)]],
            columns=["mean", "lower", "upper"], index=["all"]))

    return ci


def extract_CI_weibull(res, var_name, class_ids=None, all=False):
    """ Compute mean and 95% CI of the estimated weibull distribution """
    if class_ids is None:
        class_ids = [x.replace("k: ", "") for x in res["mixture"]["IDClass"].index]

    theta = res["variable"]["param"][var_name]["stat"]["median"].values.reshape(-1, 2)
    ci = pd.DataFrame({"mean": weibull_min(c=theta[:, 0], scale=theta[:, 1]).mean(),
                       "lower": weibull_min(c=theta[:, 0], scale=theta[:, 1]).ppf(0.025),
                       "upper": weibull_min(c=theta[:, 0], scale=theta[:, 1]).ppf(0.975)},
                      index=[x.replace("k:", "Class") for x in res["mixture"]["IDClass"].index])

    ci = ci.loc[["Class " + str(id) for id in class_ids]]

    if all:
        ci = ci.append(pd.DataFrame(
            [[np.mean(res["variable"]["data"][var_name]["completed"]),
              np.quantile(res["variable"]["data"][var_name]["completed"], 0.025),
              np.quantile(res["variable"]["data"][var_name]["completed"], 0.975)]],
            columns=["mean", "lower", "upper"], index=["all"]))


def extract_CI_multinomial(res, var_name, class_ids=None, all=False):
    """ Keep labels up to 95% probability """
    if class_ids is None:
        class_ids = [x.replace("k: ", "") for x in res["mixture"]["IDClass"].index]

    theta = res["variable"]["param"][var_name]["stat"]["median"].values.reshape(res["algo"]["nClass"], -1)

    theta = pd.DataFrame(theta, columns=[re.sub("k: .*, modality: ", "", x)
                                         for x in res["variable"]["param"][var_name]["stat"]["median"].index[:theta.shape[1]]],
                         index=[x.replace("k:", "Class") for x in res["mixture"]["IDClass"].index])
    theta = theta.loc[["Class " + str(id) for id in class_ids]]

    if all:
        freq = Counter(res["variable"]["data"][var_name]["completed"])
        ind = np.zeros((1, len(freq.keys())))
        for i, key in zip(range(len(freq.keys())), freq.keys()):
            ind[0, i] = freq[key]
        ind /= ind.sum()
        theta = theta.append(pd.DataFrame(ind, columns=[str(k) for k in freq.keys()], index=["all"]))

    for k in range(len(theta)):
        ordered_indices = theta.iloc[k].values.argsort()[::-1]
        remove = ordered_indices[(np.where(theta.iloc[k][ordered_indices].cumsum() > 0.95)[0][0] + 1):]
        theta.iloc[k, remove] = 0

    return theta
