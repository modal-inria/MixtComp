import numpy as np
import pandas as pd


def compute_discriminative_power_variable(res, class_id=None):
    """ Compute the discriminative power of variables

    Parameters
    ----------
    res : dict
        output of multi_run_pmc_pool
    class_id : int, optional
        Class number (0, ...,  n_components - 1) of class name, by default None.
        If None, it returns the discriminative power of variables globally otherwise it returns the discriminative power
        of variables in the given class

    Returns
    -------
    Series
        Discriminative power of variables

    Notes
    -----
    The discriminative power of variable j is defined by:

    .. math::
        1 - \\sum_{k=1}^K \\sum_{i=1}^n P(Z_i=k|x_{ij}) \\log(P(Z_i=k|x_{ij})) / (n\\log(K))

    The discriminative power of variable j in class k is defined by:

    .. math::
        1 - \\frac{1}{n\\log(2)}(\\sum_{i=1}^n P(Z_i!=k|x_{ij}) \\log(P(Z_i!=k|x_{ij})) +
        \\sum_{i=1}^n P(Z_i=k|x_{ij}) \\log(P(Z_i=k|x_{ij})))

    A high value (close to one) means that the variable is highly discriminating.
    A low value (close to zero) means that the variable is poorly discriminating.
    """
    if class_id is None:
        return 1 - res["mixture"]["IDClass"].sum(axis=0)
    else:
        k = len(res["mixture"]["IDClass"])

        entropy_kj = res["mixture"]["IDClass"].loc["k: " + str(class_id)] * np.log(k)
        entropy_kj_bar = res["mixture"]["IDClassBar"].loc["k: " + str(class_id)] * np.log(k)

        return 1 - (entropy_kj + entropy_kj_bar) / np.log(2)


def compute_discriminative_power_class(res):
    """ Compute the discriminative power of classes

    Parameters
    ----------
    res : dict
        output of multi_run_pmc_pool

    Returns
    -------
    Series
        discriminative power of classes

    Notes
    -----
    The discriminative power of classes is defined by:

    .. math::
        1 + \\sum_{i=1}^n P(Z_i=k|x_i) \\log(P(Z_i=k|x_i)) / (n \\exp(-1))}

    A high value (close to one) means that the class is highly discriminating.
    A low value (close to zero) means that the class is poorly discriminating.
    """
    tik = res["variable"]["data"]["z_class"]["stat"]
    return 1 + np.log(tik ** tik).mean(axis=0)/np.exp(-1)


def compute_variable_similarity(res):
    """ Compute the similarity between variables

    Parameters
    ----------
    res : dict
        output of multi_run_pmc_pool

    Returns
    -------
    DataFrame
        Similarity between variables

    Notes
    -----
    The similarities between variables j and h is defined by Delta(j, h) where Delta is:

    .. math::
        \\Delta(j,h)^2 = 1 - \\sqrt{(1/n) * \\sum_{i=1}^n \\sum_{k=1}^K (P(Z_i=k|x_{ij}) - P(Z_i=k|x_{ih}))^2}

    A high value (close to one) means that the variables provide the same information for the clustering task
    (i.e. similar partitions).
    A low value (close to zero) means that the variables provide some different information for the clustering task
    (i.e. different partitions).
    """
    return res["mixture"]["delta"]


def compute_class_similarity(res):
    """ Compute the similarity between classes

    Parameters
    ----------
    res : dict
        output of multi_run_pmc_pool

    Returns
    -------
    DataFrame
        Similarity between classes

    Notes
    -----
    The similarities between classes k and g is defined by :math:`1 - \\Sigma(k,g)` where :math:`\\Sigma` is:

    .. math::
        \\Sigma(k,g)^2 = \\frac{1}{n} * \\sum_{i=1}^n (P(Z_i=k|x_i) - P(Z_i=g|x_i))^2

    A high value (close to one) means that the classes are highly similar (high overlapping).
    A low value (close to zero) means that the classes are highly different (low overlapping).
    """
    tik = res["variable"]["data"]["z_class"]["stat"]

    simil = np.zeros((res["algo"]["nClass"], res["algo"]["nClass"]))
    for k in range(len(simil)):
        simil[k, :] = 1 - np.sqrt(((tik.values - tik.values[:, [k]]) ** 2).mean(axis=0))

    simil = pd.DataFrame(simil, columns=[x.replace("k: ", "Class ") for x in tik.columns],
                         index=[x.replace("k: ", "Class ") for x in tik.columns])

    return simil
