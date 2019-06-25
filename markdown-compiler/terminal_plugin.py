#!/usr/bin/python3
import re
from html_context import Context

terminal_last_line = ''

terminal_variables = {
    'prompt':   '{{oneline}}<span style="color:#E23434;"><b>{{user}}@{{computer-name}}</b></span>:<span style="color:#DC1F63;"><b>{{current-directory}}</b></span>$',
    'user'  :   'joseph',
    'computer-name': 'tutorials',
    'current-directory': '~/dev',
    'oneline': '<span class="terminal-oneline">{{rest-of-line}}</span>{{end-of-line}}'
}

def open_terminal(html):
    global terminal_last_line
    html.add_no_nl('<div class="terminal"><pre>')
    line = html.next_line_no_nl()
    while re.match(r'^\{\{.+\}\}:=\{\{.+\}\}$', line):
        varset = line.split("}}:={{")
        variable_to_set = varset[0][2:]
        value_to_set = varset[1][:-2]
        terminal_variables[variable_to_set] = value_to_set
        line = html.next_line_no_nl()
    terminal_last_line = line

def process_terminal(html, line_data):
    global terminal_last_line
    if terminal_last_line != '':
        html.add_pre(terminal_last_line)
    terminal_last_line = line_data[0]

def close_terminal(html):
    global terminal_last_line
    html.add_pre(terminal_last_line + '</pre>')
    html.add('</div>')

terminal_addon_dict = {
    'terminal': Context(open_terminal, process_terminal, close_terminal, terminal_variables)
}
