import pickle
import unittest

import numpy as np
import pandas as pd
from scipy.special import logsumexp

from pyMixtComp.utils.getter import _get_param_numerical, _get_param_multinomial, get_param, get_partition, get_tik


class TestUtils(unittest.TestCase):
    def setUp(self):
        self.res = pickle.load(open("tests/mixtcomp_output.pickle", "rb"))

    def test_get_param_numerical_gaussian(self):
        param = {"stat": pd.DataFrame([[1, 2, 3], [4, 5, 6], [7, 8, 9], [10, 11, 12]],
                                      columns=["median", "q0", "q1"],
                                      index=["k: 0, mean", "k: 0, sd", "k: 1, mean", "k: 1, sd"])}

        out = _get_param_numerical(param, 2, ("mean", "sd"))
        expected_out = pd.DataFrame([[1, 4], [7, 10]], columns=["mean", "sd"], index=["0", "1"])
        self.assertDictEqual(out.to_dict(), expected_out.to_dict())

        res = {"variable": {"param": {"gauss": param}, "type": {"gauss": "Gaussian"}},
               "algo": {"nClass": 2}}
        out = get_param(res, "gauss")
        self.assertDictEqual(out.to_dict(), expected_out.to_dict())

    def test_get_param_numerical_poisson(self):
        param = {"stat": pd.DataFrame([[1, 2, 3], [4, 5, 6]],
                                      columns=["median", "q0", "q1"],
                                      index=["k: 0, lambda", "k: 1, lambda"])}

        out = _get_param_numerical(param, 2, ["lambda"])
        expected_out = pd.DataFrame([[1], [4]], columns=["lambda"], index=["0", "1"])
        self.assertDictEqual(out.to_dict(), expected_out.to_dict())

        res = {"variable": {"param": {"pois": param}, "type": {"pois": "Poisson"}},
               "algo": {"nClass": 2}}
        out = get_param(res, "pois")
        self.assertDictEqual(out.to_dict(), expected_out.to_dict())

    def test_get_param_multinomial(self):
        param = {"stat": pd.DataFrame([[1, 2, 3], [4, 5, 6], [7, 8, 9], [10, 11, 12], [13, 14, 15], [16, 17, 18]],
                                      columns=["median", "q0", "q1"],
                                      index=["k: 0, modality: 0", "k: 0, modality: 1", "k: 0, modality: 2",
                                             "k: 1, modality: 0", "k: 1, modality: 1", "k: 1, modality: 2"])}

        out = _get_param_multinomial(param, 2)
        expected_out = pd.DataFrame([[1, 4, 7], [10, 13, 16]], columns=["0", "1", "2"], index=["0", "1"])
        self.assertDictEqual(out.to_dict(), expected_out.to_dict())

        res = {"variable": {"param": {"mult": param}, "type": {"mult": "Multinomial"}},
               "algo": {"nClass": 2}}
        out = get_param(res, "mult")
        self.assertDictEqual(out.to_dict(), expected_out.to_dict())

    def test_get_tik(self):
        out = get_tik(self.res, log=False, empiric=False)
        self.assertListEqual(out.tolist(),
                             self.res["variable"]["data"]["z_class"]["stat"].to_numpy().tolist())
        self.assertIsInstance(out, np.ndarray)

        out = get_tik(self.res, log=True, empiric=False)
        self.assertListEqual(out.tolist(),
                             np.log(self.res["variable"]["data"]["z_class"]["stat"].to_numpy()).tolist())
        self.assertIsInstance(out, np.ndarray)

    def test_get_tik_empiric(self):
        out_log = get_tik(self.res, log=True, empiric=True)
        self.assertListEqual(out_log.tolist(),
                             (self.res["mixture"]["lnProbaGivenClass"] - logsumexp(self.res["mixture"]["lnProbaGivenClass"],
                                                                                   axis=1)[:, np.newaxis]).tolist())
        self.assertIsInstance(out_log, np.ndarray)

        out = get_tik(self.res, log=False, empiric=True)
        self.assertListEqual(out.tolist(), np.exp(out_log).tolist())
        self.assertIsInstance(out, np.ndarray)

    def test_get_partition_empiric(self):
        out = get_partition(self.res, empiric=True)
        self.assertListEqual(out.tolist(), self.res["variable"]["data"]["z_class"]["completed"].tolist())
        self.assertIsInstance(out, np.ndarray)

    def test_get_partition(self):
        out = get_partition(self.res, empiric=False)
        tik = get_tik(self.res, log=True, empiric=True)
        expected_out = np.argmax(tik, axis=1)
        self.assertListEqual(out.tolist(), expected_out.tolist())
        self.assertIsInstance(out, np.ndarray)


if __name__ == "__main__":
    unittest.main()
