import pickle
import unittest

import numpy as np

from pyMixtComp.utils.sample import sample_multinomial, switch_representation_rank, sample_Rank_ISR


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


if __name__ == "__main__":
    unittest.main()
