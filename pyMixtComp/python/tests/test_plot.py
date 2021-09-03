import unittest

import numpy as np

from pyMixtComp.MixtComp import MixtComp
from pyMixtComp.plot.barplot import plot_proportion, plot_discriminative_power_class, plot_discriminative_power_variable
from pyMixtComp.plot.univariate_boxplot import plot_data
from pyMixtComp.plot.univariate_distribution import plot_data_CI
from pyMixtComp.plot.heatmap import plot_variable_similarity, plot_class_similarity, plot_tik


class TestPlot(unittest.TestCase):
    def setUp(self):
        self.data = {"gauss": np.concatenate((np.random.normal(-2, 0.5, 70), np.random.normal(2, 0.5, 30)), axis=None),
                     "mult": np.where(
                         np.concatenate((np.random.multinomial(1, [0.25, 0.25, 0.25, 0.25], 70),
                                         np.random.multinomial(1, [0.5, 0.1, 0.1, 0.3], 30))) == 1)[1]}

        self.mod = MixtComp(n_components=2)
        self.mod.fit(self.data, {"gauss": {"type": "Gaussian", "paramStr": ""},
                                 "mult": {"type": "Multinomial", "paramStr": ""}})

    def test_plot_proportion(self):
        plot_proportion(self.mod.res)

    def test_plot_discriminative_power_class(self):
        plot_discriminative_power_class(self.mod.res)

    def test_plot_discriminative_power_variable(self):
        plot_discriminative_power_variable(self.mod.res, class_number=None)

        plot_discriminative_power_variable(self.mod.res, class_number=0)

        plot_discriminative_power_variable(self.mod.res, class_number=1)

    def test_plot_data_bad_var_name(self):
        with self.assertRaises(ValueError):
            plot_data(self.mod.res, "gauss2", class_numbers=None, all=False)

    def test_plot_data_numerical(self):
        plot_data(self.mod.res, "gauss", class_numbers=None, all=False)

        plot_data(self.mod.res, "gauss", class_numbers=None, all=True)

        plot_data(self.mod.res, "gauss", class_numbers=[1], all=False)

    def test_plot_data_categorical(self):
        plot_data(self.mod.res, "mult", class_numbers=None, all=False)

        plot_data(self.mod.res, "mult", class_numbers=None, all=True)

        plot_data(self.mod.res, "mult", class_numbers=[1], all=True)

    def test_plot_data_CI_bad_var_name(self):
        with self.assertRaises(ValueError):
            plot_data_CI(self.mod.res, "gauss2", class_numbers=None, all=False)

    def test_plot_data_CI_numerical(self):
        plot_data_CI(self.mod.res, "gauss", class_numbers=None, all=False)

        plot_data_CI(self.mod.res, "gauss", class_numbers=None, all=True)

        plot_data_CI(self.mod.res, "gauss", class_numbers=[1], all=False)

    def test_plot_data_CI_categorical(self):
        plot_data_CI(self.mod.res, "mult", class_numbers=None, all=False)

        plot_data_CI(self.mod.res, "mult", class_numbers=None, all=True)

        plot_data_CI(self.mod.res, "mult", class_numbers=[1], all=False)

    def test_plot_variable_similarity(self):
        plot_variable_similarity(self.mod.res)

    def test_plot_class_similarity(self):
        plot_class_similarity(self.mod.res)

    def test_plot_tik(self):
        plot_tik(self.mod.res)


if __name__ == "__main__":
    unittest.main()
