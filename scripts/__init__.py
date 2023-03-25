__all__ = ["Script"]

import pathlib
from abc import ABC, abstractmethod

__PACKAGE_DIR = pathlib.Path(__file__).parent
__PROJECT_DIR =  __PACKAGE_DIR.parent

def get_pkg_dir() -> pathlib.Path:
    """Scripts Directory"""
    return __PACKAGE_DIR

def get_project_dir() -> pathlib.Path:
    """Project Directory"""
    return __PROJECT_DIR

class Script(ABC):
    """Base class for creating a script"""
    @abstractmethod
    def run(self, **kwargs):
        pass
