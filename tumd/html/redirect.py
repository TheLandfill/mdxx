#!/usr/bin/python3

def get_link(args):
    template = args[0]
    tumd = template.template
    metadata_tags = [(r'Redirect Link', True)]
    data = template.get_tags(metadata_tags)
    tumd.cur_context_vars()['redir'] = data[0]
    return ''
