#!/usr/bin/python3
import re
from html_lists import html_list_dict
from html_paragraph import html_paragraph_dict

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
        self.context_dict = { **html_list_dict, **html_paragraph_dict }
        self.last_context = ''
        self.line_data = ('', 0)

    def next_line(self):
        return self.infile.readline()

    def next_line_no_nl(self):
        return re.sub('\n', '', self.next_line())

    def add(self, line):
        self.out.write(self.tab_level + line + "\n")

    def push(self):
        self.tab_level += "\t"

    def pop(self):
        self.tab_level = self.tab_level[:-1]

    def write_body(self):
        self.write_article()
        self.write_sidenav()

    def write_article(self):
        while (True):
            self.handle_pre_section()
            self.find_next_content_line()
            line = self.line_data[0]
            if line == '':
                break
            self.handle_context()
        self.write_end_of_article()

    def handle_context(self):
        another_pre = self.check_if_pre()
        if another_pre:
            return
        heading = self.handle_heading()
        if heading:
            return
        self.check_context()

    def handle_heading(self):
        line = self.line_data[0]
        count = 0
        while line[count] == '#' and count < 6:
            count += 1
        if count > 0:
            line = re.sub('^#*\s*', '', line)
            line = re.sub('\s*$', '', line)
            self.heading_to_section(line)
            hstr = 'h' + str(count) + '>'
            self.add('<' + hstr + '<section id="' + self.headings[-1][0] + '">' + self.headings[-1][1] + '</section></' + hstr)
        return count > 0

    def check_context(self):
        line = self.line_data[0]
        if re.match(r'^\\\{\{\\.*\}\}$', line):
            line = re.sub(r'[\\{}/]', '', line)
            context = self.context_dict[line]
            if line == self.context[-1]:
                print(self.context)
                context.close_context(self)
                print(self.context)
            else:
                print('ERROR: ' + line + ' was not opened but it was closed.')
                raise SystemExit
        elif re.match(r'^\\\{\{.*\}\}$', line):
            line = re.sub(r'[\\{}/]', '', line)
            if line in self.context_dict:
                self.context += [line]
                self.check_and_close_paragraph()
                self.context_dict[line].open_context(self)
            else:
                print('ERROR: ' + line + ' is not a specified context.')
                raise SystemExit
        else:
            self.context_dict[self.context[-1]].process_context(self, self.line_data)

#   def handle_paragraph(self, line_data):
#       line = self.line_data[0]
#       blank_lines = self.line_data[1] > 1
#       if self.context[-1] == 'paragraph':
#           if blank_lines:
#               self.check_and_close_paragraph()
#               self.add('<p>' + self.expand_line(line))
#               self.push()
#               self.need_to_close_paragraph = True
#           else:
#               self.add(self.expand_line(line))
#       else:
#           self.check_and_close_paragraph()

    def write_end_of_article(self):
        self.check_and_close_paragraph()
        self.pop()
        self.add('</div>')

    def check_if_pre(self):
        line = self.line_data[0]
        if line in pre_subs:
            check_and_close_paragraph()
            self.current_pre.append(line)
            self.context.append(line)
            self.add(pre_subs[line] + self.next_line_no_nl())
        return line in pre_subs

    def handle_pre_section(self):
        while len(self.current_pre) > 0:
            line = self.next_line()
            if line == '':
                print("ERROR: Prerendered section isn't closed.")
                print(self.current_pre)
                print(self.context)
                raise SystemExit
            if re.sub('[</>]', '', line) == self.current_pre[-1]:
                current_pre = current_pre[:-1]
            else:
                another_pre = self.check_if_pre()
                if not another_pre:
                    expanded_line = self.expand_line(line)
                    self.out.write(expanded_line)

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
        self.add('<div id="sidenav">')
        self.push()
        for heading in self.headings:
            self.add('<a href="#' + heading[0] + '">' + heading[1] + '</a>')
        self.pop()
        self.add('</div>')

    def expand_line(self, line):
        return line
