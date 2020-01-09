#!/usr/bin/env python3
from pprint import pprint

def print_expansion(args):
    tumd = args[0]
    tumd.print_expansion = not tumd.print_expansion
    return ''

def print_variables(args):
    tumd = args[0]
    pprint(tumd.cur_context_vars())
    return ''

def print_context(args):
    tumd = args[0]
    pprint(tumd.context)
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
    'nl' : '\n',
    'print-expansion' : print_expansion,
    'print' : '{{print-expansion (self)}}',
    'print-variables' : print_variables,
    'print-vars' : '{{print-variables (self)}}',
    'print-context' : print_context,
    'print-con' : '{{print-context (self)}}'
}
