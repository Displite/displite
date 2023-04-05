from pathlib import Path
import pkgutil
from cookiecutter.main import cookiecutter
import importlib

from . import Script, get_project_dir

class GuiUtil(Script):

    def __init__(self) -> None:
        super().__init__()
        self.guilist = {}

    def run(self, **kwargs):
        if "create" in kwargs:
            self.create_gui_project(kwargs["create"])
            return

        for module, name, ispkg in pkgutil.iter_modules():
            if name.startswith("displite_"):
                temp_module = importlib.import_module(f"{name}")
                temp_type = getattr(temp_module, "TYPE", None)
                if(temp_type == "GUI"):
                    self.guilist[name[9:]] = str(Path(module.path) / name)
        
        if "path" in kwargs:
            if kwargs["path"] in self.guilist:
                print(self.guilist[kwargs["path"]])
            return
        
        result = ""

        for x in self.guilist:
            result += x + ";"
        
        print(result)
    
    def create_gui_project(self, name:str) -> None:
        cookiecutter("https://github.com/Displite/cookiecutter_displite_gui.git", output_dir=get_project_dir(), extra_context={"project_name": name})
