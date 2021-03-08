#!/usr/bin/python3
from pygments import highlight
from pygments.lexers import get_lexer_by_name
from pygments.formatters import HtmlFormatter

def hand_code_to_pygments(code_block, code_language, add_line_numbers, code_style, lines_to_highlight):
    output = ""
    lexer = get_lexer_by_name(code_language, stripall=True)
    formatter = HtmlFormatter(linenos=add_line_numbers, style=code_style, hl_lines=lines_to_highlight, cssclass='code-block')
    if add_line_numbers:
        output += '<div class="code-blocktable">'
    code = highlight(code_block, lexer, formatter)
    output += code.replace('<table class="code-blocktable">', '<table>')
    return output
