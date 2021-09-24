import numpy as np
import pandas as pd
from scipy.stats import norm, weibull_min, multinomial, poisson, nbinom, uniform

from .convert import create_rank
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

