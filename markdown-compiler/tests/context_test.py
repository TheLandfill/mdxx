#!/usr/bin/python3

from check_valid_context import check_valid_context
from terminal_plugin import terminal_variables

crash = {
    'crash' : '{{crash}}'
}

simple_cycle = {
    'a' : '{{b}}',
    'b' : '{{a}}'
}

more_complicated_cycle = {
    'a' : '{{b}}{{c}}',
    'b' : 'test',
    'c' : '{{d}}{{b}}',
    'd' : '{{a}}{{a}}'
}

complicated_no_cycle = {
    'a' : '{{b}}{{c}}',
    'b' : 'test',
    'c' : '{{e}}{{b}}',
    'd' : '{{a}}{{a}}',
    'e' : '{{b}}apple{{b}}apple',
    'f' : '{{a}}{{b}}{{c}}{{d}}{{e}}'
}

simple_no_cycle = {
    'a' : '{{b}}{{c}}',
    'b' : '{{c}}{{d}}',
    'c' : 'test',
    'd' : 'icles'
}

if __name__ == "__main__":
    print('crash: ' + str(check_valid_context(crash)))
    print('simple_cycle: ' + str(check_valid_context(simple_cycle)))
    print('more_complicated_cycle: ' + str(check_valid_context(more_complicated_cycle)))
    print('complicated_no_cycle: ' + str(check_valid_context(complicated_no_cycle)))
    print('simple_no_cycle: ' + str(check_valid_context(simple_no_cycle)))
    print('terminal_variables: ' + str(check_valid_context(terminal_variables)))
    terminal_variables['dir'] = '{{prompt}}'
    print('terminal_variables: ' + str(check_valid_context(terminal_variables)))
