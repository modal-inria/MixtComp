import unittest

import numpy as np
import pandas as pd

from pyMixtComp.MixtComp import MixtComp


class TestMixtComp(unittest.TestCase):
    def setUp(self):
        self.gauss = np.concatenate((np.random.normal(-2, 0.5, 70), np.random.normal(2, 0.5, 30)), axis=None)

    def test_MixtComp_init(self):
        mod = MixtComp(n_components=2, n_burn_in_iter=10, n_iter=15, n_gibbs_burn_in_iter=20, n_gibbs_iter=25,
                       n_init_per_class=30, n_sem_try=35, confidence_level=0.9,
                       ratio_stable_criterion=0.95, n_stable_criterion=40, n_init=3, n_core=2)
        self.assertEqual(mod.n_components, 2)
        self.assertEqual(mod.n_init, 3)
        self.assertEqual(mod.n_core, 2)
        self.assertEqual(mod.n_burn_in_iter, 10)
        self.assertEqual(mod.n_iter, 15)
        self.assertEqual(mod.n_gibbs_burn_in_iter, 20)
        self.assertEqual(mod.n_gibbs_iter, 25)
        self.assertEqual(mod.n_init_per_class, 30)
        self.assertEqual(mod.n_sem_try, 35)
        self.assertEqual(mod.confidence_level, 0.9)
        self.assertEqual(mod.ratio_stable_criterion, 0.95)
        self.assertEqual(mod.n_stable_criterion, 40)

    def test_MixtComp_raise_error(self):
        mod = MixtComp(n_components=2)
        with self.assertRaises(RuntimeError):
            mod.fit({"gauss": self.gauss}, model={"gauss": {"type": "Multinomial", "paramStr": ""}})

    def test_MixtComp_fit_dict(self):
        mod = MixtComp(n_components=2)

        mod.fit({"gauss": self.gauss}, model={"gauss": {"type": "Gaussian", "paramStr": ""}})
        self.assertEqual(mod.basic_mode, False)

        mod.predict({"gauss": self.gauss})

    def test_MixtComp_fit_dict_basic_mode(self):
        mod = MixtComp(n_components=2)

        mod.fit({"gauss": self.gauss})
        self.assertEqual(mod.basic_mode, True)
        self.assertDictEqual(mod.model, {"gauss": {"type": "Gaussian", "paramStr": ""}})

        mod.predict({"gauss": self.gauss})

    def test_MixtComp_fit_DataFrame(self):
        mod = MixtComp(n_components=2)

        mod.fit(pd.DataFrame(self.gauss, columns=["gauss"]), model={"gauss": {"type": "Gaussian", "paramStr": ""}})
        self.assertEqual(mod.basic_mode, False)

        mod.predict(pd.DataFrame(self.gauss, columns=["gauss"]))

    def test_MixtComp_fit_DataFrame_basic_mode(self):
        mod = MixtComp(n_components=2)

        mod.fit(pd.DataFrame(self.gauss, columns=["gauss"]))
        self.assertEqual(mod.basic_mode, True)
        self.assertDictEqual(mod.model, {"gauss": {"type": "Gaussian", "paramStr": ""}})

        mod.predict(pd.DataFrame(self.gauss, columns=["gauss"]))

    def test_MixtComp_fit_array(self):
        mod = MixtComp(n_components=2)

        mod.fit(self.gauss.reshape(-1, 1))
        self.assertEqual(mod.basic_mode, True)
        self.assertDictEqual(mod.model, {"var0": {"type": "Gaussian", "paramStr": ""}})

        mod.predict(self.gauss.reshape(-1, 1))

    def test_MixtComp_fit_multi_init(self):
        mod = MixtComp(n_components=2, n_init_per_class=10, n_init=2)

        mod.fit({"gauss": self.gauss}, model={"gauss": {"type": "Gaussian", "paramStr": ""}})
        self.assertEqual(mod.basic_mode, False)

        mod.predict({"gauss": self.gauss})

    def test_MixtComp_fit_multi_init_core(self):
        mod = MixtComp(n_components=2, n_init_per_class=10, n_init=1, n_core=5)
        self.assertEqual(mod.n_core, 1)

        mod = MixtComp(n_components=2, n_init_per_class=10, n_init=3, n_core=2)
        self.assertEqual(mod.n_core, 2)


if __name__ == "__main__":
    unittest.main()
