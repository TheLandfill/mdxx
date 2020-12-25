#!/usr/bin/python3
import re
from html.context import Context

terminal_variables = {
    'prompt':   '{{oneline}}{{user_and_comp}}:{{fulldir}}$',
    'mac-prompt': '{{oneline}}{{computer-name}}:{{mac-dir}} {{user}}$',
    'user'  :   'user',
    'computer-name': 'computer',
    'dir': '~/dev',
    'mac-dir': '~',
    'oneline': '<span class="terminal-oneline">{{rest-of-line}}</span>{{end-of-line}}',
    'user_and_comp': '<b><span style="color:{{u-and-c-color}};">{{user}}@{{computer-name}}</span></b>',
    'fulldir': '<b><span style="color:{{dir-color}};">{{dir}}</span></b>',
    'u-and-c-color': '#4CE64C',
    'dir-color': '#298FDD',
    '{': '{',
    '}': '}',
    r'\{' : r'\{',
    r'\}' : r'\}',
    'ldb' : '{{{empty}}{'
}

def open_terminal(html, args, tumd):
    if args[0:3] == "mac":
        html.add_no_nl('<div class="mac-terminal"' + args[3:] + '><pre>')
    else:
        html.add_no_nl('<div class="terminal"' + args + '><pre>')

def process_terminal(html, line_data):
    for i in range(1, line_data[1]):
        html.add_pre('')
    html.add_pre(line_data[0])

def close_terminal(html):
    html.add_pre('</pre>')
    html.add('</div>')

context_dict = {
    'terminal': Context(open_terminal, process_terminal, close_terminal, terminal_variables)
}

print('Successfully imported terminal.')
