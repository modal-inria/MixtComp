import pickle
import unittest

import numpy as np
import pandas as pd

from pyMixtComp.utils.convert import convert_functional
from pyMixtComp.utils.getter import get_param
from pyMixtComp.utils.sample import sample_multinomial, switch_representation_rank, sample_Rank_ISR, \
    log_to_multi, sample_Func_CS, sample


class TestUtilsGetter(unittest.TestCase):
    def setUp(self):
        self.res = pickle.load(open("tests/mixtcomp_output_full.pickle", "rb"))

    def test_sample_multinomial(self):
        p = [0.5, 0.5]
        out = sample_multinomial(p, 100, np.random.default_rng(42))
        self.assertIsInstance(out, np.ndarray)
        self.assertEqual(len(out), 100)
        self.assertEqual(np.sum(out == 1), 53)
        self.assertEqual(np.sum(out == 0), 47)

        p = [0.1, 0.2, 0.7]
        out = sample_multinomial(p, 100, np.random.default_rng(42))
        self.assertIsInstance(out, np.ndarray)
        self.assertEqual(len(out), 100)
        self.assertEqual(np.sum(out == 0), 7)
        self.assertEqual(np.sum(out == 1), 21)
        self.assertEqual(np.sum(out == 2), 72)

    def test_switch_representation_rank(self):
        rank = np.array([0, 1, 2, 3])
        out = switch_representation_rank(rank)
        self.assertListEqual(out.tolist(), rank.tolist())

        rank = np.array([2, 0, 1, 3])
        out = switch_representation_rank(rank)
        self.assertListEqual(out.tolist(), [1, 2, 0, 3])

    def test_sample_Rank_ISR(self):
        mu = np.array([0, 1, 2, 3])
        pi = 0.95
        data = sample_Rank_ISR(mu, pi, size=2, random_state=np.random.default_rng(42), convert_to_str=True)
        self.assertListEqual(data.tolist(), ["0,1,2,3", "0,1,2,3"])

        data = sample_Rank_ISR(mu, pi, size=2, random_state=np.random.default_rng(42), convert_to_str=False)
        self.assertListEqual(data.tolist(), [[0, 1, 2, 3], [0, 1, 2, 3]])

    def test_log_to_multi(self):
        logproba = np.log(np.array([2.5, 2.5]))
        out = log_to_multi(logproba)
        self.assertListEqual(out.tolist(), [0.5, 0.5])

        logproba = np.log(np.array([0.5, 1.5]))
        out = log_to_multi(logproba)
        self.assertAlmostEqual(np.max(np.abs(out - np.array([0.25, 0.75]))), 0)

    def test_sample_Func_CS(self):
        param = get_param(self.res, "Functional1")
        alpha = param["alpha"].iloc[0].values
        beta = param["beta"].iloc[0].values
        sd = param["sd"].iloc[0].values
        t = np.linspace(0, 20, 21)

        out = sample_Func_CS(alpha, beta, sd, t, size=2, random_state=np.random.default_rng(42))
        self.assertEqual(len(out), 2)
        self.assertIsInstance(out[0], str)
        self.assertIsInstance(out[1], str)
        t0, v0 = convert_functional(out[0])
        t1, v1 = convert_functional(out[1])
        self.assertListEqual(t0.tolist(), t.tolist())
        self.assertListEqual(t1.tolist(), t.tolist())
        self.assertAlmostEqual(v0[0], 8, places=-1)
        self.assertAlmostEqual(v0[10], 0, places=-1)
        self.assertAlmostEqual(v0[-1], 8, places=-1)
        self.assertAlmostEqual(v1[0], 8, places=-1)
        self.assertAlmostEqual(v1[10], 0, places=-1)
        self.assertAlmostEqual(v1[-1], 8, places=-1)

    def test_sample(self):
        x, z = sample(self.res, 10, random_state=np.random.default_rng(42))

        self.assertIsInstance(x, pd.DataFrame)
        self.assertIsInstance(z, pd.Series)
        self.assertEqual(x.shape, (10, 8))
        self.assertEqual(z.shape, (10,))
        self.assertListEqual(x.columns.values.tolist(), list(self.res["variable"]["type"].keys() - {"z_class"}))


if __name__ == "__main__":
    unittest.main()
