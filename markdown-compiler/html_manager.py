#!/usr/bin/python3
import re
from html_lists import html_list_dict
from html_paragraph import html_paragraph_dict
from terminal_plugin import terminal_addon_dict
from aside_plugin import aside_plugin_dict
from html_default import html_default_dict
from pprint import pprint

pre_subs = {}

class HTML_Manager:

    def __init__(self, file_writer, file_reader):
        self.tab_level = ""
        self.out = file_writer
        self.infile = file_reader
        self.headings = []
        self.current_pre = []
        self.context = ['default', 'paragraph']
        self.need_to_close_paragraph = False
        self.context_dict = { **html_list_dict, **html_paragraph_dict, **terminal_addon_dict, **aside_plugin_dict }
        for key in self.context_dict:
            for key2 in html_default_dict:
                self.context_dict[key].variables[key2] = html_default_dict[key2]
            print(key + ': ')
            pprint(self.context_dict[key].variables)
            print()
        self.last_context = ''
        self.line_data = ('', 0)

    def next_line(self):
        return self.infile.readline()

    def next_line_no_nl(self):
        return re.sub('\n', '', self.next_line())

    def add(self, line):
        self.out.write(self.tab_level + line + "\n")

    def add_pre(self, line):
        self.out.write(line + "\n")

    def add_no_nl(self, line):
        self.out.write(self.tab_level + line)

    def push(self):
        self.tab_level += "\t"

    def pop(self):
        self.tab_level = self.tab_level[:-1]

    def write_body(self):
        self.write_article()
        self.write_sidenav()

    def write_article(self):
        while (True):
            self.find_next_content_line()
            line = self.line_data[0]
            if line == '':
                break
            self.handle_context()
        self.write_end_of_article()

    def handle_context(self):
        number_of_headings = len(self.headings)
        self.handle_heading()
        if len(self.headings) > number_of_headings:
            return
        self.check_context()

    def handle_heading(self):
        if self.context_dict[self.context[-1]].variables != self.context_dict['paragraph'].variables:
            return
        line = self.line_data[0]
        count = 0
        while line[count] == '#' and count < 6:
            count += 1
        if count > 0:
            line = self.expand_line(line)
            line = re.sub(r'^#*\s*', '', line)
            line = re.sub(r'\s*$', '', line)
            hstr = 'h' + str(count)
            if len(self.context) == 2:
                self.heading_to_section(line)
                self.add('<' + hstr + ' id="' + self.headings[-1][0] + '">' + self.headings[-1][1] + '</' + hstr + '>')
            else:
                line = '<' + hstr + '>' + line + '</' + hstr + '>'
        self.line_data = (line, self.line_data[1])

    def check_context(self):
        line = self.line_data[0]
        if re.match(r'^\{\{.+\}\}:=\{\{.+\}\}$', line):
            varset = line.split("}}:={{")
            variable_to_set = varset[0][2:]
            value_to_set = varset[1][:-2]
            self.context_dict[self.context[-1]].variables[variable_to_set] = value_to_set
        elif re.match(r'^\\\{\{\\.*\}\}$', line):
            line = re.sub(r'[\\{}/]', '', line)
            context = self.context_dict[line]
            if line == self.context[-1]:
                context.close_context(self)
                self.context = self.context[:-1]
            else:
                print('ERROR: ' + line + ' was not opened but it was closed.')
                raise SystemExit
        elif re.match(r'^\\\{\{.*\}\}$', line):
            line = re.sub(r'[\\{}/]', '', line)
            args = ''
            if line.find(' ') != -1:
                args = line[line.find(' '):]
                line = line[:line.find(' ')]
            if line in self.context_dict:
                self.context += [line]
                self.check_and_close_paragraph()
                self.context_dict[line].open_context(self, args)
            else:
                print(line.find(' '))
                print(line.find(' ') != -1)
                print('line: ' + line)
                print('args: ' + args)
                print('ERROR: ' + line + ' is not a specified context.')
                raise SystemExit
        else:
            line_data = (self.expand_line(self.line_data[0]), self.line_data[1])
            self.context_dict[self.context[-1]].process_context(self, line_data)

    def write_end_of_article(self):
        self.check_and_close_paragraph()
        self.pop()
        self.add('</div>')

    def check_and_close_paragraph(self):
        if self.need_to_close_paragraph:
            self.pop()
            self.add('</p>')
            self.need_to_close_paragraph = False

    def find_next_content_line(self):
        line = '    '
        count = 0
        while re.match('^\s*$', line) and line != '':
            count += 1
            line = self.next_line()
        line = re.sub('\n', '', line)
        self.line_data = (line, count)

    def heading_without_section(self, heading):
        heading = re.sub('^\s*', '', heading)
        heading = re.sub('\s*$', '', heading)

    def heading_to_section(self, heading):
        section = re.sub('^\s*', '', heading)
        section = re.sub('\s*$', '', section)
        section = re.sub('\s+', '-', section)
        section = re.sub('[^\w\-]', '', section)
        self.headings.append((section.lower(), heading))

    def write_sidenav(self):
        self.add('<nav id="sidenav">')
        self.push()
        for heading in self.headings:
            self.add('<a href="#' + heading[0] + '">' + heading[1] + '</a>')
        self.pop()
        self.add('</nav>')

    def expand_line(self, line):
        context_variables = self.context_dict[self.context[-1]].variables
        current_sub = re.search(r'\{\{.+?\}\}', line)
        while current_sub:
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
                line = line.replace(current_sub.group(), context_variables[current_sub.group()[2:-2]])
            current_sub = re.search(r'\{\{.+?\}\}', line)
        line = line.replace('{', '<code>')
        line = line.replace('}', '</code>')
        line = line.replace(r'\{', '{')
        line = line.replace(r'\}', '}')
        return line
