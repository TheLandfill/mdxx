#!/usr/bin/python3
from html.lists import list_dict
from html.paragraph import paragraph_dict
from html.tumd_comment import comment_dict
from html.default import default_dict
from html.raw_html import raw_html_dict
from html.style import style_dict
from util.check_valid_context import check_valid_context, generate_graph
from pprint import pprint
import re
import importlib

class TUMD_Manager:

    def __init__(self, file_reader):
        self.infile = file_reader
        self.context = ['default']
        self.context_dict = { **list_dict, **paragraph_dict, **comment_dict, **raw_html_dict, **style_dict }
        self.add_default_variables()
        self.line_number = 0
        self.line_data = ('', 0)
        self.line_stack = ''

    def add_default_variables(self):
        for key in self.context_dict:
            for key2 in default_dict:
                if key2 not in self.context_dict[key].variables:
                    self.context_dict[key].variables[key2] = default_dict[key2]
        for key in self.context_dict:
            self.check_variable_dependency(key)

    def next_line(self):
        self.line_number += 1
        return self.infile.readline()

    def next_line_no_nl(self):
        return re.sub('\n$', '', self.next_line())

    def cur_context_vars(self):
        return self.cur_context().variables

    def cur_context(self):
        return self.context_dict[self.context[-1]]

    def print_line(self):
        print('\tLine ' + str(self.line_number).zfill(3) + ':\t' + self.line_data[0])

    def handle_context(self, html):
        line = self.line_data[0]
        if re.match(r'^\{\{.+\}\}:=".+"$', line):
            self.variable_definition(line)
        elif re.match(r'^\{\{.+?\}\}:=\{\{from\s+.+\s+import\s+.+\}\}', line):
            self.python_import(line)
        elif re.match(r'^\\\{\{\\.*\}\}$', line):
            self.close_context(line, html)
        elif re.match(r'^\\\{\{.+\}\}$', line):
            self.open_context(line, html)
        else:
            self.line_data = (self.expand_line(self.line_data[0]), self.line_data[1])
            self.cur_context().process_context(html, self.line_data)

    def variable_definition(self, line):
        varset = line.split('}}:="')
        variable_to_set = varset[0][2:]
        value_to_set = varset[1][:-1]
        self.cur_context_vars()[variable_to_set] = value_to_set
        self.check_variable_dependency(self.context[-1])

    def python_import(self, line):
        varset = line.split('}}:={{from')
        variable_to_set = varset[0][2:]
        module_py_object = varset[1].split('import', 1)
        module = re.sub('\s', '', module_py_object[0])
        py_object = re.sub('\s', '', module_py_object[1][:-2])
        py_object = py_object.split('.')
        module_dict = importlib.import_module(module).__dict__
        cur_object = module_dict[py_object[0]]
        py_object = py_object[1:]
        for obj in py_object:
            cur_object = getattr(cur_object, obj)
        self.cur_context_vars()[variable_to_set] = cur_object

    def close_context(self, line, html):
        line = re.sub(r'[\\{}/]', '', line)
        context = self.context_dict[line]
        if line == self.context[-1]:
            context.close_context(html)
            self.context = self.context[:-1]
        else:
            print('ERROR: ' + line + ' was not opened but it was closed.')
            self.print_line()
            raise SystemExit

    def open_context(self, line, html):
        line = re.sub(r'[\\{}/]', '', line)
        args = ''
        if line.find(' ') != -1:
            args = line[line.find(' ') + 1:]
            line = line[:line.find(' ')]
        if line in self.context_dict:
            self.context += [line]
            html.check_and_close_paragraph()
            self.context_dict[line].open_context(html, args, self)
        else:
            print('ERROR: ' + line + ' is not a specified context.')
            pprint(self.context_dict)
            self.print_line()
            raise SystemExit

    def find_next_content_line(self):
        line = '    '
        count = 0
        if self.line_stack != '':
            count = len(self.line_stack) - len(self.line_stack.lstrip('\n')) + 1
            self.line_stack = self.line_stack.lstrip('\n')
            line_split = self.line_stack.split('\n', 1)
            line = line_split[0]
            if len(line_split) == 1:
                self.line_stack = ''
            else:
                self.line_stack = line_split[1]
            self.line_data = (line, count)
            return
        while re.match('^\s*$', line) and line != '':
            count += 1
            line = self.next_line()
        line = re.sub('\n$', '', line)
        self.line_data = (line, count)

    def expand_line(self, line):
        context_variables = self.cur_context_vars()
        current_sub = re.search(r'\{\{[^{]+?\}\}', line)
        while current_sub:
            # Could make {{rest-of-line}} handling its own function
            if (current_sub.group() == '{{rest-of-line}}'):
                rest_of_line = line[current_sub.span()[1]:]
                text_to_end = re.search(r'\{\{end-of-line\}\}', line)
                if text_to_end is None:
                    print("Missing corresponding {{end-of-line}} in line: " + line)
                    raise SystemExit
                end = line[current_sub.span()[1]:text_to_end.span()[0]]
                middle = line[text_to_end.span()[1]:]
                begin = line[:current_sub.span()[0]]
                line = begin + middle + end
            else:
                # This next line needs to only replace outer parentheses
                var_args = current_sub.group()[2:-2].replace('(', '{{').replace(')', '}}')
                var_args = var_args.split()
                var = var_args[0]
                args = var_args[1:]
                for i, arg in enumerate(args):
                    args[i] = self.expand_line(arg)
                var = context_variables[var]
                if isinstance(var, str):
                    line = line.replace(current_sub.group(), var)
                elif callable(var):
                    line = line.replace(current_sub.group(), var(args))
                else:
                    line = var
                    return line
            current_sub = re.search(r'\{\{[^{]+?\}\}', line)
        if '\n' in line:
            line_split = line.split('\n', 1)
            line = line_split[0]
            self.line_stack = line_split[1] + self.line_stack
        line = re.sub(r'(?<!\\){', context_variables['{'], line)
        line = re.sub(r'(?<!\\)}', context_variables['}'], line)
        line = line.replace(r'\{', context_variables['\{'])
        line = line.replace(r'\}', context_variables['\}'])
        return line

    def check_variable_dependency(self, context):
        circular_reference = check_valid_context(self.context_dict[context].variables)
        if len(circular_reference) > 0:
            print('\n\033[48;5;196m\033[38;5;15mERROR: circular variable dependency\033[m\n')
            dependency_cycle = ''
            ul_col = '\033[4m'
            dep_col = '\033[38;5;14m'
            cont_col = '\033[38;5;33m'
            for dep in circular_reference:
                dependency_cycle += ul_col + dep_col + dep + '\033[m depends on '
            dependency_cycle += ul_col + dep_col + circular_reference[0]
            dependency_cycle += '\033[m in context '
            dependency_cycle += ul_col + cont_col + context + '\033[m'
            self.print_line()
            print('\n' + ul_col + cont_col + context + ' dependencies\033[m')
            pprint(generate_graph(self.context_dict[context].variables))
            print('\n' + ul_col + cont_col + context + '\033[m')
            pprint(self.context_dict[context].variables)
            print('Circular Dependency:\t' + dependency_cycle)
            raise SystemExit
