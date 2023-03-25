from enum import Enum

class Color(Enum):
    RED = ";31"
    GREEN = ";32"
    YELLOW = ";33"
    BLUE = ";34"

class Console:
    
    @classmethod
    def pprint(cls, message: str, color: Color = None, **kwargs) -> None:
        if color is None:
            print(message, **kwargs)
        else:
            print(f"\033[{color.value}m{message}\033[0m", **kwargs)
    
    @classmethod
    def prompt(cls, message:str) -> bool:
        while True:
            cls.pprint(message + "(Yes or No)", Color.YELLOW, end=" : ")
            result = input()
            if result.lower() == "yes":
                return True
            elif result.lower() == "no":
                return False
            else:
                cls.pprint("invalid option.", Color.RED)
    
    @classmethod
    def get_input(cls, message:str) -> str:
        cls.pprint(message, Color.YELLOW, end=" : ")
        return input()
