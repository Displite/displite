from . import Script, get_project_dir
from .console import Console, Color
from fabric import Connection
import subprocess

class FlashElf(Script):
    """Create helper script to flash elf using openocd tool on local or remote device"""
    
    def run(self, **kwargs):

        cmake_build_dir = get_project_dir() / "build"
        elf_files = list(cmake_build_dir.glob("*.elf"))

        if not len(elf_files):
            Console.pprint("no elf file found in cmake build directory", Color.RED)
            return
        
        self.elf_file_name = elf_files[0].name
        self.elf_path = str(elf_files[0].absolute())

        if "local" in kwargs:
            self.run_local()
            return

        if "remote" in kwargs:
            self.run_remote(kwargs["remote"])
            return

        should_be_remote = Console.prompt("elf flash by remote device?")

        if should_be_remote:
            ssh_connection = Console.get_input("enter remote device config name defined in ssh config file")
            self.run_remote(ssh_connection)
    
    def run_local(self) -> None:
        Console.pprint(f"flashing elf via openocd", Color.GREEN)
        status = subprocess.run(f'openocd -f interface/raspberrypi-swd.cfg -f target/rp2040.cfg -c "program {self.elf_path} verify reset exit"', cwd=get_project_dir(), shell=True)

        if status.returncode:
            Console.pprint("elf could not be flashed", Color.RED)
        else:
            Console.pprint(f"flashing complete", Color.GREEN)

    def run_remote(self, ssh_config_name: str) -> None:
        with Connection(ssh_config_name) as c:
                Console.pprint(f"Uploading {self.elf_path} to /home/{c.user}/", Color.GREEN)
                result = c.put(self.elf_path, remote=f"/home/{c.user}/")
                Console.pprint(f"Uploaded {result.local} to {result.remote}", Color.GREEN)
                result = c.run(f'openocd -f interface/raspberrypi-swd.cfg -f target/rp2040.cfg -c "program /home/{c.user}/{self.elf_file_name} verify reset exit"')
                if result.exited or None:
                    Console.pprint("elf could not be flashed", Color.RED)
                else:
                    Console.pprint(f"flashing complete", Color.GREEN)
