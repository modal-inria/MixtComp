from collections import Counter

import matplotlib.pyplot as plt
import matplotlib.patches as patches
import numpy as np
import pandas as pd
from scipy.stats import norm, poisson, nbinom, weibull_min
import seaborn as sns

from pyMixtComp.plot.univariate_boxplot import barplot_per_class_categorical


def plot_data_CI(res, var_name, class_numbers=None, all=False):
    """ Plot Mean and 95%-level confidence intervals per class

    Parameters
    ----------
    res : dict
        output of multi_run_pmc_pool
    var_name : str
        Variable name
    class_numbers : int, array-like of int, optional
        Classes to plot, by default None, all classes are plotted
    all : bool, optional
        If True, it adds the mean and 95%-level confidence interval for all data, by default False

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
    if var_name not in res["variable"]["type"].keys():
        raise ValueError(var_name + " variable does not exist in the mixture model.")

    if isinstance(class_numbers, int):
        class_numbers = [class_numbers]

    model = res["variable"]["type"][var_name]

    if model == "Gaussian":
        return plot_CI_numerical(extract_CI_gaussian(res, var_name, class_numbers, all),
                                 var_name)
    elif model == "Multinomial":
        return barplot_per_class_categorical(extract_CI_multinomial(res, var_name, class_numbers, all),
                                             var_name)
    elif model == "Poisson":
        return plot_CI_numerical(extract_CI_poisson(res, var_name, class_numbers, all),
                                 var_name)
    elif model == "NegativeBinomial":
        return plot_CI_numerical(extract_CI_nbinom(res, var_name, class_numbers, all),
                                 var_name)
    elif model == "Weibull":
        return plot_CI_numerical(extract_CI_weibull(res, var_name, class_numbers, all),
                                 var_name)
    else:
        raise NotImplementedError("Not yet implemented for model " + model)


def plot_CI_numerical(ci, var_name):
    color = sns.color_palette()
    _, ax = plt.subplots()
    i = 0
    for _, row in ci.iterrows():
        ax.add_patch(patches.Rectangle((row.iloc[1], i), width=row.iloc[2]-row.iloc[1], alpha=0.5,
                                       height=0.8, fill=True, edgecolor="black", lw=1.5, facecolor=color[i % len(color)]))
        plt.scatter([row.iloc[0]], [i + 0.4], c="black", s=10, axes=ax)
        i += 1

    ax.set_xlabel(var_name)
    ax.set_ylabel("Class")
    plt.yticks([j + 0.4 for j in range(i)], ci.index.to_list())

    return ax


def extract_CI_gaussian(res, var_name, class_numbers=None, all=False):
    """ Compute mean and 95% CI of the estimated gaussian distribution """
    if class_numbers is None:
        class_numbers = list(range(res["algo"]["nClass"]))

    theta = res["variable"]["param"][var_name]["stat"]["median"].values.reshape(-1, 2)

    ci = pd.DataFrame({"mean": theta[:, 0],
                       "lower": norm(loc=theta[:, 0], scale=theta[:, 1]).ppf(0.025),
                       "upper": norm(loc=theta[:, 0], scale=theta[:, 1]).ppf(0.975)},
                      index=["Class " + str(k) for k in range(len(theta))])

    ci = ci.iloc[class_numbers]

    if all:
        ci = ci.append(pd.DataFrame(
            [[np.mean(res["variable"]["data"][var_name]["completed"]),
             np.quantile(res["variable"]["data"][var_name]["completed"], 0.025),
             np.quantile(res["variable"]["data"][var_name]["completed"], 0.975)]],
            columns=["mean", "lower", "upper"], index=["all"]))

    return ci


def extract_CI_poisson(res, var_name, class_numbers=None, all=False):
    """ Compute mean and 95% CI of the estimated poisson distribution """
    if class_numbers is None:
        class_numbers = list(range(res["algo"]["nClass"]))
    theta = res["variable"]["param"][var_name]["stat"]["median"].values.reshape(-1, 1)

    ci = pd.DataFrame({"mean": theta[:, 0],
                       "lower": poisson(theta[:, 0]).ppf(0.025),
                       "upper": poisson(theta[:, 0]).ppf(0.975)},
                      index=["Class " + str(k) for k in range(len(theta))])

    ci = ci.iloc[class_numbers]
    if all:
        ci = ci.append(pd.DataFrame(
            [[np.mean(res["variable"]["data"][var_name]["completed"]),
             np.quantile(res["variable"]["data"][var_name]["completed"], 0.025),
             np.quantile(res["variable"]["data"][var_name]["completed"], 0.975)]],
            columns=["mean", "lower", "upper"], index=["all"]))

    return ci


def extract_CI_nbinom(res, var_name, class_numbers=None, all=False):
    """ Compute mean and 95% CI of the estimated negative binomial distribution """
    if class_numbers is None:
        class_numbers = list(range(res["algo"]["nClass"]))
    theta = res["variable"]["param"][var_name]["stat"]["median"].values.reshape(-1, 2)

    ci = pd.DataFrame({"mean": nbinom.mean(theta[:, 0], theta[:, 1]),
                       "lower": nbinom(theta[:, 0], theta[:, 1]).ppf(0.025),
                       "upper": nbinom(theta[:, 0], theta[:, 1]).ppf(0.975)},
                      index=["Class " + str(k) for k in range(len(theta))])

    ci = ci.iloc[class_numbers]
    if all:
        ci = ci.append(pd.DataFrame(
            [[np.mean(res["variable"]["data"][var_name]["completed"]),
              np.quantile(res["variable"]["data"][var_name]["completed"], 0.025),
              np.quantile(res["variable"]["data"][var_name]["completed"], 0.975)]],
            columns=["mean", "lower", "upper"], index=["all"]))

    return ci


def extract_CI_weibull(res, var_name, class_numbers=None, all=False):
    """ Compute mean and 95% CI of the estimated weibull distribution """
    if class_numbers is None:
        class_numbers = list(range(res["algo"]["nClass"]))
    theta = res["variable"]["param"][var_name]["stat"]["median"].values.reshape(-1, 2)
    ci = pd.DataFrame({"mean": weibull_min(c=theta[:, 0], scale=theta[:, 1]).mean(),
                       "lower": weibull_min(c=theta[:, 0], scale=theta[:, 1]).ppf(0.025),
                       "upper": weibull_min(c=theta[:, 0], scale=theta[:, 1]).ppf(0.975)},
                      index=["Class " + str(k) for k in range(len(theta))])

    ci = ci.iloc[class_numbers]

    if all:
        ci = ci.append(pd.DataFrame(
            [[np.mean(res["variable"]["data"][var_name]["completed"]),
              np.quantile(res["variable"]["data"][var_name]["completed"], 0.025),
              np.quantile(res["variable"]["data"][var_name]["completed"], 0.975)]],
            columns=["mean", "lower", "upper"], index=["all"]))


def extract_CI_multinomial(res, var_name, class_numbers=None, all=False):
    """ Keep labels up to 95% probability """
    if class_numbers is None:
        class_numbers = list(range(res["algo"]["nClass"]))
    theta = res["variable"]["param"][var_name]["stat"]["median"].values.reshape(res["algo"]["nClass"], -1)

    theta = pd.DataFrame(theta, columns=range(theta.shape[1]), index=["Class " + str(k) for k in range(len(theta))])
    theta = theta.iloc[class_numbers]

    if all:
        freq = Counter(res["variable"]["data"][var_name]["completed"])
        ind = np.zeros((1, theta.shape[1]))
        for i in range(theta.shape[1]):
            ind[0, i] = freq[i]
        ind /= ind.sum()
        theta = theta.append(pd.DataFrame(ind, columns=theta.columns, index=["all"]))

    for k in range(len(theta)):
        ordered_indices = theta.iloc[k].values.argsort()[::-1]
        remove = ordered_indices[(np.where(theta.iloc[k][ordered_indices].cumsum() > 0.95)[0][0] + 1):]
        theta.iloc[k, remove] = 0

    return theta