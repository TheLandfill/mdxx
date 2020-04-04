#!/usr/bin/python3
from html.context import Context
from html.paragraph import *
from html.manager import push, pop

spoiler_variables = {}

def open_spoiler(html, args, tumd):
    html.add('<div class="spoiler">')
    push([html])
    html.add('<div class="content">')

def close_spoiler(html):
    html.check_and_close_paragraph()
    html.add('</div>')
    pop([html])
    html.add('</div>')

context_dict = {
    'spoiler': Context(open_spoiler, paragraph, close_spoiler, spoiler_variables)
}

print('Successfully imported spoiler.')
