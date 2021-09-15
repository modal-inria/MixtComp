import seaborn as sns

from pyMixtComp.utils.criterion import compute_discriminative_power_class, compute_discriminative_power_variable


def plot_discriminative_power_variable(res, class_id=None):
    """ Plot the discriminative power of variables

    Parameters
    ----------
    res : dict
        output of multi_run_pmc_pool
    class_id : int, optional
        Class number (0, ...,  n_components - 1) or class name, by default None.
        If None, it returns the discriminative power of variables globally otherwise it returns the discriminative power
        of variables in the given class

    Returns
    -------
    Axes
        Barplot
    """
    discrim_power = compute_discriminative_power_variable(res, class_id)
    discrim_power = discrim_power.sort_values(ascending=False)

    title = "Discriminative level of variables"
    if class_id is not None:
        title += " in class " + str(class_id)

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
