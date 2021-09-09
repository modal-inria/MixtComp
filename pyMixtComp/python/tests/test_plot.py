import unittest

import numpy as np
import pandas as pd

from pyMixtComp.MixtComp import MixtComp
from pyMixtComp.plot.barplot import plot_proportion, plot_discriminative_power_class, plot_discriminative_power_variable
from pyMixtComp.plot.univariate_boxplot import plot_data
from pyMixtComp.plot.univariate_distribution import plot_data_CI
from pyMixtComp.plot.heatmap import plot_variable_similarity, plot_class_similarity, plot_tik


class TestPlot(unittest.TestCase):
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
        self.z_class = np.repeat(["A", "B"], [70, 30])

        self.data = {"gauss": self.gauss, "mult": self.mult}

        self.mod = MixtComp(n_components=2)
        self.mod.fit(self.data, {"gauss": {"type": "Gaussian", "paramStr": ""},
                                 "mult": {"type": "Multinomial", "paramStr": ""}})

        self.mod_basic = MixtComp(n_components=2)
        self.mod_basic.fit(pd.DataFrame({"gauss": self.gauss, "mult": self.mult_basic, "z_class": self.z_class}))

    def test_plot_proportion(self):
        plot_proportion(self.mod.res_)
        plot_proportion(self.mod_basic.res_)

    def test_plot_discriminative_power_class(self):
        plot_discriminative_power_class(self.mod.res_)
        plot_discriminative_power_class(self.mod_basic.res_)

    def test_plot_discriminative_power_variable(self):
        plot_discriminative_power_variable(self.mod.res_, class_id=None)
        plot_discriminative_power_variable(self.mod_basic.res_, class_id=None)

        plot_discriminative_power_variable(self.mod.res_, class_id=0)
        plot_discriminative_power_variable(self.mod_basic.res_, class_id="A")

        plot_discriminative_power_variable(self.mod.res_, class_id=1)
        plot_discriminative_power_variable(self.mod_basic.res_, class_id="B")

    def test_plot_data_bad_var_name(self):
        with self.assertRaises(ValueError):
            plot_data(self.mod.res_, "gauss2", class_ids=None, all=False)

    def test_plot_data_numerical(self):
        plot_data(self.mod.res_, "gauss", class_ids=None, all=False)
        plot_data(self.mod_basic.res_, "gauss", class_ids=None, all=False)

        plot_data(self.mod.res_, "gauss", class_ids=None, all=True)
        plot_data(self.mod_basic.res_, "gauss", class_ids=None, all=True)

        plot_data(self.mod.res_, "gauss", class_ids=[1], all=False)
        plot_data(self.mod_basic.res_, "gauss", class_ids=["B"], all=False)

    def test_plot_data_categorical(self):
        plot_data(self.mod.res_, "mult", class_ids=None, all=False)
        plot_data(self.mod_basic.res_, "mult", class_ids=None, all=False)

        plot_data(self.mod.res_, "mult", class_ids=None, all=True)
        plot_data(self.mod_basic.res_, "mult", class_ids=None, all=True)

        plot_data(self.mod.res_, "mult", class_ids=[1], all=True)
        plot_data(self.mod_basic.res_, "mult", class_ids=["A"], all=True)

    def test_plot_data_CI_bad_var_name(self):
        with self.assertRaises(ValueError):
            plot_data_CI(self.mod.res_, "gauss2", class_ids=None, all=False)

    def test_plot_data_CI_numerical(self):
        plot_data_CI(self.mod.res_, "gauss", class_ids=None, all=False)
        plot_data_CI(self.mod_basic.res_, "gauss", class_ids=None, all=False)

        plot_data_CI(self.mod.res_, "gauss", class_ids=None, all=True)
        plot_data_CI(self.mod_basic.res_, "gauss", class_ids=None, all=True)

        plot_data_CI(self.mod.res_, "gauss", class_ids=[1], all=False)
        plot_data_CI(self.mod_basic.res_, "gauss", class_ids=["B"], all=False)

    def test_plot_data_CI_categorical(self):
        plot_data_CI(self.mod.res_, "mult", class_ids=None, all=False)
        plot_data_CI(self.mod_basic.res_, "mult", class_ids=None, all=False)

        plot_data_CI(self.mod.res_, "mult", class_ids=None, all=True)
        plot_data_CI(self.mod_basic.res_, "mult", class_ids=None, all=True)

        plot_data_CI(self.mod.res_, "mult", class_ids=[1], all=False)
        plot_data_CI(self.mod_basic.res_, "mult", class_ids=["B"], all=False)

    def test_plot_variable_similarity(self):
        plot_variable_similarity(self.mod.res_)
        plot_variable_similarity(self.mod_basic.res_)

    def test_plot_class_similarity(self):
        plot_class_similarity(self.mod.res_)
        plot_class_similarity(self.mod_basic.res_)

    def test_plot_tik(self):
        plot_tik(self.mod.res_)
        plot_tik(self.mod_basic.res_)

    def test_MixtComp_interface(self):
        self.mod.plot_class_similarity()
        self.mod.plot_variable_similarity()
        self.mod.plot_discriminative_power_class()
        self.mod.plot_discriminative_power_variable()
        self.mod.plot_tik()
        self.mod.plot_data("gauss")
        self.mod.plot_data_CI("gauss")
        self.mod.plot_proportion()

        self.mod_basic.plot_class_similarity()
        self.mod_basic.plot_variable_similarity()
        self.mod_basic.plot_discriminative_power_class()
        self.mod_basic.plot_discriminative_power_variable()
        self.mod_basic.plot_tik()
        self.mod_basic.plot_data("mult")
        self.mod_basic.plot_data_CI("mult")
        self.mod_basic.plot_proportion()


if __name__ == "__main__":
    unittest.main()
