#!/usr/bin/python3
import re
from html.manager import HTML_Manager
from tumd_manager import TUMD_Manager

class Article_Manager:
    
    def __init__(self, html_manager, tumd_manager):
        self.html = html_manager
        self.tumd = tumd_manager
        self.headings = []

    def write_article(args):
        args[0].tumd.context = [ 'default', 'paragraph' ]
        while (True):
            args[0].tumd.find_next_content_line()
            if args[0].tumd.line_data[0] == '':
                break
            args[0].expand_context()
        args[0].write_end_of_article()
        return ''

    def expand_context(self):
        number_of_headings = len(self.headings)
        self.handle_heading()
        if len(self.headings) > number_of_headings:
            return
        self.tumd.handle_context(self.html)

    def handle_heading(self):
        if self.tumd.cur_context_vars() != self.tumd.context_dict['paragraph'].variables:
            return
        line = self.tumd.line_data[0]
        count = 0
        while line[count] == '#' and count < 6:
            count += 1
        if count > 0:
            if self.html.need_to_close_paragraph:
                self.html.pop()
                self.html.add('</p>')
                self.html.need_to_close_paragraph = False
            line = self.tumd.expand_line(line)
            line = re.sub(r'^#*\s*', '', line)
            line = re.sub(r'\s*$', '', line)
            hstr = 'h' + str(count)
            if len(self.tumd.context) == 2:
                Article_Manager.heading_to_section([self, line, count])
                self.html.add('<' + hstr + ' id="' + self.headings[-1][0] + '">' + self.headings[-1][1] + '</' + hstr + '>')
            else:
                line = '<' + hstr + '>' + line + '</' + hstr + '>'
        self.tumd.line_data = (line, self.tumd.line_data[1])

    def heading_to_section(args):
        count = int(args[2])
        section = re.sub(r'^\s*', '', args[1])
        section = re.sub(r'\s*$', '', section)
        section = re.sub(r'\s+', '-', section)
        section = re.sub(r'<.*?>', '', section)
        section = re.sub(r'[^\w\-]', '', section)
        args[0].headings.append((section.lower(), args[1], int(args[2])))
        return section.lower()

    def write_sidenav(args):
        args[0].html.add('<nav id="sidenav">')
        args[0].html.push()
        for heading in args[0].headings:
            sidenav_level = ''
            for i in range(1, heading[2]):
                sidenav_level += '<div class="sidenav-level"></div>'
            args[0].html.add(sidenav_level + '<a href="#' + heading[0] + '" style="padding-left:' + str(12 * (heading[2] + 1)) + 'px;">' + heading[1] + '</a>')
        args[0].html.pop()
        args[0].html.add('</nav>')
        args[0].html.add('<div id="sidenav-activator"></div>')
        return ''

    def write_end_of_article(self):
        self.html.check_and_close_paragraph()
        self.html.pop()
        self.html.add('</div>')
