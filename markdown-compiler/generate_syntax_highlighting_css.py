#!/usr/bin/python3
from pygments import highlight
from pygments.lexers import get_lexer_by_name
from pygments.formatters import HtmlFormatter
from pygments.styles import get_all_styles

if __name__ == "__main__":
    style_list = list(get_all_styles())
    for s in style_list:
        with open("../css/" + s + "-code.css", "w") as f:

            f.write(HtmlFormatter(cssclass='code-block', style=s).get_style_defs())
