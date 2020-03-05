#!/usr/bin/python3
import re
from html.context import Context

terminal_variables = {
    'prompt':   '{{oneline}}{{user_and_comp}}:{{fulldir}}$',
    'mac-prompt': '{{oneline}}{{computer-name}}:{{mac-dir}} {{user}}$',
    'user'  :   'joseph',
    'computer-name': 'tutorials',
    'dir': '~/dev',
    'mac-dir': '~',
    'oneline': '<span class="terminal-oneline">{{rest-of-line}}</span>{{end-of-line}}',
    'user_and_comp': '<span style="color:{{u-and-c-color}};"><b>{{user}}@{{computer-name}}</b></span>',
    'fulldir': '<span style="color:{{dir-color}};"><b>{{dir}}</b></span>',
    'u-and-c-color': '#3734E2',
    'dir-color': '#FF0070',
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
