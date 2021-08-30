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


def create_model(model_names=None, var_names=None):
    """ Create model dict from model and variables names

    At least 1 of the parameters must not be None

    Parameters
    ----------
    model_names : list of str, optional
        model names, by default None
    var_names : list of str, optional
        variables names, by default None

    Returns
    -------
    dict
        A dict with where every key has the form `{"type": "model", "paramStr": "param"}`
    """
    if model_names is None:
        model_names = ["Gaussian"] * len(var_names)

    if var_names is None:
        var_names = ["var" + str(i) for i in range(len(model_names))]

    model = {}
    for i in range(len(model_names)):
        model[var_names[i]] = {"type": model_names[i], "paramStr": ""}

    return model
