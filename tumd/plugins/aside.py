#!/usr/bin/python3
from html.context import Context
from html.paragraph import *

def open_aside(html, args):
    html.add('<div class="aside" ' + args + '>')
    html.push()

def close_aside(html):
    html.check_and_close_paragraph()
    html.pop()
    html.add('</div>')

context_dict = {
    'aside':    Context(open_aside, paragraph, close_aside, paragraph_dict['paragraph'].variables)
}

print('Successfully imported aside.')
