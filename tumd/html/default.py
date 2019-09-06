#!/usr/bin/python3

def print_expansion(args):
    tumd = args[0]
    tumd.print_expansion = not tumd.print_expansion
    return ''

default_dict = {
    'empty': u"\u0007",             # The 'bell' character hasn't been used in
                                    # twenty years and it wouldn't even work in
                                    # html anyway
    'zs' : '&#8203;',               # Zero-width space
    '{' : '<code>',
    '}' : '</code>',
    r'\{' : '{',
    r'\}' : '}',
    'ldb' : r'\{{{empty}}\{',
    'lt' : '&lt;',
    'gt' : '&gt;',
    'print_expansion' : print_expansion,
    'print' : '{{print_expansion (self)}}'
}
