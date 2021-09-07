from collections import Counter
import re

import matplotlib.pyplot as plt
import matplotlib.patches as patches
import numpy as np
import pandas as pd
import seaborn as sns


def plot_data(res, var_name, class_ids=None, all=False):
    """ Plot Data Distribution

    Parameters
    ----------
    res : dict
        output of multi_run_pmc_pool
    var_name : str
        Name of the variable to plot
    class_ids : list of int, optional
        Numbers or names of classes to plot, by default None
    all : bool, optional
        If True add the overall distribution, by default False

    Returns
    -------
    Axes
        Boxplot of the data distribution
    """
    if var_name not in res["variable"]["type"].keys():
        raise ValueError(var_name + " variable does not exist in the mixture model.")

    if isinstance(class_ids, int) | isinstance(class_ids, str):
        class_ids = [class_ids]

    model = res["variable"]["type"][var_name]

    if model == "Gaussian":
        return boxplot_per_class_numerical(extract_bounds_boxplot_numerical(res, var_name, class_ids, all),
                                           var_name)
    elif model == "Multinomial":
        return barplot_per_class_categorical(extract_bounds_barplot_categorical(res, var_name, class_ids, all),
                                             var_name)
    elif model == "Poisson":
        return boxplot_per_class_numerical(extract_bounds_boxplot_numerical(res, var_name, class_ids, all),
                                           var_name)
    elif model == "NegativeBinomial":
        return boxplot_per_class_numerical(extract_bounds_boxplot_numerical(res, var_name, class_ids, all),
                                           var_name)
    elif model == "Weibull":
        return boxplot_per_class_numerical(extract_bounds_boxplot_numerical(res, var_name, class_ids, all),
                                           var_name)
    else:
        raise NotImplementedError("Not yet implemented for model " + model)


def boxplot_per_class_numerical(bounds, var_name):
    """ plot_data part for a numerical variable """

    color = sns.color_palette()
    _, ax = plt.subplots()
    i = 0
    for _, row in bounds.iterrows():
        ax.add_patch(patches.Rectangle((row.iloc[1], i), width=row.iloc[3]-row.iloc[1],
                                       height=0.8, fill=True, edgecolor="black", lw=1.5, facecolor=color[i % len(color)]))
        plt.plot(row.iloc[:2].values, [i + 0.4] * 2, c="black")
        plt.plot(row.iloc[-2:].values, [i + 0.4] * 2, c="black")
        plt.plot([row.iloc[2]] * 2, [i, i + 0.8], c="black")
        i += 1

    ax.set_xlabel(var_name)
    ax.set_ylabel("Class")
    plt.yticks([j + 0.4 for j in range(i)], bounds.index.to_list())


def barplot_per_class_categorical(bounds, var_name):
    """ plot_data part for a categorical variable """

    bounds2 = pd.DataFrame()
    bounds2["Probability"] = bounds.values.flatten()
    bounds2["Class"] = np.repeat(bounds.index.to_list(), bounds.shape[1])
    bounds2["Category"] = bounds.columns.to_list() * bounds.shape[0]

    ax = sns.barplot(data=bounds2, x="Category", y="Probability", hue="Class")
    ax.set_xlabel(var_name)

    return ax


def extract_bounds_boxplot_numerical(res, var_name, class_ids=None, all=False):
    """ Compute quantiles for boxplot for boxplot_per_class_numerical

    Returns
    -------
    DataFrame
        Quantiles at [.05, .25, .5, .75, .95] for the different classes. 1 row = 1 class
    """
    obs = res["variable"]["data"][var_name]["completed"]
    tik = res["variable"]["data"]["z_class"]["stat"]

    if class_ids is None:
        class_ids = [re.sub("k: ", "", x) for x in tik.columns]

    ordered_indices = obs.argsort()

    cum_sums = tik.iloc[ordered_indices].cumsum()
    cum_sums /= cum_sums.iloc[-1]

    q = [.05, .25, .5, .75, .95]
    thresholds = np.zeros((res["algo"]["nClass"], len(q)))
    for i in range(len(q)):
        thresholds[:, i] = obs[ordered_indices[np.argmin(abs(cum_sums - q[i]).values, axis=0)]]

    thresholds = pd.DataFrame(thresholds, columns=["q" + str(i) for i in q],
                              index=[re.sub("k:", "Class", x) for x in tik.columns])
    thresholds = thresholds.loc[["Class " + str(id) for id in class_ids]]

    if all:
        thresholds = thresholds.append(pd.DataFrame(np.quantile(obs, q).reshape(1, -1),
                                                    columns=thresholds.columns, index=["all"]))

    return thresholds


def extract_bounds_barplot_categorical(res, var_name, class_ids=None, all=False):
    """ Compute the proportion of every label in every class

    Returns
    -------
    DataFrame
        Proportion of each level in each class. 1 row = 1 class
    """
    obs = res["variable"]["data"][var_name]["completed"]
    tik = res["variable"]["data"]["z_class"]["stat"]

    if class_ids is None:
        class_ids = [re.sub("k: ", "", x) for x in tik.columns]

    levels = np.unique(obs)

    probas = np.zeros((res["algo"]["nClass"], len(levels)))
    for i in range(len(levels)):
        probas[:, i] = (tik * (obs == levels[i])[:, np.newaxis]).sum() / tik.sum()

    probas = pd.DataFrame(probas, columns=levels,
                          index=[re.sub("k:", "Class", x) for x in tik.columns])
    probas = probas.loc[["Class " + str(id) for id in class_ids]]

    if all:
        freq = Counter(obs)
        ind = np.zeros((1, len(levels)))
        for i in range(len(levels)):
            ind[0, i] = freq[levels[i]]
        ind /= ind.sum()
        probas = probas.append(pd.DataFrame(ind, columns=probas.columns, index=["all"]))

    return probas
