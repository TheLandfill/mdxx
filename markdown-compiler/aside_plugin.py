#!/usr/bin/python3
from html_context import Context
from html_paragraph import *

def open_aside(html, args):
    html.add('<div class="aside"' + args + '>')
    html.push()

def close_aside(html):
    html.pop()
    html.add('</div>')

aside_plugin_dict = {
    'aside':    Context(open_aside, html_paragraph, close_aside, html_paragraph_dict['paragraph'].variables)
}
