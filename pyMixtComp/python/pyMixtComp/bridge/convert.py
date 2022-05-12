"""
Functions to convert c++ outputs to python
"""
import numpy as np
import pandas as pd


def convert_c_matrix(x):
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


def convert_c_vector(x):
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
                    object[key] = convert_c_matrix(object[key])
                elif object[key]["ctype"] == "Vector":
                    object[key] = convert_c_vector(object[key])
            else:
                convert(object[key])


def convert_data_to_dict(X):
    """ Convert data to dict of list of string

    Parameters
    ----------
    X : array, DataFrame, dict
        Data. Columns corresponds to variables. In the case of a dict: 1 key = 1 variable

    Returns
    -------
    dict
        Data in the supported format
    """
    if isinstance(X, np.ndarray):
        d = {}
        for i in range(X.shape[1]):
            d["var" + str(i)] = X[:, i].astype("str")

        return d

    if isinstance(X, pd.DataFrame):
        return X.astype(str).to_dict("list")

    if isinstance(X, dict):
        d = {}
        for key in X.keys():
            if isinstance(X[key], np.ndarray):
                d[key] = X[key].astype("str")
            elif isinstance(X[key], list):
                d[key] = np.array(X[key]).astype("str")

        return d
