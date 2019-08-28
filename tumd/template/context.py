#!/usr/bin/python3

from html.context import Context
from html.raw_html import process_html
from html.default import default_dict
from html.manager import pop, push

metadata_variables = {
    'title' : '{{missing}}TITLE',
    'author' : '{{missing}}AUTHOR',
    'date' : '{{missing}}DATE',
    'missing' : '<span style="color: #FF0000;">MISSING {{rest-of-line}}</span>{{end-of-line}}',
    'pop' : pop,
    'push' : push,
    'tupu' : '{{push (html)}}',
    'tupo' : '{{pop (html)}}'
}

metadata_variables = { **default_dict, **metadata_variables }

template_dict = {
    'template' : Context(None, process_html, None, metadata_variables)
}
