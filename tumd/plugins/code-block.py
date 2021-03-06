#!/usr/bin/python3
from html.context import Context

from pygments import highlight
from pygments.lexers import get_lexer_by_name
from pygments.formatters import HtmlFormatter

code_block_variables = {
    'ldb': '{{{empty}}{',
    '{': '{',
    '}': '}',
    r'\{' : r'\{',
    r'\}' : r'\}',
    'lt' : '<',
    'gt' : '>',
    'amp' : '&'
}

code_block = ''
code_language = ''
css_class = 'code-block'
line_number = -1
lines_to_highlight = []
add_line_numbers = True

def open_code_block(html, args, tumd):
    global code_language
    global add_line_numbers
    split_args = args.split()
    if len(split_args) < 1:
        print('ERROR: Need to specify the language.')
        tumd.print_line()
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

def code_block_substitutions(code):
    code = code.replace('<table class="code-blocktable">', '<table>')
    return code

def close_code_block(html):
    global code_block
    global code_language
    global lines_to_highlight
    global line_number
    global add_line_numbers
    global code_block_variables
    code_block = code_block.replace(code_block_variables['empty'], '')
    lexer = get_lexer_by_name(code_language, stripall=True)
    formatter = HtmlFormatter(linenos=add_line_numbers, style=html.code_style, hl_lines=lines_to_highlight, cssclass='code-block')
    if add_line_numbers:
        html.add('<div class="code-blocktable">')
    code = highlight(code_block, lexer, formatter)
    html.out.write(code_block_substitutions(code))
    html.add('')
    if add_line_numbers:
        html.add('</div>')
    code_block = ''
    lines_to_highlight = []
    line_number = -1;
    add_line_numbers = True;

context_dict = {
    'code-block': Context(open_code_block, process_code_block, close_code_block, code_block_variables)
}

print('Successfully imported code-block.')
