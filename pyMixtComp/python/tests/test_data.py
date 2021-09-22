import unittest

import pandas as pd

from pyMixtComp.data import load_prostate, load_canadian_weather, load_simulated_data


class TestData(unittest.TestCase):
    def test_load_prostate(self):
        data, model = load_prostate()

        self.assertIsInstance(data, pd.DataFrame)
        self.assertEqual(data.shape, (506, 12))
        self.assertEqual(len(model), 12)
        self.assertListEqual(list(model.keys()), list(data.columns))

    def test_load_canadian_weather(self):
        data, model = load_canadian_weather()

        self.assertIsInstance(data, pd.DataFrame)
        self.assertEqual(data.shape, (35, 2))
        self.assertEqual(len(model), 2)
        self.assertListEqual(list(model.keys()), list(data.columns))

    def test_load_simulated_data(self):
        data, model = load_simulated_data()

        self.assertIsInstance(data, pd.DataFrame)
        self.assertEqual(data.shape, (200, 9))
        self.assertEqual(len(model), 8)
        self.assertListEqual(list(model.keys()), list(data.columns[1:]))


if __name__ == "__main__":
    unittest.main()
