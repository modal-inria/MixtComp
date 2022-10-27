import pickle
import unittest

import numpy as np
import pandas as pd
from pyMixtComp.bridge.utils import (
    _impute_model,
    _refactor_categorical,
    create_algo,
    format_data_basic_mode,
    format_model,
    format_output_basic_mode,
    impute_model,
)


class TestUtils(unittest.TestCase):
    def setUp(self):
        self.n_components = 2
        self.n_burn_in_iter = 10
        self.n_iter = 15
        self.n_gibbs_burn_in_iter = 20
        self.n_gibbs_iter = 25
        self.n_init_per_class = 30
        self.n_sem_try = 35
        self.confidence_level = 0.9
        self.ratio_stable_criterion = 0.95
        self.n_stable_criterion = 40

    def test_create_algo(self):
        out = create_algo(self, {"a": [1, 2], "b": [3, 4], "c": [5, 6]}, "learn")
        expected_out = {
            "nbBurnInIter": self.n_burn_in_iter,
            "nbIter": self.n_iter,
            "nbGibbsBurnInIter": self.n_gibbs_burn_in_iter,
            "nbGibbsIter": self.n_gibbs_iter,
            "nInitPerClass": self.n_init_per_class,
            "nSemTry": self.n_sem_try,
            "confidenceLevel": self.confidence_level,
            "ratioStableCriterion": self.ratio_stable_criterion,
            "nStableCriterion": self.n_stable_criterion,
            "nClass": self.n_components,
            "nInd": 2,
            "mode": "learn"
        }

        self.assertDictEqual(out, expected_out)

    def test_format_model_full(self):
        expected_out = {"a": {"type": "Gaussian", "paramStr": ""},
                        "b": {"type": "Multinomial", "paramStr": ""},
                        "c": {"type": "Poisson", "paramStr": ""}}

        model = {"a": {"type": "Gaussian", "paramStr": ""},
                 "b": {"type": "Multinomial", "paramStr": ""},
                 "c": {"type": "Poisson", "paramStr": ""}}
        format_model(model)
        self.assertDictEqual(model, expected_out)

    def test_format_model_reduced(self):
        expected_out = {"a": {"type": "Gaussian", "paramStr": ""},
                        "b": {"type": "Multinomial", "paramStr": ""},
                        "c": {"type": "Poisson", "paramStr": ""}}

        model = {"a": "Gaussian", "b": "Multinomial", "c": "Poisson"}
        format_model(model)
        self.assertDictEqual(model, expected_out)

    def test_format_model_semi_reduced(self):
        expected_out = {"a": {"type": "Gaussian", "paramStr": ""},
                        "b": {"type": "Multinomial", "paramStr": ""},
                        "c": {"type": "Poisson", "paramStr": ""}}
        model = {"a": {"type": "Gaussian"}, "b": {"type": "Multinomial"}, "c": {"type": "Poisson"}}
        format_model(model)
        self.assertDictEqual(model, expected_out)

    def test_format_model_mixt(self):
        expected_out = {"a": {"type": "Gaussian", "paramStr": ""},
                        "b": {"type": "Multinomial", "paramStr": ""},
                        "c": {"type": "Poisson", "paramStr": ""}}
        model = {"a": {"type": "Gaussian", "paramStr": ""}, "b": {"type": "Multinomial"}, "c": "Poisson"}
        format_model(model)
        self.assertDictEqual(model, expected_out)

    def test_impute_model(self):
        self.assertEqual(_impute_model("gauss", np.float64), "Gaussian")
        self.assertEqual(_impute_model("pois", np.int64), "Poisson")
        self.assertEqual(_impute_model("z_class", np.int64), "LatentClass")
        self.assertEqual(_impute_model("z_class", str), "LatentClass")
        self.assertEqual(_impute_model("mult", str), "Multinomial")
        with self.assertRaises(TypeError):
            _impute_model("mult", np.datetime64)

    def test_impute_model_DataFrame(self):
        model = impute_model(pd.DataFrame({"gauss": [1.5, 2.5],
                                           "pois": [1, 2],
                                           "mult": ["a", "b"],
                                           "z_class": [1, 2]}))

        self.assertDictEqual(model, {"gauss": "Gaussian", "pois": "Poisson",
                                     "mult": "Multinomial", "z_class": "LatentClass"})

    def test_impute_model_array(self):
        model = impute_model(np.array([[1, 2], [3, 4]]))
        self.assertDictEqual(model, {"var0": "Poisson", "var1": "Poisson"})

        model = impute_model(np.array([[1., 2.], [3., 4.]]))
        self.assertDictEqual(model, {"var0": "Gaussian", "var1": "Gaussian"})

        model = impute_model(np.array([["a", "a"], ["b", "b"]]))
        self.assertDictEqual(model, {"var0": "Multinomial", "var1": "Multinomial"})

    def test_impute_model_dict(self):
        model = impute_model({"gauss": [1.5, 2.5],
                              "pois": [1, 2],
                              "mult": ["a", "b"],
                              "z_class": [1, 2]})
        self.assertDictEqual(model, {"gauss": "Gaussian", "pois": "Poisson",
                                     "mult": "Multinomial", "z_class": "LatentClass"})

    def test_format_data_basic_mode_without_dict(self):
        model = {"gauss": {"type": "Gaussian", "paramStr": ""},
                 "pois": {"type": "Poisson", "paramStr": ""},
                 "mult": {"type": "Multinomial", "paramStr": ""},
                 "z_class": {"type": "LatentClass", "paramStr": ""}}

        data = {"gauss": [1.5, np.nan, None],
                "pois": [1, 4, 2],
                "mult": ["a", "b", None],
                "z_class": [1, 0, 1]}

        dictionary = None
        data, dictionary = format_data_basic_mode(data, model, dictionary)

        self.assertEqual(dictionary, {"mult": {"old": ["a", "b"], "new": ["0", "1"]}})
        self.assertEqual(data, {"gauss": ["1.5", "?", "?"],
                                "pois": ["1", "4", "2"],
                                "mult": ["0", "1", "?"],
                                "z_class": ["1", "0", "1"]})

    def test_format_data_basic_mode_with_dict(self):
        model = {"gauss": {"type": "Gaussian", "paramStr": ""},
                 "pois": {"type": "Poisson", "paramStr": ""},
                 "mult": {"type": "Multinomial", "paramStr": ""},
                 "z_class": {"type": "LatentClass", "paramStr": ""}}

        data = {"gauss": [1.5, np.nan, None],
                "pois": [1, 4, 2],
                "mult": ["a", "b", None],
                "z_class": [1, 0, 1]}

        dictionary = {"mult": {"old": ["a", "b"], "new": ["1", "0"]}}
        data, dictionary = format_data_basic_mode(data, model, dictionary)

        self.assertEqual(dictionary, {"mult": {"old": ["a", "b"], "new": ["1", "0"]}})
        self.assertEqual(data, {"gauss": ["1.5", "?", "?"],
                                "pois": ["1", "4", "2"],
                                "mult": ["1", "0", "?"],
                                "z_class": ["1", "0", "1"]})

    def test_refactor_categorical_with_series(self):
        data = pd.Series([0.0, 1.0, 0.0])
        new_categ = ["a", "b"]
        old_categ = ["0", "1"]

        out = _refactor_categorical(data, old_categ, new_categ)

        self.assertListEqual(out.to_list(), ["a", "b", "a"])

    def test_refactor_categorical_with_array(self):
        data = np.array([0.0, 1.0, 0.0])
        new_categ = ["a", "b"]
        old_categ = ["0", "1"]

        out = _refactor_categorical(data, old_categ, new_categ)

        self.assertListEqual(list(out), ["a", "b", "a"])
        self.assertIsInstance(out, np.ndarray)

    def test_format_output_basic_mode(self):
        with open("tests/mixtcomp_output.pickle", "rb") as f:
            res = pickle.load(f)

            dictionary = {"mult": {"old": ["a", "b", "c", "d"], "new": ["0", "1", "2", "3"]},
                          "mult2": {"old": ["d", "c", "b", "a"], "new": ["0", "1", "2", "3"]},
                          "z_class": {"old": ["A", "B"], "new": ["0", "1"]}}

            expected_mult = res["variable"]["data"]["mult"]["completed"].copy().astype(str)
            for i in range(4):
                expected_mult[expected_mult == str(i)] = dictionary["mult"]["old"][i]

            expected_mult2 = res["variable"]["data"]["mult2"]["completed"].copy().astype(str)
            for i in range(4):
                expected_mult2[expected_mult2 == str(i)] = dictionary["mult2"]["old"][i]

            expected_z_class = res["variable"]["data"]["z_class"]["completed"].copy().astype(str)
            for i in range(2):
                expected_z_class[expected_z_class == str(i)] = dictionary["z_class"]["old"][i]

            format_output_basic_mode(res, dictionary)

            self.assertDictEqual(res["algo"]["dictionary"], dictionary)

            self.assertListEqual(res["variable"]["data"]["mult"]["stat"]["4"]["modality"].to_list(), ["b", "d", "c"])
            self.assertListEqual(res["variable"]["data"]["mult"]["stat"]["5"]["modality"].to_list(), ["b", "c", "d"])
            self.assertListEqual(list(res["variable"]["data"]["z_class"]["stat"].columns), ["k: A", "k: B"])

            self.assertListEqual(list(res["variable"]["data"]["mult"]["completed"]), list(expected_mult))
            self.assertListEqual(list(res["variable"]["data"]["mult2"]["completed"]), list(expected_mult2))
            self.assertListEqual(list(res["variable"]["data"]["z_class"]["completed"]), list(expected_z_class))

            self.assertListEqual(list(res["variable"]["param"]["mult"]["stat"].index),
                                 ["k: " + k + ", modality: " + m for k, m in zip(np.repeat(dictionary["z_class"]["old"], 4),
                                                                                 dictionary["mult"]["old"] * 2)])
            self.assertListEqual(list(res["variable"]["param"]["mult2"]["stat"].index),
                                 ["k: " + k + ", modality: " + m for k, m in zip(np.repeat(dictionary["z_class"]["old"], 4),
                                                                                 dictionary["mult2"]["old"] * 2)])
            self.assertListEqual(list(res["variable"]["param"]["gauss"]["stat"].index),
                                 ["k: " + k + ", " + m for k, m in zip(np.repeat(dictionary["z_class"]["old"], 2),
                                                                       ["mean", "sd"] * 2)])
            self.assertListEqual(list(res["variable"]["param"]["z_class"]["stat"].index), ["k: A", "k: B"])
            self.assertListEqual(list(res["mixture"]["IDClass"].index), ["k: A", "k: B"])
            self.assertListEqual(list(res["mixture"]["IDClassBar"].index), ["k: A", "k: B"])


if __name__ == "__main__":
    unittest.main()
