import matplotlib.pyplot as plt
import numpy as np
from scipy.stats import norm
from scipy.optimize import root_scalar

import seaborn as sns


def plot_functional_data(bounds, res, var_name, add_obs=False, add_CI=True):
    """ Plot bounds for functional data"""
    _, ax = plt.subplots()
    if add_obs:
        for i in range(len(res["variable"]["data"][var_name]["time"])):
            sns.lineplot(x=res["variable"]["data"][var_name]["time"][i], y=res["variable"]["data"][var_name]["data"][i],
                         linewidth=2, alpha=0.5, color="black", ax=ax)

    for i in bounds[4]:
        sns.lineplot(x=bounds[0], y=bounds[2][i], label="Class " + str(i), ax=ax)

    if add_CI:
        for i in bounds[4]:
            ax.fill_between(bounds[0], bounds[1][i], bounds[3][i], alpha=0.5)

    ax.set_title("Mean curves and 95% level confidence intervals per class")
    ax.set_xlabel("Time")
    ax.set_ylabel(var_name)

    return ax


def extract_CI_Func_CS(res, var_name, class_ids=None):
    """ Compute mean and 95% CI for functional model"""
    n_class = res["algo"]["nClass"]

    if class_ids is None:
        class_ids = list(range(n_class))

    time_values = np.unique(res["variable"]["data"][var_name]["time"])
    time_values.sort()

    param = res["variable"]["param"][var_name]

    n_sub = int(len(param["sd"]["stat"]) / n_class)
    n_coeff = int(len(param["beta"]["stat"]) / n_class / n_sub)

    alpha = param["alpha"]["stat"]["median"].values.reshape(-1, 2)
    alpha = [alpha[(g * n_sub):((g+1) * n_sub)] for g in range(n_class)]

    beta = param["beta"]["stat"]["median"].values.reshape(-1, n_coeff)
    beta = [beta[(g * n_sub):((g+1) * n_sub)] for g in range(n_class)]
    sigma = param["sd"]["stat"]["median"].values.reshape(n_class, n_sub)

    inf_curves, mean_curves, sup_curves = compute_functional_bounds(time_values, alpha, beta, sigma, class_ids)

    return time_values, inf_curves, mean_curves, sup_curves, class_ids


def compute_functional_bounds(time_values, alpha, beta, sigma, class_ids):
    """ Compute lower, mean and upper curve for each class """
    n_class = len(alpha)
    func_mean = np.empty((n_class, len(time_values)))
    func_lower = np.empty((n_class, len(time_values)))
    func_upper = np.empty((n_class, len(time_values)))

    for k in class_ids:
        for i in range(len(time_values)):
            weights = _compute_functional_weights(time_values[i], alpha[k])
            trends = _compute_functional_trends(time_values[i], beta[k])

            func_mean[k, i] = np.sum(trends * weights)
            func_lower[k, i] = _compute_functional_bound(weights, trends, sigma[k], bound=0.025)
            func_upper[k, i] = _compute_functional_bound(weights, trends, sigma[k], bound=0.975)

    return func_lower, func_mean, func_upper


def _compute_functional_weights(t, alpha):
    weights = alpha[:, 1] * t + alpha[:, 0]
    weights -= weights.max()
    weights = np.exp(weights) / np.sum(np.exp(weights))  # To avoid numerical problems
    weights = weights / weights.sum()

    return weights


def _compute_functional_trends(t, beta):
    trends = 0
    for i in range(beta.shape[1]):
        trends += beta[:, i] * t ** i

    return trends


def objective_functional(x, pi, mu, s, threshold):
    """objective function to found quantile for functional model"""
    return np.sum(pi * norm.cdf(x, loc=mu, scale=s)) - threshold


def _compute_functional_bound(weights, means, sigma, bound):
    """ estimate the quantile at level bound for functional model"""
    bracket = np.array([norm.ppf(bound - 0.001, means, sigma), norm.ppf(bound + 0.001, means, sigma)])
    bracket = [bracket.min(), bracket.max()]

    min_value = root_scalar(objective_functional, args=(weights, means, sigma, bound), method="brentq", bracket=bracket)

    return min_value.root
