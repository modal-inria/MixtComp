import importlib.resources as pkg_resources

import pandas as pd


def load_iris():
    """Iris data

    The iris data set gives the measurements in centimeters of the variables sepal length and width and petal length and width,
    respectively, for 50 flowers from each of 3 species of iris. The species are Iris setosa, versicolor, and virginica.

    Returns
    -------
    (DataFrame, dict)
        Data are returned in a dataFrame with 5 columns.
        Model is returned as a dict

    References
    ----------
    Fisher, R. A. (1936) The use of multiple measurements in taxonomic problems. Annals of Eugenics, 7, Part II, 179–188.

    The data were collected by Anderson, Edgar (1935). The irises of the Gaspe Peninsula, Bulletin of the American
    Iris Society, 59, 2–5.

    Examples
    --------
    >>> import pyMixtComp
    >>> data, model = pyMixtComp.data.load_iris()
    """
    stream = pkg_resources.open_text(__package__, "iris.csv")
    data = pd.read_csv(stream)
    model = {
        "sepal length (cm)": "Gaussian",
        "sepal width (cm)": "Gaussian",
        "petal length (cm)": "Gaussian",
        "petal width (cm)": "Gaussian",
    }

    return data, model


def load_prostate():
    """Prostate cancer data

    This data set was obtained from a randomized clinical trial comparing four treatments for n = 506 patients
    with prostatic cancer grouped on clinical criteria into two Stages 3 and 4 of the disease.

    Returns
    -------
    (DataFrame, dict)
        Data are returned in a dataFrame with 12 columns (see Notes).
        Model is returned as a dict

    Notes
    -----
    data contains 506 individuals described by 12 mixed variables:
    - Age: Age (Continuous)
    - HG: Index of tumour stage and histolic grade (Continuous)
    - Wt: Weight (Continuous)
    - AP: Serum prostatic acid phosphatase C (Continuous)
    - BP: Systolic blood pressure (Continuous)
    - PF: Performance rating (Categorical)
    - DBP: Diastolic blood pressure (Continuous)
    - HX: Cardiovascular disease history (Categorical)
    - SG: Serum haemoglobin (Continuous)
    - BM: Bone metastasis (Categorical)
    - SZ: Size of primary tumour (Continuous)
    - EKG: Electrocardiogram code (Categorical)

    References
    ----------
    Yakovlev, Goot and Osipova (1994), The choice of cancer treatment based on covariate information. Statist. Med.,
    13: 1575-1581. doi:10.1002/sim.4780131508

    Examples
    --------
    >>> import pyMixtComp
    >>> data, model = pyMixtComp.data.load_prostate()
    """
    stream = pkg_resources.open_text(__package__, "prostate.csv")
    data = pd.read_csv(stream, dtype=object)
    model = {
        "Age": "Gaussian",
        "Wt": "Gaussian",
        "PF": "Multinomial",
        "HX": "Multinomial",
        "SBP": "Gaussian",
        "DBP": "Gaussian",
        "EKG": "Multinomial",
        "HG": "Gaussian",
        "SZ": "Gaussian",
        "SG": "Gaussian",
        "AP": "Gaussian",
        "BM": "Multinomial",
    }

    return data, model


def load_simulated_data():
    """Simulated data

    Data simulated from the different models used in MixtComp.

    Returns
    -------
    (DataFrame, dict)
        Data are returned in a dataFrame with 9 columns: one for each model in pyMixtComp and the true labels (z_class).
        Model is returned as a dict (z_class is not included but can be added to perform supervised clustering).

    Examples
    --------
    >>> import pyMixtComp
    >>> data, model = pyMixtComp.data.load_prostate()
    """
    stream = pkg_resources.open_text(__package__, "simulated_data.csv")
    data = pd.read_csv(stream, dtype=object)
    model = {
        "Poisson1": "Poisson",
        "Gaussian1": "Gaussian",
        "Categorical1": "Multinomial",
        "nBinom1": "NegativeBinomial",
        "Weibull1": "Weibull",
        "Functional1": {"type": "Func_CS", "paramStr": "nSub: 2, nCoeff: 2"},
        "FunctionalSharedAlpha1": {"type": "Func_SharedAlpha_CS", "paramStr": "nSub: 2, nCoeff: 2"},
        "Rank1": "Rank_ISR",
    }

    return data, model


def load_canadian_weather():
    """Canadian average annual weather cycle

    Daily temperature and precipitation at 35 different locations in Canada averaged over 1960 to 1994.
    Data from fda R package.

    Returns
    -------
    DataFrame, dict
        Data are returned in a dataFrame with 2 columns: tempav (average temperature in degrees celsius
        for each day of the year) and precav (average rainfall in millimeters for each day of the year).
        Model is returned as a dict

    References
    ----------
    - Ramsay, James O., and Silverman, Bernard W. (2006), Functional Data Analysis, 2nd ed., Springer, New York.
    - Ramsay, James O., and Silverman, Bernard W. (2002), Applied Functional Data Analysis, Springer, New York

    Examples
    --------
    >>> import pyMixtComp
    >>> data, model = pyMixtComp.data.load_canadian_weather()
    """
    stream = pkg_resources.open_text(__package__, "canadian_weather.csv")
    data = pd.read_csv(stream, dtype=object)
    model = {
        "tempav": {"type": "Func_CS", "paramStr": "nSub: 4, nCoeff: 2"},
        "precav": {"type": "Func_CS", "paramStr": "nSub: 4, nCoeff: 2"},
    }

    return data, model
