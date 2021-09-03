import numpy as np
import seaborn as sns


def compute_discriminative_power_variable(res, class_number=None):
    """ Compute the discriminative power of variables

    Parameters
    ----------
    res : dict
        output of multi_run_pmc_pool
    class_number : int, optional
        Class number (0, ...,  n_components - 1), by default None.
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
    1 - \sum_{k=1}^K sum_{i=1}^n P(Z_i=k|x_{ij}) \log(P(Z_i=k|x_{ij})) / (n*\log(K))

    The discriminative power of variable j in class k is defined by:
    .. math::
    1 - (sum_{i=1}^n P(Z_i!=k|x_{ij}) \log(P(Z_i!=k|x_{ij})) + sum_{i=1}^n P(Z_i=k|x_{ij}) \log(P(Z_i=k|x_{ij}))) / (n*\log(2))

    A high value (close to one) means that the variable is highly discriminating.
    A low value (close to zero) means that the variable is poorly discriminating.
    """
    if class_number is None:
        return 1 - res["mixture"]["IDClass"].sum(axis=0)
    else:
        K = len(res["mixture"]["IDClass"])

        entropy_KJ = res["mixture"]["IDClass"].iloc[class_number] * np.log(K)
        entropy_KJ_bar = res["mixture"]["IDClassBar"].iloc[class_number] * np.log(K)

        return 1 - (entropy_KJ + entropy_KJ_bar) / np.log(2)


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
    1 + \sum_{i=1}^n P(Z_i=k|x_i) \log(P(Z_i=k|x_i)) / (n*\exp(-1))}

    A high value (close to one) means that the class is highly discriminating.
    A low value (close to zero) means that the class is poorly discriminating.
    """
    tik = res["variable"]["data"]["z_class"]["stat"]
    return 1 + np.log(tik ** tik).mean(axis=0)/np.exp(-1)


def plot_discriminative_power_variable(res, class_number=None):
    """ Plot the discriminative power of variables

    Parameters
    ----------
    res : dict
        output of multi_run_pmc_pool
    class_number : int, optional
        Class number (0, ...,  n_components - 1), by default None.
        If None, it returns the discriminative power of variables globally otherwise it returns the discriminative power
        of variables in the given class

    Returns
    -------
    Axes
        Barplot
    """
    discrim_power = compute_discriminative_power_variable(res, class_number)
    discrim_power = discrim_power.sort_values(ascending=False)

    title = "Discriminative level of variables"
    if class_number is not None:
        title += " in class " + str(class_number)

    ax = _barplot(discrim_power, title, "Variables", "Discriminative power")
    return ax


def plot_discriminative_power_class(res):
    """Plot the discriminative power of classes

    Parameters
    ----------
    res : dict
        output of multi_run_pmc_pool

    Returns
    -------
    Axes
        barplot
    """
    discrim_power = compute_discriminative_power_class(res)
    discrim_power = discrim_power.sort_values(ascending=False)

    title = "Discriminative level of classes"

    ax = _barplot(discrim_power, title, "Classes", "Discriminative power")
    return ax


def plot_proportion(res):
    """ Plot the class proportion

    Parameters
    ----------
    res : dict
        output of multi_run_pmc_pool

    Returns
    -------
    Axes
        barplot
    """
    ax = _barplot(res["variable"]["param"]["z_class"]["stat"]["median"], "Class proportion", "Classes", "Proportion")

    return ax


def _barplot(heights, title, xlabel, ylabel):
    """ Common functions for plotting barplot

    Parameters
    ----------
    heights : Series
        Series of bar heights
    title : str
        title
    xlabel : str
        x-axis label
    ylabel : str
        y-axis label

    Returns
    -------
    Axes
        barplot
    """
    ax = sns.barplot(x=heights.index, y=heights, color="tab:blue")
    for p in ax.patches:
        _x = p.get_x() + p.get_width() / 2
        _y = p.get_y() + p.get_height()
        value = "{:.2f}".format(p.get_height())
        ax.text(_x, _y, value, ha="center")

    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel)
    ax.set_title(title)

    return ax
