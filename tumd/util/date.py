#!/usr/bin/python3
import datetime

def date(args):
    return datetime.datetime.today().strftime('%-d %B %Y at %-I:%M %p')
