import unittest

import numpy as np
import pandas as pd
from pyMixtComp.bridge.convert import convert, convert_c_matrix, convert_c_vector, convert_data_to_dict


class TestBridge(unittest.TestCase):
    def test_convert_C_matrix_convert_array(self):
        x = {"data": np.array([[1.]]), "rowNames": [], "colNames": [],
             "ctype": "Matrix", "dtype": "Real", "ncol": 1, "nrow": 1}

        out = convert_c_matrix(x)
        self.assertEqual(out, x["data"])

        x = {"data": np.array([[1.73245393e-06], [1.49653391e-07]]), "rowNames": [],
             "colNames": [], "ctype": "Matrix", "dtype": "Real", "ncol": 1, "nrow": 2}
        out = convert_c_matrix(x)
        self.assertListEqual(out.tolist(), x["data"].tolist())

        x = {"data": np.array([[-2.02395103, -2.02395103, -2.02395103],
                               [0.53140162,  0.53140162,  0.53140162],
                               [1.78087572,  1.78087572,  1.78087572],
                               [0.34934893,  0.34934893,  0.34934893]]),
             "rowNames": [],
             "colNames": [],
             "ctype": "Matrix", "dtype": "Real", "ncol": 3, "nrow": 4}
        out = convert_c_matrix(x)
        self.assertListEqual(out.tolist(), x["data"].tolist())

    def test_convert_C_matrix_convert_DataFrame(self):
        x = {"data": np.array([[1.]]), "rowNames": ["a"], "colNames": ["gauss"],
             "ctype": "Matrix", "dtype": "Real", "ncol": 1, "nrow": 1}

        df = pd.DataFrame(x["data"], columns=x["colNames"], index=x["rowNames"])
        out = convert_c_matrix(x)
        self.assertDictEqual(out.to_dict(), df.to_dict())

        x = {"data": np.array([[1.73245393e-06], [1.49653391e-07]]), "rowNames": ["k: 1", "k: 2"],
             "colNames": ["gauss"], "ctype": "Matrix", "dtype": "Real", "ncol": 1, "nrow": 2}
        df = pd.DataFrame(x["data"], columns=x["colNames"], index=x["rowNames"])

        out = convert_c_matrix(x)
        self.assertDictEqual(out.to_dict(), df.to_dict())

        x = {"data": np.array([[-2.02395103, -2.02395103, -2.02395103],
                               [0.53140162,  0.53140162,  0.53140162],
                               [1.78087572,  1.78087572,  1.78087572],
                               [0.34934893,  0.34934893,  0.34934893]]),
             "rowNames": ["k: 1, mean", "k: 1, sd", "k: 2, mean", "k: 2, sd"],
             "colNames": ["median", "q 2.500000%", "q 97.500000%"],
             "ctype": "Matrix", "dtype": "Real", "ncol": 3, "nrow": 4}
        df = pd.DataFrame(x["data"], columns=x["colNames"], index=x["rowNames"])
        out = convert_c_matrix(x)
        self.assertDictEqual(out.to_dict(), df.to_dict())

        x = {"data": np.array([[-2.02395103, -2.02395103, -2.02395103],
                               [0.53140162,  0.53140162,  0.53140162],
                               [1.78087572,  1.78087572,  1.78087572],
                               [0.34934893,  0.34934893,  0.34934893]]),
             "rowNames": [],
             "colNames": ["median", "q 2.500000%", "q 97.500000%"],
             "ctype": "Matrix", "dtype": "Real", "ncol": 3, "nrow": 4}
        df = pd.DataFrame(x["data"], columns=x["colNames"])
        out = convert_c_matrix(x)
        self.assertDictEqual(out.to_dict(), df.to_dict())

    def test_convert_C_vector_convert_array(self):
        x = {"data": np.array([-127.17314741, -127.17314741, -127.17314741]),
             "rowNames": [], "nrow": 3, "dtype": "Real", "ctype": "Vector"}
        out = convert_c_vector(x)
        self.assertListEqual(list(out), list(x["data"]))

    def test_convert_C_vector_convert_Series(self):
        x = {"data": np.array([-127.17314741, -127.17314741, -127.17314741]),
             "rowNames": ["a", "b", "c"], "nrow": 3, "dtype": "Real", "ctype": "Vector"}
        out = convert_c_vector(x)
        df = pd.Series(x["data"], index=x["rowNames"])
        self.assertEqual(out.to_dict(), df.to_dict())

    def test_convert(self):
        x = {}
        x["a"] = {"data": np.array([-127.17314741, -127.17314741, -127.17314741]),
                  "rowNames": ["a", "b", "c"], "nrow": 3, "dtype": "Real", "ctype": "Vector"}
        x["b"] = {"data": np.array([[-2.02395103, -2.02395103, -2.02395103],
                                    [0.53140162,  0.53140162,  0.53140162],
                                    [1.78087572,  1.78087572,  1.78087572],
                                    [0.34934893,  0.34934893,  0.34934893]]),
                  "rowNames": [],
                  "colNames": ["median", "q 2.500000%", "q 97.500000%"],
                  "ctype": "Matrix", "dtype": "Real", "ncol": 3, "nrow": 4}
        x["c"] = 2
        x["d"] = {"dd1": {"ddd": 7},
                  "dd2": {"data": np.array([-127.17314741, -127.17314741, -127.17314741]),
                          "rowNames": ["a", "b", "c"], "nrow": 3, "dtype": "Real", "ctype": "Vector"}}

        expected_out = {}
        expected_out["a"] = convert_c_vector(x["a"])
        expected_out["b"] = convert_c_matrix(x["b"])
        expected_out["c"] = 2
        expected_out["d"] = {"dd1": {"ddd": 7},
                             "dd2": convert_c_vector(x["d"]["dd2"])}

        convert(x)
        self.assertEqual(x.keys(), expected_out.keys())
        self.assertDictEqual(x["a"].to_dict(), expected_out["a"].to_dict())
        self.assertDictEqual(x["b"].to_dict(), expected_out["b"].to_dict())
        self.assertEqual(x["c"], expected_out["c"])
        self.assertEqual(x["d"].keys(), expected_out["d"].keys())
        self.assertEqual(x["d"]["dd1"], expected_out["d"]["dd1"])
        self.assertDictEqual(x["d"]["dd2"].to_dict(), expected_out["d"]["dd2"].to_dict())

    def test_convert_data_to_dict_with_array(self):
        x = np.array([[1, 2, 3], [4, 5, 6]])
        expected_out = {"var0": np.array(["1", "4"], dtype="<U21"),
                        "var1": np.array(["2", "5"], dtype="<U21"),
                        "var2": np.array(["3", "6"], dtype="<U21")}

        out = convert_data_to_dict(x)
        self.assertEqual(out.keys(), expected_out.keys())
        for key in expected_out.keys():
            self.assertListEqual(list(out[key]), list(expected_out[key]))

    def test_convert_data_to_dict_with_dict(self):
        x = {"a": [1, 4],
             "b": [2, 5],
             "c": [3, 6]}
        expected_out = {"a": ["1", "4"],
                        "b": ["2", "5"],
                        "c": ["3", "6"]}

        out = convert_data_to_dict(x)
        self.assertEqual(out.keys(), expected_out.keys())
        for key in expected_out.keys():
            self.assertListEqual(list(out[key]), list(expected_out[key]))

    def test_convert_data_to_dict_with_dataframe(self):
        x = pd.DataFrame(np.array([[1, 2, 3], [4, 5, 6]]), columns=["a", "b", "c"])
        expected_out = {"a": ["1", "4"],
                        "b": ["2", "5"],
                        "c": ["3", "6"]}

        out = convert_data_to_dict(x)
        self.assertEqual(out.keys(), expected_out.keys())
        for key in expected_out.keys():
            self.assertListEqual(list(out[key]), list(expected_out[key]))


if __name__ == "__main__":
    unittest.main()
