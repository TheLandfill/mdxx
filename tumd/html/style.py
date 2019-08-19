#!/usr/bin/python3
from html.context import Context
from html.manager import push, pop

style_variables = {
    '{': '{',
    '}': '}',
    r'\{' : r'\{',
    r'\}' : r'\}',
}

used_style = False

def open_style(html, args, tumd):
    global used_style
    if used_style:
        print('ERROR: You can only use one style context.')
        raise SystemExit
    if len(tumd.context) > 2:
        print('ERROR: You can only use a style context at the beginning of the tumd file.')
        print(tumd.context)
        raise SystemExit
    html.add('<style>')
    used_style = True
    push([html])

def process_style(html, line_data):
    html.add(line_data[0])

def close_style(html):
    pop([html])
    html.add('</style>')

style_dict = {
    'style': Context(open_style, process_style, close_style, style_variables)
}

def get_style(args):
    article = args[0]
    article.tumd.find_next_content_line()
    if article.tumd.line_data[0] == r'\{{style}}':
        while article.tumd.line_data[0] != r'\{{\style}}':
            if article.tumd.line_data[0] == '':
                print('ERROR: style not closed')
            article.tumd.handle_context(article.html)
            article.tumd.find_next_content_line()
        article.tumd.handle_context(article.html)
    else:
        article.tumd.line_stack = '\n' + article.tumd.line_data[0]
    return ''
