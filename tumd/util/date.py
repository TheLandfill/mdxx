#!/usr/bin/python3
import datetime

def date(args):
    path = args[0]
    current_date = datetime.datetime.today().strftime('%-d %B %Y at %-I:%M %p')
    try:
        with open(path + '/date.txt', 'r') as reader:
            line = reader.readline()
            line = line.strip()
            return line
    except FileNotFoundError:
        with open(path + '/date.txt', 'w') as writer:
            writer.write(current_date)
    return current_date
