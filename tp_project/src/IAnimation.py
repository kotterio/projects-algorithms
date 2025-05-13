from abc import ABC, abstractmethod

class IAnimation(ABC):
    @abstractmethod
    def __init__(self, ans):
        self.ans = ans
    
    @abstractmethod
    def draw(self):
        pass
