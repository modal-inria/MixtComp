# flake8: noqa
from .MixtComp import MixtComp
from . import data
from . import utils
from . import plot
from pkg_resources import get_distribution

__version__ = get_distribution("pyMixtComp").version
