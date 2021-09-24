import numpy as np
import pandas as pd
from scipy.stats import norm, weibull_min, multinomial, poisson, nbinom, uniform

def sample_multinomial(p, size=1, random_state=None):
    return np.where(multinomial.rvs(1, p, size, random_state) == 1)[1]

