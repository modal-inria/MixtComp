"""
Functions to convert c++ outputs to python
"""

import pandas as pd


def convert_C_matrix(x):
    """ Convert a matrix to array or DataFrame

    The matrix format used  in c++ is:
    `{"data": np.array([[1.73245393e-06], [1.49653391e-07]]), "rowNames": [],
     "colNames": [], "ctype": "Matrix", "dtype": "Real", "ncol": 1, "nrow": 2}`

    Parameters
    ----------
    x : dict
        matrix object from c++

    Returns
    -------
    array or DataFrame
        matrix in python format
    """
    if x["colNames"] == []:
        return x["data"]
    else:
        if x["rowNames"] == []:
            return pd.DataFrame(x["data"], columns=x["colNames"])
        else:
            return pd.DataFrame(x["data"], columns=x["colNames"], index=x["rowNames"])


def convert_C_vector(x):
    """ Convert a vector to array or Series

    The matrix format used  in c++ is:
    `{"data": np.array([-127.17314741, -127.17314741, -127.17314741]),
      "rowNames": [], "nrow": 3, "dtype": "Real", "ctype": "Vector"}`

    Parameters
    ----------
    x : dict
        vector object from c++

    Returns
    -------
    array or Series
        vector in python format
    """
    if x["rowNames"] == []:
        return x["data"]
    else:
        return pd.Series(x["data"], index=x["rowNames"])


def convert(object):
    """ Convert c++ output

    C++ returns a dict. It converts all vectors and matrices in python format

    Parameters
    ----------
    object : dict
        c++ output. Modified in place.
    """
    for key in object.keys():
        if isinstance(object[key], dict):
            if "ctype" in object[key].keys():
                if object[key]["ctype"] == "Matrix":
                    object[key] = convert_C_matrix(object[key])
                elif object[key]["ctype"] == "Vector":
                    object[key] = convert_C_vector(object[key])
            else:
                convert(object[key])
