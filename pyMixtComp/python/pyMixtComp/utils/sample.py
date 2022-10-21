import numpy as np
import pandas as pd
from scipy.stats import multinomial, nbinom, norm, poisson, uniform, weibull_min

from .convert import create_rank
from .getter import get_param


def sample(res, size, random_state=None):
    """ Generate random samples from the fitted mixture model.

    Parameters
    ----------
    res : dict
        output of multi_run_pmc_pool
    size : int
        number of samples to generate
    random_state : Generator, optional
        Random number generator from numpy, by default None

    Returns
    -------
    DataFrame
        Randomly generated sample
    Series
        Component labels

    Raises
    ------
    NotImplementedError
        Not yet implemented for model
    """
    n_class = res["algo"]["nClass"]
    var_names = list(res["variable"]["type"].keys() - {"z_class"})

    dat = pd.DataFrame(columns=var_names, index=range(size))

    proportion = get_param(res, "z_class")
    z_class = pd.Series(sample_multinomial(proportion["pi"], size, random_state))

    size_k = z_class.value_counts().to_dict()
    for var_name in var_names:
        model = res["variable"]["type"][var_name]
        param = get_param(res, var_name)

        for k in range(n_class):
            if model == "Gaussian":
                dat.loc[z_class == k, var_name] = norm.rvs(loc=param["mean"].iloc[k], scale=param["sd"].iloc[k],
                                                           size=size_k[k], random_state=random_state)
            elif model == "Multinomial":
                dat.loc[z_class == k, var_name] = sample_multinomial(p=param.iloc[k], size=size_k[k],
                                                                     random_state=random_state)
            elif model == "Poisson":
                dat.loc[z_class == k, var_name] = poisson.rvs(mu=param["lambda"].iloc[k], size=size_k[k],
                                                              random_state=random_state)
            elif model == "NegativeBinomial":
                dat.loc[z_class == k, var_name] = nbinom.rvs(n=param["n"].iloc[k], p=param["p"].iloc[k],
                                                             size=size_k[k], random_state=random_state)
            elif model == "Weibull":
                dat.loc[z_class == k, var_name] = weibull_min.rvs(c=param["k (shape)"].iloc[k],
                                                                  scale=param["lambda (scale)"].iloc[k],
                                                                  size=size_k[k], random_state=random_state)
            elif (model == "Func_CS") | (model == "Func_SharedAlpha_CS"):
                all_time_values = np.unique(res["variable"]["data"][var_name]["time"])
                t = np.linspace(all_time_values.min(), all_time_values.max(), 100)
                dat.loc[z_class == k, var_name] = sample_Func_CS(param["alpha"].iloc[k].values, param["beta"].iloc[k].values,
                                                                 param["sd"].iloc[k].values, t, size=size_k[k],
                                                                 random_state=random_state)
            elif model == "Rank_ISR":
                dat.loc[z_class == k, var_name] = sample_Rank_ISR(param["mu"][k], param["pi"].values[k][0], size=size_k[k],
                                                                  random_state=random_state, convert_to_str=True)
            else:
                raise NotImplementedError("Not yet implemented for model " + model)

    return dat, z_class


def sample_multinomial(p, size=1, random_state=None):
    return np.where(multinomial.rvs(1, p, size, random_state) == 1)[1]


def sample_Rank_ISR(mu, pi, size=1, random_state=None, convert_to_str=True):
    """ Sample ranks from an ISR(mu, pi)

    Parameters
    ----------
    mu : array
        Central rank parameter of ISR model in ordering presentation.
    pi : float
        pi parameter of ISR model: probability that a comparison is a good one.
    size : int, optional
        Number of samples to generate, by default 1
    random_state : Generator, optional
        Random number generator from numpy, by default None
    convert_to_str : bool, optional
        If True, each rank is returned as str, by default True

    Returns
    -------
    array
        array of generated ranks
    """
    if random_state is None:
        random_state = np.random.default_rng()

    n_modality = len(mu)
    mu_ranking = switch_representation_rank(mu)
    data = []
    for _ in range(size):
        y = random_state.permutation(mu)

        generated_rank = []
        generated_rank.append(y[0])

        for i in range(1, n_modality):
            curr_y = y[i]
            y_placed = False

            for j in range(i):
                comparison = mu_ranking[curr_y] < mu_ranking[generated_rank[j]]

                # is the comparison incorrect ? if yes we negate comparison
                if pi < uniform.rvs(size=1, random_state=random_state):
                    comparison = not comparison

                if comparison:
                    generated_rank.insert(j, curr_y)
                    y_placed = True
                    break

            if not y_placed:
                generated_rank.insert(i, curr_y)

        generated_rank = np.array(generated_rank)
        if convert_to_str:
            generated_rank = create_rank(generated_rank)

        data.append(generated_rank)

    data = np.array(data)

    return data


def switch_representation_rank(rank):
    """ Switch rank from ordering to ranking representation or vice-versa

    Parameters
    ----------
    rank : array
        rank

    Returns
    -------
    array
        rank in the other representation
    """
    n_modality = len(rank)
    converted_rank = np.empty(n_modality, dtype=int)
    for i in range(n_modality):
        converted_rank[rank[i]] = i

    return converted_rank


def sample_Func_CS(alpha, beta, sd, t, size=1, random_state=None):
    """ Sample functional data from a Func_CS(alpha, beta, sd)

    Parameters
    ----------
    alpha : array
        alpha parameters
    beta : array
        beta parameters
    sd : array
        standard deviation for each subregression
    t : array
        times values to compute the functional
    size : int, optional
        Number of samples to generate, by default 1
    random_state : Generator, optional
        Random number generator from numpy, by default None

    Returns
    -------
    array
        array of generated functional data
    """
    n_sub = len(sd)
    n_coeff = int(len(beta) / n_sub)

    data = []
    for _ in range(size):
        func = ""
        for i in range(len(t)):
            # choose the current subregression
            log_kappa = alpha[::2] + alpha[1::2] * t[i]
            kappa = log_to_multi(log_kappa)
            ind_sub = sample_multinomial(kappa, size=1, random_state=None)[0]

            # mean regression
            x_loc = beta[(n_coeff * ind_sub):(n_coeff * (ind_sub + 1))].dot(t[i] ** np.arange(0, n_coeff))

            # normal error
            x = norm.rvs(loc=x_loc, scale=sd[ind_sub], size=1, random_state=random_state)[0]

            func += str(t[i]) + ":" + str(x) + ","

        # we remove the extra ","
        if len(func) != 0:
            func = func[:-1]

        data.append(func)

    data = np.array(data)

    return data


def log_to_multi(log_in):
    """ Compute tik using log(proba)"""
    m = log_in.max()
    out = np.exp(log_in - m)
    out /= out.sum()

    return out
