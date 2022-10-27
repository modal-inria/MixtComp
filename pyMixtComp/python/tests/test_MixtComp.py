import unittest
from multiprocessing import cpu_count

import numpy as np
import pandas as pd
from pyMixtComp.MixtComp import MixtComp
from sklearn.exceptions import NotFittedError


class TestMixtComp(unittest.TestCase):
    def setUp(self):
        rng = np.random.default_rng(42)

        self.gauss = np.concatenate((rng.normal(-2, 0.5, 70), rng.normal(2, 0.5, 30)), axis=None)
        self.mult = np.where(
                         np.concatenate((rng.multinomial(1, [0.25, 0.25, 0.25, 0.25], 70),
                                         rng.multinomial(1, [0.5, 0.1, 0.1, 0.3], 30))) == 1)[1]
        self.mult_basic = self.mult.copy().astype("str")
        self.mult_basic[self.mult_basic == "0"] = "a"
        self.mult_basic[self.mult_basic == "1"] = "b"
        self.mult_basic[self.mult_basic == "2"] = "c"
        self.mult_basic[self.mult_basic == "3"] = "d"

    def test_MixtComp_init(self):
        mod = MixtComp(n_components=2, n_burn_in_iter=10, n_iter=15, n_gibbs_burn_in_iter=20, n_gibbs_iter=25,
                       n_init_per_class=30, n_sem_try=35, confidence_level=0.9,
                       ratio_stable_criterion=0.95, n_stable_criterion=40, n_init=3, n_core=2)
        self.assertEqual(mod.n_components, 2)
        self.assertEqual(mod.n_init, 3)
        if cpu_count() > 1:
            self.assertEqual(mod.n_core, 2)
        else:
            self.assertEqual(mod.n_core, 1)
        self.assertEqual(mod.n_burn_in_iter, 10)
        self.assertEqual(mod.n_iter, 15)
        self.assertEqual(mod.n_gibbs_burn_in_iter, 20)
        self.assertEqual(mod.n_gibbs_iter, 25)
        self.assertEqual(mod.n_init_per_class, 30)
        self.assertEqual(mod.n_sem_try, 35)
        self.assertEqual(mod.confidence_level, 0.9)
        self.assertEqual(mod.ratio_stable_criterion, 0.95)
        self.assertEqual(mod.n_stable_criterion, 40)

    def test_MixtComp_init_bad(self):
        with self.assertRaises(ValueError):
            MixtComp(n_components=-1)
        with self.assertRaises(TypeError):
            MixtComp(n_components=["ss"])

        for param in ["n_burn_in_iter", "n_iter", "n_gibbs_burn_in_iter", "n_gibbs_iter", "n_init_per_class", "n_sem_try",
                      "n_stable_criterion", "n_init", "n_core"]:
            with self.assertRaises(ValueError):
                MixtComp(n_components=1, **{param: -5})
            with self.assertRaises(TypeError):
                MixtComp(n_components=1, **{param: "str"})

        for param in ["confidence_level", "ratio_stable_criterion"]:
            with self.assertRaises(ValueError):
                MixtComp(n_components=1, **{param: 1.5})
            with self.assertRaises(TypeError):
                MixtComp(n_components=1, **{param: "str"})

    def test_MixtComp_raise_error(self):
        mod = MixtComp(n_components=2)
        with self.assertRaises(RuntimeError):
            mod.fit({"gauss": self.gauss}, model={"gauss": {"type": "Multinomial", "paramStr": ""}})

    def test_MixtComp_fit_dict(self):
        mod = MixtComp(n_components=2)

        mod.fit({"gauss": self.gauss}, model={"gauss": {"type": "Gaussian", "paramStr": ""}})
        self.assertEqual(mod._basic_mode, False)
        self.assertIsInstance(mod.res_, dict)

        mod.predict({"gauss": self.gauss})
        self.assertIsInstance(mod.res_predict_, dict)

    def test_MixtComp_fit_dict_basic_mode(self):
        mod = MixtComp(n_components=2)

        mod.fit({"gauss": self.gauss})
        self.assertEqual(mod._basic_mode, True)
        self.assertDictEqual(mod.model_, {"gauss": {"type": "Gaussian", "paramStr": ""}})
        self.assertIsInstance(mod.res_, dict)

        mod.predict({"gauss": self.gauss})
        self.assertIsInstance(mod.res_predict_, dict)

    def test_MixtComp_fit_DataFrame(self):
        mod = MixtComp(n_components=2)

        mod.fit(pd.DataFrame(self.gauss, columns=["gauss"]), model={"gauss": {"type": "Gaussian", "paramStr": ""}})
        self.assertEqual(mod._basic_mode, False)
        self.assertIsInstance(mod.res_, dict)

        mod.predict(pd.DataFrame(self.gauss, columns=["gauss"]))
        self.assertIsInstance(mod.res_predict_, dict)

    def test_MixtComp_fit_DataFrame_basic_mode(self):
        mod = MixtComp(n_components=2)

        mod.fit(pd.DataFrame({"gauss": self.gauss, "mult": self.mult_basic}))
        self.assertEqual(mod._basic_mode, True)
        self.assertDictEqual(mod.model_, {"gauss": {"type": "Gaussian", "paramStr": ""},
                                          "mult": {"type": "Multinomial", "paramStr": ""}})
        self.assertIsInstance(mod.res_, dict)

        mod.predict(pd.DataFrame({"gauss": self.gauss, "mult": self.mult_basic}))
        self.assertIsInstance(mod.res_predict_, dict)

    def test_MixtComp_fit_array(self):
        mod = MixtComp(n_components=2)

        mod.fit(self.gauss.reshape(-1, 1))
        self.assertEqual(mod._basic_mode, True)
        self.assertDictEqual(mod.model_, {"var0": {"type": "Gaussian", "paramStr": ""}})
        self.assertIsInstance(mod.res_, dict)

        mod.predict(self.gauss.reshape(-1, 1))
        self.assertIsInstance(mod.res_predict_, dict)

    def test_MixtComp_fit_multi_init(self):
        mod = MixtComp(n_components=2, n_init_per_class=10, n_init=2)

        mod.fit({"gauss": self.gauss}, model={"gauss": {"type": "Gaussian", "paramStr": ""}})
        self.assertEqual(mod._basic_mode, False)
        self.assertIsInstance(mod.res_, dict)

        mod.predict({"gauss": self.gauss})
        self.assertIsInstance(mod.res_predict_, dict)

    def test_MixtComp_fit_multi_init_core(self):
        mod = MixtComp(n_components=2, n_init_per_class=10, n_init=1, n_core=5)
        self.assertEqual(mod.n_core, 1)

        mod = MixtComp(n_components=2, n_init_per_class=10, n_init=3, n_core=2)
        if cpu_count() > 1:
            self.assertEqual(mod.n_core, 2)
        else:
            self.assertEqual(mod.n_core, 1)

    def test_MixtComp_error_when_no_fit(self):
        mod = MixtComp(n_components=2)
        with self.assertRaises(NotFittedError):
            mod.predict({"gauss": [1, 2]})

    def test_MixtComp_crit(self):
        mod = MixtComp(n_components=2)
        with self.assertRaises(NotFittedError):
            mod.bic()
        with self.assertRaises(NotFittedError):
            mod.aic()
        with self.assertRaises(NotFittedError):
            mod.icl()

        mod.fit({"gauss": self.gauss}, model={"gauss": {"type": "Gaussian", "paramStr": ""}})

        bic = mod.bic()
        self.assertEqual(bic, mod.res_["mixture"]["BIC"])
        icl = mod.icl()
        self.assertEqual(icl, mod.res_["mixture"]["ICL"])
        aic = mod.aic()
        self.assertEqual(aic, mod.res_["mixture"]["lnObservedLikelihood"] - mod.res_["mixture"]["nbFreeParameters"])

        bic = mod.bic({"gauss": self.gauss})
        self.assertEqual(bic, mod.res_predict_["mixture"]["BIC"])
        icl = mod.icl({"gauss": self.gauss})
        self.assertEqual(icl, mod.res_predict_["mixture"]["ICL"])
        aic = mod.aic({"gauss": self.gauss})
        self.assertEqual(aic,
                         mod.res_predict_["mixture"]["lnObservedLikelihood"] - mod.res_predict_["mixture"]["nbFreeParameters"])

    def test_MixtComp_score(self):
        mod = MixtComp(n_components=2)
        with self.assertRaises(NotFittedError):
            mod.score()

        mod.fit({"gauss": self.gauss}, model={"gauss": {"type": "Gaussian", "paramStr": ""}})

        self.assertEqual(mod.score(), mod.res_["mixture"]["lnObservedLikelihood"])
        self.assertEqual(mod.score({"gauss": self.gauss}), mod.res_predict_["mixture"]["lnObservedLikelihood"])

    def test_MixtComp_sample(self):
        mod = MixtComp(n_components=2)

        mod.fit({"gauss": self.gauss}, model={"gauss": {"type": "Gaussian", "paramStr": ""}})

        x, z = mod.sample(10)

        self.assertIsInstance(x, pd.DataFrame)
        self.assertIsInstance(z, pd.Series)
        self.assertEqual(x.shape, (10, 1))
        self.assertEqual(z.shape, (10,))
        self.assertListEqual(x.columns.values.tolist(), ["gauss"])


if __name__ == "__main__":
    unittest.main()
