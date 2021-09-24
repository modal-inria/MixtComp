import unittest

import numpy as np
import pandas as pd

from pyMixtComp.utils.convert import create_functional, convert_functional, refactor_categorical, create_rank, convert_rank


class TestUtilsConvert(unittest.TestCase):

    def test_create_functional_empty(self):
        times = []
        values = []
        func = create_functional(times, values)
        self.assertEqual(func, "")

        func = create_functional(np.array(times), np.array(values))
        self.assertEqual(func, "")

        func = create_functional(pd.Series(times), pd.Series(values))
        self.assertEqual(func, "")

    def test_create_functional(self):
        times = [1., 2., 3.]
        values = [4, 5, 6]
        func = create_functional(times, values)
        self.assertEqual(func, "1.0:4,2.0:5,3.0:6")

        func = create_functional(np.array(times), np.array(values))
        self.assertEqual(func, "1.0:4,2.0:5,3.0:6")

        func = create_functional(pd.Series(times), pd.Series(values))
        self.assertEqual(func, "1.0:4,2.0:5,3.0:6")

    def test_create_functional_missing_values(self):
        times = [1., 2., np.nan, 4., 5.]
        values = [4, 5, 6, None, 8]
        func = create_functional(times, values)
        self.assertEqual(func, "1.0:4,2.0:5,5.0:8")

        func = create_functional(np.array(times), np.array(values))
        self.assertEqual(func, "1.0:4,2.0:5,5.0:8")

        func = create_functional(pd.Series(times), pd.Series(values))
        self.assertEqual(func, "1.0:4.0,2.0:5.0,5.0:8.0")

    def test_convert_functional_empty(self):
        func = ""
        times, values = convert_functional(func)
        self.assertListEqual(times.tolist(), [])
        self.assertListEqual(values.tolist(), [])

    def test_convert_functional(self):
        func = "1.0:4,2.0:5,3.0:6"
        times, values = convert_functional(func)
        self.assertListEqual(times.tolist(), [1., 2., 3.])
        self.assertListEqual(values.tolist(), [4., 5., 6.])

    def test_refactor_categorical(self):
        data = [0.0, 1.0, 0.0]
        new_categ = ["a", "b"]
        old_categ = [0, 1]

        out = refactor_categorical(data, old_categ, new_categ)
        self.assertListEqual(list(out), ["a", "b", "a"])

        out = refactor_categorical(np.array(data), old_categ, new_categ)
        self.assertListEqual(list(out), ["a", "b", "a"])
        self.assertIsInstance(out, np.ndarray)

        out = refactor_categorical(pd.Series(data), old_categ, new_categ)
        self.assertListEqual(list(out), ["a", "b", "a"])
        self.assertIsInstance(out, pd.Series)

    def test_refactor_categorical_more_old(self):
        data = np.array([0.0, 1.0, 0.0])
        new_categ = ["a", "b", "c"]
        old_categ = [0, 1, 2]

        out = refactor_categorical(data, old_categ, new_categ)

        self.assertListEqual(list(out), ["a", "b", "a"])
        self.assertIsInstance(out, np.ndarray)

    def test_refactor_categorical_less_old(self):
        data = np.array([0.0, 1.0, 0.0, 2.0])
        new_categ = ["a", "b"]
        old_categ = [0, 1]

        out = refactor_categorical(data, old_categ, new_categ)

        self.assertListEqual(list(out), ["a", "b", "a", 2.0])
        self.assertIsInstance(out, np.ndarray)

    def test_convert_rank(self):
        rank_str = "0, 1, 2, 3"
        rank = convert_rank(rank_str)
        self.assertListEqual(rank.tolist(), [0, 1, 2, 3])

        rank_str = "0,1,2,3"
        rank = convert_rank(rank_str)
        self.assertListEqual(rank.tolist(), [0, 1, 2, 3])

    def test_create_rank(self):
        rank = np.array([0, 1, 2, 3])
        rank_str = create_rank(rank)
        self.assertEqual(rank_str, "0,1,2,3")


if __name__ == "__main__":
    unittest.main()
