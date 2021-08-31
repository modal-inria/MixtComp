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
                       ratio_stable_criterion=0.95, n_stable_criterion=40)
        self.assertEqual(mod.n_components, 2)
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

    def test_MixtComp_fit_DataFrame(self):
        mod = MixtComp(n_components=2)

        mod.fit(pd.DataFrame(self.gauss, columns=["gauss"]), model={"gauss": {"type": "Gaussian", "paramStr": ""}})

    def test_MixtComp_fit_array(self):
        mod = MixtComp(n_components=2)

        mod.fit(self.gauss.reshape(-1, 1))
        self.assertDictEqual(mod.model, {"var0": {"type": "Gaussian", "paramStr": ""}})


if __name__ == "__main__":
    unittest.main()
