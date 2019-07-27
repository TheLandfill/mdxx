#/usr/bin/python3

def get_author_image(args):
    name = args[0]
    return author_name(name) + '/large.jpg'

def get_author_description(args):
    name = args[0]
    return '<div class="article"><object id="author-description" data="' + author_name(name) + '/index.html" title="' + name + '\'s bio and picture."></object></div>'

def author_name(name):
    return 'https://tuacm.com/authors/' + '-'.join(name.split()).lower()
