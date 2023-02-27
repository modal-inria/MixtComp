# flake8: noqa
import importlib.metadata

from . import data, plot, utils
from .MixtComp import MixtComp

try:
    __version__ = importlib.metadata.version("pyMixtComp")
except importlib.metadata.PackageNotFoundError:
    __version__ = "Not installed"
