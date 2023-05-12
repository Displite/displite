from . import Script, get_project_dir, get_pkg_dir
from cookiecutter.main import cookiecutter

class CreateGui(Script):

    def run(self, **kwargs):
        cookiecutter(str(get_pkg_dir() / "gui_template"), output_dir=(get_project_dir()/ "gui"))