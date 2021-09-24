import pickle
import unittest

import numpy as np

from pyMixtComp.utils.sample import sample_multinomial


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


if __name__ == "__main__":
    unittest.main()
