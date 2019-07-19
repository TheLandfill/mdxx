#!/usr/bin/python3

from html.context import Context
from html.raw_html import process_html
from html.default import default_dict

metadata_variables = {
    'title' : '{{missing}}TITLE',
    'author' : '{{missing}}AUTHOR',
    'date' : '{{missing}}DATE',
    'missing' : '<span style="color: #FF0000;">MISSING {{rest-of-line}}</span>{{end-of-line}}'
}

metadata_variables = { **default_dict, **metadata_variables }

non_article_dict = {
    'non-article' : Context(None, process_html, None, metadata_variables)
}
