import numpy as np
import pandas as pd


def create_algo(object, X, mode):
    """ Create the algo dict required by MixtComp

    Parameters
    ----------
    object : MixtComp
        object of class MixtComp
    X : dict
        data
    mode : str
        "learn" or "predict"

    Returns
    -------
    dict
        dict with algorithm parameters
    """
    algo = {
        "nbBurnInIter": object.n_burn_in_iter,
        "nbIter": object.n_iter,
        "nbGibbsBurnInIter": object.n_gibbs_burn_in_iter,
        "nbGibbsIter": object.n_gibbs_iter,
        "nInitPerClass": object.n_init_per_class,
        "nSemTry": object.n_sem_try,
        "confidenceLevel": object.confidence_level,
        "ratioStableCriterion": object.ratio_stable_criterion,
        "nStableCriterion": object.n_stable_criterion
    }
    algo["mode"] = mode
    algo["nInd"] = len(X[list(X.keys())[0]])
    algo["nClass"] = object.n_components

    return algo


def format_model(model):
    """ Format the model dict to the MixtComp format

    A dict with where every key has the form `{"type": "model", "paramStr": "param"}`

    Parameters
    ----------
    model : dict
        model dict in the right format. model is modified in place
    """
    if isinstance(model, dict):
        for key in model.keys():
            if isinstance(model[key], dict):
                if "paramStr" not in model[key].keys():
                    model[key]["paramStr"] = ""
            else:
                model[key] = {"type": model[key], "paramStr": ""}


def impute_model(data):
    """ Create model dict from data


    Parameters
    ----------
    data : array, DataFrame or dict
        data for MixtComp

    Returns
    -------
    dict
        A dict with where every key has the form `{"type": "model", "paramStr": "param"}`
    """
    if isinstance(data, np.ndarray):
        var_names = ["var" + str(i) for i in range(data.shape[1])]
        var_types = [data.dtype] * data.shape[1]

    if isinstance(data, pd.DataFrame):
        var_names = data.columns
        var_types = data.dtypes

    if isinstance(data, dict):
        var_names = data.keys()
        var_types = [type(v[0]) for v in data.values()]

    model = {}
    for var_name, var_type in zip(var_names, var_types):
        model[var_name] = _impute_model(var_name, var_type)

    return model


def _impute_model(var_name, var_type):
    """ Return the model given the variable type and name

    Parameters
    ----------
    var_name : str
        variable name
    var_type : dtype, class
        variable type

    Returns
    -------
    str
        model name

    Raises
    ------
    TypeError
        Cannot impute the model
    """
    if pd.api.types.is_float_dtype(var_type):
        return "Gaussian"
    elif pd.api.types.is_int64_dtype(var_type):
        if var_name == "z_class":
            return "LatentClass"
        else:
            return "Poisson"
    elif pd.api.types.is_string_dtype(var_type) or pd.api.types.is_categorical_dtype(var_type):
        if var_name == "z_class":
            return "LatentClass"
        else:
            return "Multinomial"
    else:
        raise TypeError("Cannot impute the model for variable ", var_name, ". Please provide the model parameter.")


def format_data_basic_mode(data, model, dictionary=None):
    create_dict = dictionary is None

    if not isinstance(data, pd.DataFrame):
        data = pd.DataFrame(data, columns=model.keys())

    if create_dict:
        dictionary = {}

    for var_name in model.keys():
        if model[var_name]["type"] in ["Multinomial", "LatentClass"]:
            if not pd.api.types.is_int64_dtype(data[var_name].dtype):
                data[var_name] = data[var_name].astype("category")

                if create_dict:
                    old_categ = data[var_name].unique()
                    old_categ = old_categ[old_categ.notna()]

                    dictionary[var_name] = {"old": old_categ.categories.to_list(),
                                            "new": [str(i) for i in range(len(old_categ))]}
                elif var_name not in dictionary.keys():
                    raise ValueError("No dictionary given for variable " + var_name)

                data[var_name] = data[var_name].cat.rename_categories(dict(zip(dictionary[var_name]["old"],
                                                                               dictionary[var_name]["new"])))

                data[var_name] = data[var_name].cat.add_categories("?")
                data[var_name] = data[var_name].fillna("?")
            else:
                data.loc[data[var_name].isna(), var_name] = "?"
                data[var_name] = data[var_name].astype("str")
        else:
            data.loc[data[var_name].isna(), var_name] = "?"
            data[var_name] = data[var_name].astype("str")

    return data.to_dict("list"), dictionary
