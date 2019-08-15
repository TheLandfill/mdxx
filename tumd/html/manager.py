#!/usr/bin/python3
from html.default import default_dict

class HTML_Manager:

    def __init__(self, file_writer):
        self.tab_level = ""
        self.out = file_writer
        self.need_to_close_paragraph = False
        self.code_style = 'trac'

    def add(self, line):
        self.write(self.tab_level + line + "\n")

    def add_pre(self, line):
        self.write(line + "\n")

    def add_no_nl(self, line):
        self.write(self.tab_level + line)

    def write(self, line):
        line = self.remove_empty(line)
        self.out.write(line)

    def remove_empty(self, line):
        return line.replace(default_dict['empty'], '')

    def check_and_close_paragraph(self):
        if self.need_to_close_paragraph:
            pop([self])
            self.add('</p>')
            self.need_to_close_paragraph = False

def push(args):
    args[0].tab_level += "\t"
    return ''

def pop(args):
    args[0].tab_level = args[0].tab_level[:-1]
    return ''
