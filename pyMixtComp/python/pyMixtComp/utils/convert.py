import numpy as np
import pandas as pd


def create_functional(times, values):
    """ Create a functional in MixtComp format

    Parameters
    ----------
    times : array, list or Series
        array containing the time values (in float) of the functional
    values : array, list or Series
        array containing the value of the functional

    Returns
    -------
    str
        The functional data formatted to the MixtComp standard

    Raises
    ------
    ValueError
        times and values do not have the same length.

    Examples
    --------
    >>> from pyMixtComp.utils import create_functional
    >>> times = [1, 2, 3]
    >>> values = [4, 5, 6]
    >>> func = create_functional(times, values)
    >>> func
    '1:4,2:5,3:6'
    """
    if len(times) != len(values):
        raise ValueError("times and values do not have the same length.")

    func = ""
    for i in range(len(times)):
        if (times[i] is not None) and (not np.isnan(times[i])) and (values[i] is not None) and (not np.isnan(values[i])):
            func += str(times[i]) + ":" + str(values[i]) + ","

    # remove the trailing comma
    if len(func) > 0:
        func = func[:-1]

    return func


def convert_functional(functional):
    """ Convert a MixtComp functional (string) into a tuple of 2 arrays

    Parameters
    ----------
    functional : str
        a string containing a functional

    Returns
    -------
    (array, array)
        times array and values array

    Raises
    ------
    TypeError
        functional must be a string.

    Examples
    --------
    >>> from pyMixtComp.utils import convert_functional
    >>> times, values = convert_functional('1:4,2:5,3:6')
    >>> times
    array([1., 2., 3.])
    >>> values
    array([4., 5., 6.])
    """
    if not isinstance(functional, str):
        raise TypeError("functional must be a string.")
    if (functional == ""):
        return np.array([]), np.array([])

    out = np.array([x.split(":") for x in functional.split(",")]).astype(float)

    return out[:, 0], out[:, 1]


def refactor_categorical(data, old_categores, new_categores):
    """ Rename a categorical variable

    Parameters
    ----------
    data : array, list or Series
        array with categorical variables to rename
    old_categores : list or array
        array containing categories to change
    new_categores : list or array
        array containing new categorical values

    Returns
    -------
    array
        array with converted data

    Examples
    --------
    >>> from pyMixtComp.utils import refactor_categorical
    >>> data = ["single", "married", "married", "divorced", "single"]
    >>> new_data = refactor_categorical(dat, ["single", "married", "divorced"], [1, 2, 3])
    >>> new_data
    array([1, 2, 2, 3, 1])
    """
    is_not_series = not isinstance(data, pd.Series)

    if is_not_series:
        data = pd.Series(data)
    data = data.astype("category")
    data = data.cat.rename_categories(dict(zip(old_categores, new_categores)))

    if is_not_series:
        data = data.to_numpy()

    return data


def create_rank(rank_array):
    """ Convert a rank from array to string

    Parameters
    ----------
    rank_array : array
        Rank in array format (e.g. [3, 2, 0, 1])

    Returns
    -------
    str
        Rank in string format (e.g. "3, 2, 0, 1")
    """
    return ",".join(rank_array.astype(str))


def convert_rank(rank_str):
    """ Convert a rank from string to array

    Parameters
    ----------
    rank_str : str
        Rank in string format (e.g. "3, 2, 0, 1")

    Returns
    -------
    array
        Rank in array format (e.g. [3, 2, 0, 1])
    """
    return np.array(rank_str.split(",")).astype(int)
