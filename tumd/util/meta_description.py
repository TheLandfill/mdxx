#!/usr/bin/env python3
import os

def get_meta_description(args):
    path = args[0]
    filename = path + '/meta-description.txt'
    output = []
    try:
        with open(filename, 'r') as reader:
            line = reader.readline()
            output.append(line.strip())
            while line:
                line = reader.readline()
                output.append(line.strip())
    except FileNotFoundError:
        print("WARNING: No meta-description.txt found.")
    return ' '.join((' '.join(output).strip()).split())
