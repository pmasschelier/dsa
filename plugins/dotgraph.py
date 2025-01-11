import gdb
import subprocess
import tempfile
import os

def dot_graph_list(graph: gdb.Value, tree: gdb.Value = None):
    """Generate dot representation for a graph in adjacency list format."""
    nb_vert = int(graph["nb_vert"])
    is_weighted = bool(graph["is_weighted"])

    # If tree provided, create a set of tree edges for quick lookup
    tree_edges = set()
    if tree is not None:
        for i in range(nb_vert):
            ancestor = int(tree[i])
            if ancestor != -1:
                tree_edges.add((ancestor, i))

    result = 'digraph {\n'
    result += '\tnode [style=filled, fillcolor=lightblue]\n'
    result += '\tedge [labelDistance=2.0]\n'

    # Create nodes
    for i in range(nb_vert):
        result += f'\tnode_{i} [label="{i}"]\n'
    
    # Create edges
    for i in range(nb_vert):
        # Get the list of neighbors for vertex i
        neighbor_list = graph["neighbours"][i]

        # Get the first element of the list
        current = neighbor_list["begin"]
        while current != 0:  # While not NULL
            # Cast the void pointer to graph_list_edge_t*
            edge = current["p"].cast(gdb.lookup_type("graph_list_edge_t").pointer()).dereference()
            to_vertex = int(edge["to"])

            # Check if this edge is part of the covering tree
            edge_attrs = []
            if (i, to_vertex) in tree_edges:
                edge_attrs.extend(['color="red"', 'penwidth=2.0'])
            
            if is_weighted:
                weight = int(edge["w"])
                edge_attrs.append(f'label="{weight}"')
            
            # Combine all edge attributes
            attrs_str = ', '.join(edge_attrs) if edge_attrs else ''
            if attrs_str:
                result += f'\tnode_{i} -> node_{to_vertex} [{attrs_str}]\n'
            else:
                result += f'\tnode_{i} -> node_{to_vertex}\n'

            current = current["next"]
    
    result += '}\n'
    return result

def dot_graph_mat(graph: gdb.Value, tree: gdb.Value = None):
    """Generate dot representation for a graph in adjacency matrix format."""
    nb_vert = int(graph["nb_vert"])
    edges = graph["edges"]
    weights = graph["weights"]

    # If tree provided, create a set of tree edges for quick lookup
    tree_edges = set()
    if tree is not None:
        for i in range(nb_vert):
            ancestor = int(tree[i])
            if ancestor != -1:
                tree_edges.add((ancestor, i))
    
    result = 'digraph {\n'
    result += '\tnode [style=filled, fillcolor=lightblue]\n'
    result += '\tedge [labelDistance=2.0]\n'
    
    # Create nodes
    for i in range(nb_vert):
        result += f'\tnode_{i} [label="{i}"]\n'
    
    # Create edges
    for i in range(nb_vert):
        for j in range(nb_vert):
            idx = i * nb_vert + j
            if bool(edges[idx]):
                edge_attrs = []
                
                # Check if this edge is part of the covering tree
                if (i, j) in tree_edges:
                    edge_attrs.extend(['color="red"', 'penwidth=2.0'])
                
                if weights != 0:  # If weighted graph
                    weight = int(weights[idx])
                    edge_attrs.append(f'label="{weight}"')
                
                # Combine all edge attributes
                attrs_str = ', '.join(edge_attrs) if edge_attrs else ''
                if attrs_str:
                    result += f'\tnode_{i} -> node_{j} [{attrs_str}]\n'
                else:
                    result += f'\tnode_{i} -> node_{j}\n'
    
    result += '}\n'
    return result

class DotGraph(gdb.Command):
    """Print graph structures using the graphviz tools.
    Usage: dot-graph <graph_variable>
    Example: dot-graph my_graph"""
    
    def __init__(self):
        super(DotGraph, self).__init__("dot-graph", gdb.COMMAND_DATA, gdb.COMPLETE_SYMBOL)
    
    def invoke(self, arg, from_tty):
        args = gdb.string_to_argv(arg)
        if not args:
            print("Missing graph variable argument")
            return
            
        try:
            # Parse the graph variable
            value = gdb.parse_and_eval(args[0])
            while value.type.code == gdb.TYPE_CODE_PTR:
                if value == 0:
                    print("Can't dereference a NULL pointer")
                    return
                value = value.dereference()

            # Parse the optional tree variable
            tree = None
            if len(args) > 1:
                tree = gdb.parse_and_eval(args[1])
                while tree.type.code == gdb.TYPE_CODE_PTR:
                    if tree == 0:
                        print("Can't dereference a NULL pointer for tree")
                        return
                    tree = tree.dereference()
            
            # Select the appropriate printer based on graph type
            if value.type.name == "graph_list_t":
                dot_content = dot_graph_list(value, tree)
            elif value.type.name == "graph_mat_t":
                dot_content = dot_graph_mat(value, tree)
            else:
                print(f"Unsupported graph type: {value.type.name}")
                return
            
            if dot_content is None:
                return

            # Create temporary files for dot and svg
            with tempfile.NamedTemporaryFile(mode='w', suffix='.dot', delete=False) as dot_file:
                dot_file.write(dot_content)
                dot_path = dot_file.name

            svg_path = dot_path[:-4] + '.svg'

            try:
                subprocess.run(['dot', '-Tsvg', dot_path, '-o', svg_path], check=True)
                if from_tty:
                    if os.name == 'posix':  # Linux/Unix
                        subprocess.run(['xdg-open', svg_path])
                    elif os.name == 'nt':    # Windows
                        os.startfile(svg_path)
                    else:                    # MacOS
                        subprocess.run(['open', svg_path])
            except subprocess.CalledProcessError:
                print("Error: Failed to generate SVG. Is Graphviz installed?")
            except Exception as e:
                print(f"Error opening SVG: {e}")
            finally:
                try:
                    os.unlink(dot_path)
                except:
                    pass

        except gdb.error as e:
            print(f"GDB error: {e}")
        except Exception as e:
            print(f"Error: {e}")

DotGraph()
