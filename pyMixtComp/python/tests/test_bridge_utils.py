import unittest

from pyMixtComp.bridge.utils import create_algo, create_model, format_model


class TestUtils(unittest.TestCase):
    def setUp(self):
        self.n_component = 2
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
            "nClass": self.n_component,
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

    def test_create_model_model_names_None(self):
        model = create_model(None, ["gauss", "mult"])
        self.assertDictEqual(model, {"gauss": {"type": "Gaussian", "paramStr": ""},
                                     "mult": {"type": "Gaussian", "paramStr": ""}})

    def test_create_model_var_names_None(self):
        model = create_model(["Gaussian", "Multinomial"], None)
        self.assertDictEqual(model, {"var0": {"type": "Gaussian", "paramStr": ""},
                                     "var1": {"type": "Multinomial", "paramStr": ""}})

    def test_create_model(self):
        model = create_model(["Gaussian", "Multinomial"], ["gauss", "mult"])
        self.assertDictEqual(model, {"gauss": {"type": "Gaussian", "paramStr": ""},
                                     "mult": {"type": "Multinomial", "paramStr": ""}})


if __name__ == "__main__":
    unittest.main()
