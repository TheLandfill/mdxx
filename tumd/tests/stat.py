#!/usr/bin/env python3
from os.path import getmtime
import sys

if __name__ == "__main__":
    args = sys.argv[1:]
    for arg in args:
        print(arg + ": " + str(getmtime(arg)))
