import unittest

import numpy as np
import pandas as pd

from pyMixtComp.bridge.utils import create_algo, format_model, impute_model, _impute_model


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


if __name__ == "__main__":
    unittest.main()
