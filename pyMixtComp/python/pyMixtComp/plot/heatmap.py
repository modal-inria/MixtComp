import re

import numpy as np
import seaborn as sns

from ..utils.criterion import compute_class_similarity, compute_variable_similarity


def plot_class_similarity(res):
    """ Plot the similarity between classes

    Parameters
    ----------
    res : dict
        output of multi_run_pmc_pool

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
    similarity = compute_class_similarity(res)
    ax = sns.heatmap(data=similarity, vmin=0, vmax=1, annot=True, cmap="coolwarm")
    ax.set_title("Similarities between classes")

    return ax


def plot_variable_similarity(res):
    """ Plot the similarity between variables

    Parameters
    ----------
    res : dict
        output of multi_run_pmc_pool

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
    similarity = compute_variable_similarity(res)
    ax = sns.heatmap(data=similarity, vmin=0, vmax=1, annot=True, cmap="coolwarm")
    ax.set_title("Similarities between variables")

    return ax


def plot_tik(res):
    """ Heatmap of the tik = P(Z_i=k|x_i)

    Observation are sorted according to the hard partition then for each component
    they are sorted by decreasing order of their tik

    Parameters
    ----------
    res : dict
        output of multi_run_pmc_pool

    Returns
    -------
    Axes
        Heatmap of the tik
    """
    tik = res["variable"]["data"]["z_class"]["stat"]
    predicted_class = res["variable"]["data"]["z_class"]["completed"].astype(str)

    order_tik = np.zeros((0, ))
    for k in range(tik.shape[1]):
        class_id = re.sub("k: ", "", tik.columns[k])
        order_tik = np.concatenate(
            (order_tik,
             np.argsort(tik.values[:, k] * (predicted_class == class_id))[::-1][:(predicted_class == class_id).sum()]))

    ax = sns.heatmap(data=tik.iloc[order_tik, ], vmin=0, vmax=1, annot=False, cmap="coolwarm")
    ax.set_title("Probabilities of classification")

    return ax
