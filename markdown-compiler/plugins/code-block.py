#!/usr/bin/python3
from html.context import Context

from pygments import highlight
from pygments.lexers import get_lexer_by_name
from pygments.formatters import HtmlFormatter

code_block_variables = {
    '{': '{',
    '}': '}',
    r'\{' : r'\{',
    r'\}' : r'\}'
}

code_block = ''
code_language = ''
css_class = 'code-block'
line_number = 0
lines_to_highlight = []
add_line_numbers = True

def open_code_block(html, args):
    global code_language
    global add_line_numbers
    split_args = args.split()
    if len(split_args) < 1:
        print('ERROR: Need to specify the language.')
        html.print_line()
        raise SystemExit
    code_language = split_args[0]
    if len(split_args) > 1:
        add_line_numbers = (split_args[1] != 'no-line-numbers')

def process_code_block(html, line_data):
    global line_number
    global code_block
    global lines_to_highlight
    line_number += line_data[1]
    skip_hl = 0
    if line_data[0][0:2] == 'hl':
        skip_hl = 2
        lines_to_highlight.append(line_number)
    for i in range(1, line_data[1]):
        code_block += '\n'
    code_block += line_data[0][skip_hl:] + '\n'

def close_code_block(html):
    global code_block
    global code_language
    global lines_to_highlight
    global line_number
    global add_line_numbers
    lexer = get_lexer_by_name(code_language, stripall=True)
    formatter = HtmlFormatter(linenos=add_line_numbers, style=html.code_style, hl_lines=lines_to_highlight, cssclass='code-block')
    highlight(code_block, lexer, formatter, outfile=html.out)
    html.add('')
    print(lines_to_highlight)
    print(line_number)
    code_block = ''
    lines_to_highlight = []
    line_number = 0;
    add_line_numbers = True;

context_dict = {
    'code-block': Context(open_code_block, process_code_block, close_code_block, code_block_variables)
}

print('Successfully imported code-block.')
