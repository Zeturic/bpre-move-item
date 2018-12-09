import collections

class OrderedSet():
    def __init__(self, things=()):
        self._dict = collections.OrderedDict()
        for thing in things:
            self._dict[thing] = None

    def __iter__(self):
        return iter(self._dict)

    def add(self, thing):
        self._dict[thing] = None

    def remove(self, thing):
        self._dict.pop(thing)