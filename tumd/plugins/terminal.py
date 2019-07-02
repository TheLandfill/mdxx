#!/usr/bin/python3
import re
from html.context import Context

terminal_variables = {
    'prompt':   '{{oneline}}{{user&comp}}:{{fulldir}}$',
    'user'  :   'joseph',
    'computer-name': 'tutorials',
    'dir': '~/dev',
    'oneline': '<span class="terminal-oneline">{{rest-of-line}}</span>{{end-of-line}}',
    'user&comp': '<span style="color:{{u&ccolor}};"><b>{{user}}@{{computer-name}}</b></span>',
    'fulldir': '<span style="color:{{dircolor}};"><b>{{dir}}</b></span>',
    'u&ccolor': '#E23434',
    'dircolor': '#DC1F63',
    '{': '{',
    '}': '}',
    r'\{' : r'\{',
    r'\}' : r'\}'
}

def open_terminal(html, args):
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
