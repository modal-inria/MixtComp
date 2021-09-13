import pickle
import unittest

import numpy as np
import pandas as pd
from scipy.special import logsumexp

from pyMixtComp.utils.getter import _get_param_numerical, _get_param_multinomial, _get_param_Rank_ISR, \
                                    _get_param_Func_CS, get_param, get_partition, get_tik


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

    def test_get_param_rank(self):
        param = {"pi": {"stat": pd.DataFrame([[1, 2, 3], [4, 5, 6]],
                                             columns=["median", "q0", "q1"], index=["k: 0, pi", "k: 1, pi"])},
                 "mu": {"stat": {"k: 0": {"rank": np.array([3, 2, 1, 0])},
                                 "k: 1": {"rank": np.array([0, 1, 2, 3])}}},
                 "paramStr": "nModality: 4"}

        out = _get_param_Rank_ISR(param, 2)
        expected_out = {"pi": pd.DataFrame([[1], [4]], columns=["pi"], index=["0", "1"]),
                        "mu": np.array([[3, 2, 1, 0], [0, 1, 2, 3]])}
        self.assertEqual(out.keys(), expected_out.keys())
        self.assertDictEqual(out["pi"].to_dict(), expected_out["pi"].to_dict())
        self.assertListEqual(out["mu"].tolist(), expected_out["mu"].tolist())

        res = {"variable": {"param": {"rank": param}, "type": {"rank": "Rank_ISR"}},
               "algo": {"nClass": 2}}
        out = get_param(res, "rank")
        self.assertEqual(out.keys(), expected_out.keys())
        self.assertDictEqual(out["pi"].to_dict(), expected_out["pi"].to_dict())
        self.assertListEqual(out["mu"].tolist(), expected_out["mu"].tolist())

    def test_get_param_func_CS(self):
        param = {"alpha": {"stat": pd.DataFrame([[1, 2, 3], [4, 5, 6], [7, 8, 9], [10, 11, 12],
                                                 [13, 14, 15], [16, 17, 18], [19, 20, 21], [22, 23, 24]],
                                                columns=["median", "q0", "q1"],
                                                index=["k: 0, s: 0, alpha0", "k: 0, s: 0, alpha1", "k: 0, s: 1, alpha0",
                                                       "k: 0, s: 1, alpha1", "k: 1, s: 0, alpha0", "k: 1, s: 0, alpha1",
                                                       "k: 1, s: 1, alpha0", "k: 1, s: 1, alpha1"])},
                 "beta": {"stat": pd.DataFrame([[1, 2, 3], [4, 5, 6], [7, 8, 9], [10, 11, 12],
                                                [13, 14, 15], [16, 17, 18], [19, 20, 21], [22, 23, 24]],
                                               columns=["median", "q0", "q1"],
                                               index=["k: 0, s: 0, c: 0", "k: 0, s: 0, c: 1", "k: 0, s: 1, c: 0",
                                                      "k: 0, s: 1, c: 1", "k: 1, s: 0, c: 0", "k: 1, s: 0, c: 1",
                                                      "k: 1, s: 1, c: 0", "k: 1, s: 1, c: 1"])},
                 "sd": {"stat": pd.DataFrame([[1, 2, 3], [4, 5, 6], [7, 8, 9], [10, 11, 12]],
                                             columns=["median", "q0", "q1"],
                                             index=["k: 0, s: 0", "k: 0, s: 1", "k: 1, s: 0", "k: 1, s: 1"])},
                 "paramStr": "nSub: 2, nCoeff: 2"}

        out = _get_param_Func_CS(param, 2)
        expected_out = {"alpha": pd.DataFrame([[1, 4, 7, 10], [13, 16, 19, 22]],
                                              columns=["s: 0, alpha0", "s: 0, alpha1", "s: 1, alpha0", "s: 1, alpha1"],
                                              index=["0", "1"]),
                        "beta": pd.DataFrame([[1, 4, 7, 10], [13, 16, 19, 22]],
                                             columns=["s: 0, c: 0", "s: 0, c: 1", "s: 1, c: 0", "s: 1, c: 1"],
                                             index=["0", "1"]),
                        "sd": pd.DataFrame([[1, 4], [7, 10]], columns=["s: 0", "s: 1"], index=["0", "1"])}
        self.assertEqual(out.keys(), expected_out.keys())
        self.assertDictEqual(out["alpha"].to_dict(), expected_out["alpha"].to_dict())
        self.assertDictEqual(out["beta"].to_dict(), expected_out["beta"].to_dict())
        self.assertDictEqual(out["sd"].to_dict(), expected_out["sd"].to_dict())

        res = {"variable": {"param": {"func": param}, "type": {"func": "Func_CS"}},
               "algo": {"nClass": 2}}
        out = get_param(res, "func")
        self.assertEqual(out.keys(), expected_out.keys())
        self.assertDictEqual(out["alpha"].to_dict(), expected_out["alpha"].to_dict())
        self.assertDictEqual(out["beta"].to_dict(), expected_out["beta"].to_dict())
        self.assertDictEqual(out["sd"].to_dict(), expected_out["sd"].to_dict())

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
