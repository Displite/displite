#!/usr/bin/env python3
import argparse
import importlib
from pkgutil import iter_modules
import scripts
from scripts.console import Console, Color
import re

registered_scripts = {}
help_text = "scripts:\n"

def to_camel(string: str) -> str:
    return re.sub(r"(?:^|_)(.)", lambda m: m.group(1).upper(), string)

def load_scripts():
    global help_text
    for module in iter_modules(scripts.__path__):
        if module.ispkg:
            continue

        temp_module = importlib.import_module(f"scripts.{module.name}")
        temp_class = getattr(temp_module, to_camel(module.name), None)

        if temp_class is None or not isinstance(temp_class, type):
            continue

        if issubclass(temp_class, scripts.Script):
            registered_scripts[module.name] = temp_class
            help_text += f"  {module.name} : {temp_class.__doc__}\n"

def run_script(script_name: str, args=[]) -> None:
    if not script_name in registered_scripts:
        Console.pprint(f"{script_name} does not exit", Color.RED)
        return
    
    if not len(args):
        registered_scripts[script_name]().run()
        return
    
    split_args = {}
    for x in args:
        temp = x.split("=")
        if len(temp) != 2:
            Console.pprint(f"invalid arg defined. arg should be in key=value", Color.RED)
            return
        
        split_args[temp[0]] = temp[1]
    
    registered_scripts[script_name]().run(**split_args)


if __name__ == "__main__":
    load_scripts()
    parser = argparse.ArgumentParser(epilog=help_text, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("script", help="run specific utility", nargs="*")
    args = parser.parse_args()
    if len(args.script) < 2:
        run_script(args.script[0])
    else:
        run_script(args.script[0], args.script[1:])