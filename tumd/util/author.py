#/usr/bin/python3
from urllib.request import urlopen

def get_author_image(args):
    name = args[0]
    return author_name(name) + '/large.jpg'

def write_author_description(args):
    meta = args[0]
    title = meta.template.cur_context_vars()['title']
    author = meta.template.cur_context_vars()['author']
    html = meta.html
    html.add('<div class="article">')
    html.push()
    html.add('<div id="author-description-local">')
    html.push()
    html.add('<img src="' + get_author_image([author]) + '" id="author-description-image" alt="A picture of ' + author + ', the author.">')
    html.add('<div id="author-description-text">')
    html.push()
    html.add_no_nl('<p>')
    html.push()
    print(author_name(author) + '/description.txt')
    data = urlopen(author_name(author) + '/description.txt')
    html.out.write(data.readline().decode("utf-8"))
    for line in data:
        html.add_no_nl(line.decode("utf-8"))
    html.pop()
    html.add('</p>')
    html.pop()
    html.add('</div>')
    html.pop()
    html.add('</div>')
    html.pop()
    html.add('</div>')
    return ''

def author_name(name):
    return 'https://tuacm.com/authors/' + '-'.join(name.split()).lower()
