from pathlib import Path
import pkgutil

from . import Script

class GuiUtil(Script):

    def __init__(self) -> None:
        super().__init__()
        self.guilist = {}

    def run(self, **kwargs):
        for module, name, ispkg in pkgutil.iter_modules():
            if name.startswith("displite_"):
                pkg_path = Path(module.path) / name
                if (pkg_path / "gui.h").exists() and (pkg_path / "gui.h").is_file():
                    self.guilist[name[9:]] = str(Path(module.path) / name)
        
        if "path" in kwargs:
            if kwargs["path"] in self.guilist:
                print(self.guilist[kwargs["path"]])
            return
        
        result = ""

        for x in self.guilist:
            result += x + ";"
        
        print(result)
