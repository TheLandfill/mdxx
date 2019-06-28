#!/usr/bin/python3
import re
from pprint import pprint

def check_valid_context(context_dict):
    graph = generate_graph(context_dict)
    return no_cycles(graph)

def generate_graph(context_dict):
    con_dict = { **context_dict, **{ 'rest-of-line': '', 'end-of-line': ''}}
    graph = {}
    var_re = r'\{\{.+?\}\}'
    for key in con_dict:
        expansion = con_dict[key]
        cur_sub = re.search(var_re, expansion)
        graph[key] = []
        while cur_sub:
            graph[key].append(cur_sub.group()[2:-2])
            expansion = expansion.replace(cur_sub.group(), '')
            cur_sub = re.search(var_re, expansion)
    return graph

def no_cycles(graph):
    for node in graph:
        visited = {}
        for key in graph:
            visited[key] = False
        stack = [ node ]
        visited[node] = True
        nodes_to_unvisit = []
        while len(stack) > 0:
            adj = graph[stack[-1]]
            if node in adj:
                return stack
            next_node = get_first_unvisited(adj, visited)
            if next_node == None:
                stack = stack[:-1]
            else:
                stack.append(next_node)
                visited[next_node] = True
    return []

def get_first_unvisited(adj, visited):
    for node in adj:
        if visited[node] is False:
            return node
    return None
