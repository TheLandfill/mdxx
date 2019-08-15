#!/usr/bin/python3
from html.context import Context
from html.paragraph import *
from html.manager import push, pop

def open_aside(html, args, tumd):
    html.add('<div class="aside" ' + args + '>')
    push([html])

def close_aside(html):
    html.check_and_close_paragraph()
    pop([html])
    html.add('</div>')

context_dict = {
    'aside':    Context(open_aside, paragraph, close_aside, paragraph_dict['paragraph'].variables)
}

print('Successfully imported aside.')
