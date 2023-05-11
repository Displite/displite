from . import Script, get_project_dir
import subprocess
from .console import Console, Color

class Init(Script):
    """Configure/Init project"""

    def run(self, **kwargs):
        result = subprocess.run("git submodule update --init", cwd=str(get_project_dir()), shell=True)
        if result.returncode:
            Console.pprint("Could not init project", Color.RED)
            return
        
        result = subprocess.run("git submodule update --init -- ./lib/tinyusb", cwd=str(get_project_dir()/"clib"/"pico-sdk"), shell=True)

        if result.returncode:
            Console.pprint("Could not init tinyusb submodule", Color.RED)
            return