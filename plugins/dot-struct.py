import gdb
import subprocess
import tempfile
import os
import argparse

def dot_linked_list(list: gdb.Value, data_type: gdb.Type) -> str | None:
    begin = list["begin"]
    if begin == 0:
        print("Empty list.")
        return

    fillcolor = "lightblue"
    result = 'digraph {\n'
    result += '\trankdir=LR\n'
    result += '\tgraph [ordering=out]\n'
    result += '\tnode [style=filled]\n'
    values = []

    result += '\tsentinel_begin [label="NULL", fillcolor="gray", shape=point]\n'
    result += '\tsentinel_end [label="NULL", fillcolor="gray", shape=point]\n'
    count = 0

    node = begin
    while node != 0:
        try:
            value = node.dereference()["data"].dereference().cast(data_type)
            if data_type.code == gdb.TYPE_CODE_PTR:
                if data_type.target() == gdb.lookup_type('char'):
                    try:
                        value = value.string()
                    except gdb.error:
                        value = str(value)
                else:
                    value = str(value)
            elif data_type.code == gdb.TYPE_CODE_STRUCT:
                value = f"struct@{value.address}"
            else:
                value = value.format_string(format='d')
            result += f'\tnode_{node} [label={value}, fillcolor={fillcolor}]\n'
        except gdb.error:
            value = '?'
            result += f'\tnode_{node} [label="?", fillcolor=lightgray]\n'

        next = node.dereference()["next"]
        if next != 0:
            result += f'\tnode_{node} -> node_{next} \n'
        else:
            result += f'\tnode_{node} -> sentinel_end [style=dashed]\n'
        prev = node.dereference()["prev"]
        if prev != 0:
            result += f'\tnode_{node} -> node_{prev} [style=dashed] \n'
        else:
            result += f'\tsentinel_begin -> node_{node} [style=dashed,dir=back]\n'
        values.append(value)
        node = next
        count += 1
    result += '}\n'
    print(values)
    return result

def dot_indexed_list(list: gdb.Value, data_type: gdb.Type):
    size = list["size"]
    if list["begin"] < 0:
        print("Empty list.")
        if size != 0:
            print(f"Warning The list size is {list["size"]}")
        return

    fillcolor = "lightblue"
    data = list["data"].cast(data_type.pointer())
    next = list["next"].cast(gdb.lookup_type('int').pointer())
    prev = list["prev"].cast(gdb.lookup_type('int').pointer())
    index = int(list["begin"])
    result = 'digraph {\n'
    result += '\trankdir=LR\n'
    result += '\tgraph [ordering=out]\n'
    result += '\tnode [style=filled]\n'
    values = []

    result += '\tsentinel_begin [label="NULL", fillcolor="gray", shape=point]\n'
    result += '\tsentinel_end [label="NULL", fillcolor="gray", shape=point]\n'
    count = 0

    while index >= 0 and count <= size:
        try:
            value = data[index].cast(data_type)
            if data_type.code == gdb.TYPE_CODE_PTR:
                if data_type.target() == gdb.lookup_type('char'):
                    try:
                        value = value.string()
                    except gdb.error:
                        value = str(value)
                else:
                    value = str(value)
            elif data_type.code == gdb.TYPE_CODE_STRUCT:
                value = f"struct@{value.address}"
            else:
                value = value.format_string(format='d')
            result += f'\tnode_{index} [label={value}, fillcolor={fillcolor}]\n'
        except gdb.error:
            value = '?'
            result += f'\tnode_{index} [label="?", fillcolor=lightgray]\n'

        next_idx = int(next[index])
        if next_idx >= 0:
            result += f'\tnode_{index} -> node_{next_idx} \n'
        else:
            result += f'\tnode_{index} -> sentinel_end [style=dashed]\n'
        prev_idx = int(prev[index])
        if prev_idx >= 0:
            result += f'\tnode_{index} -> node_{prev_idx} [style=dashed] \n'
        else:
            result += f'\tsentinel_begin -> node_{index} [style=dashed,dir=back]\n'
        values.append(value)
        index = next_idx
        count += 1
    result += '}\n'
    if count > size:
        print(f"Error: There are more than {size} nodes")
    print(values)
    return result

def is_node_red(node: gdb.Value) -> bool:
    """Check if a node is red using the C function."""
    return bool(gdb.parse_and_eval(f"rb_tree_is_node_red({node})"))

def dot_btree(tree: gdb.Value, data_type: gdb.Type, args: argparse.Namespace):
    if tree["root"] == 0:
        print("Empty tree.")
        return

    stack = [tree["root"]]
    result = 'digraph {\n'
    result += '\tgraph [ordering="out"]\n'
    result += '\tnode [style=filled]\n'

    # Add a legend for red-black trees
    if args.red_black:
        result += '\tsubgraph cluster_legend {\n'
        result += '\t\tlabel="Legend"\n'
        result += '\t\tnode [style=filled]\n'
        result += '\t\tred_node [label="Red Node", fillcolor="lightpink"]\n'
        result += '\t\tblack_node [label="Black Node", fillcolor="lightgray"]\n'
        result += '\t}\n'

    sentinel_count = 0

    while len(stack) > 0:
        node = stack.pop()
        node_addr = int(str(node), 16)

        try:
            value = node["data"].cast(data_type.pointer()).dereference()
            if data_type.code == gdb.TYPE_CODE_PTR:
                if data_type.target() == gdb.lookup_type('char'):
                    try:
                        value = value.string()
                    except:
                        value = str(value)
                else:
                    value = str(value)
            elif data_type.code == gdb.TYPE_CODE_STRUCT:
                value = f"struct@{value.address}"
            else:
                value = str(value)

            # Set node color based on whether it's a red-black tree
            if args.red_black:
                fillcolor = "lightpink" if is_node_red(node) else "lightgray"
            else:
                fillcolor = "lightblue"

            result += f'\tnode_{node_addr} [label="{value}", fillcolor="{fillcolor}"]\n'
        except:
            result += f'\tnode_{node_addr} [label="?", fillcolor="lightgray"]\n'

        if node["ls"] != 0:
            left_addr = int(str(node["ls"]), 16)
            result += f'\tnode_{node_addr} -> node_{left_addr} \n'
            stack.append(node["ls"])
        else:
            result += f'\tsentinel_{sentinel_count} [label="NULL", fillcolor="gray", shape=point]\n'
            result += f'\tnode_{node_addr} -> sentinel_{sentinel_count} [style="dashed"]\n'
            sentinel_count += 1

        if node["rs"] != 0:
            right_addr = int(str(node["rs"]), 16)
            result += f'\tnode_{node_addr} -> node_{right_addr} \n'
            stack.append(node["rs"])
        else:
            result += f'\tsentinel_{sentinel_count} [label="NULL", fillcolor="gray", shape=point]\n'
            result += f'\tnode_{node_addr} -> sentinel_{sentinel_count} [style="dashed"]\n'
            sentinel_count += 1

    result += '}\n'
    return result

def dot_graph_list(graph: gdb.Value, tree: gdb.Value | None = None):
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
            edge = current["data"].cast(gdb.lookup_type("graph_list_edge_t").pointer()).dereference()
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

def dot_graph_mat(graph: gdb.Value, tree: gdb.Value | None = None):
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

def try_infer_type(hint: str | None, sizeof: int) -> gdb.Type:
    """Try to infer the type of data based on pointer size."""

    if hint is not None:
        try:
            return gdb.lookup_type(hint)
        except gdb.error:
            raise gdb.GdbError(f"Invalid type: {hint}")

    ptr_size = sizeof
    size_to_type = {
        4: 'int',
        8: 'double',
        1: 'char',
        2: 'short'
    }
    if ptr_size not in size_to_type:
        raise gdb.GdbError("Could not infer type and no type argument provided")
    return gdb.lookup_type(size_to_type[ptr_size])

def pointed2(value: gdb.Value):
    while value.type.code == gdb.TYPE_CODE_PTR:
        if value == 0:
            raise gdb.GdbError("Can't dereference a NULL pointer")
        value = value.dereference()
    return value

def run_dot(graph: str, from_tty: bool):
    # Create temporary files for dot and svg
    with tempfile.NamedTemporaryFile(mode='w', suffix='.dot', delete=False) as dot_file:
        dot_file.write(graph)
        dot_path = dot_file.name

    svg_path = dot_path[:-4] + '.svg'

    try:
        subprocess.run(['dot', '-Tsvg', dot_path, '-o', svg_path], check=True)
        if from_tty:
            if os.name == 'posix':
                subprocess.run(['xdg-open', svg_path])
            elif os.name == 'nt':
                os.startfile(svg_path)
            else:
                subprocess.run(['open', svg_path])
    except subprocess.CalledProcessError:
        print("Error: Failed to generate SVG. Is Graphviz installed?")
    except Exception as e:
        print(f"Error opening SVG: {e}")
    finally:
        try:
            os.unlink(dot_path)
        except Exception:
            pass

class DotList(gdb.Command):
    """Print list structures using the graphviz tools.
    Usage: dot-list <list_variable> [type]
    Options:
        type: Data type of the values stored in the tree (e.g., int, char*)
    Example: dot-list my_list int"""

    def __init__(self):
        super(DotList, self).__init__("dot-list", gdb.COMMAND_DATA, gdb.COMPLETE_SYMBOL)

    def invoke(self, argument, from_tty):
        args = gdb.string_to_argv(argument)
        try:
            # Parse arguments
            parser = argparse.ArgumentParser(exit_on_error=False)
            parser.add_argument('list', type=str)
            parser.add_argument('type', type=str, nargs='?')
            try:
                args = parser.parse_args(args)
            except argparse.ArgumentError as e:
                raise gdb.GdbError(f"Error: {e}")

            value = pointed2(gdb.parse_and_eval(args.list))

            # Get the data type if provided
            data_type = try_infer_type(args.type, int(value["size_bytes"]))

            printers = {
                "linked_list_t": dot_linked_list,
                "indexed_list_t": dot_indexed_list,
            }

            printer = printers.get(value.type.name)
            if printer is None:
                raise gdb.GdbError(f"Type {value.type.name} is not supported")

            dot_content = printer(value, data_type)
            if dot_content is None:
                return

            run_dot(dot_content, from_tty)

        except gdb.error as e:
            print(f"GDB error: {e}")

DotList()

class DotBtree(gdb.Command):
    """Print binary tree structures using the graphviz tools.
    Usage: dot-btree <tree_variable> [type] [-rb]
    Options:
        type: Data type of the values stored in the tree (e.g., int, char*)
        -rb: Indicate that this is a red-black tree
    Example: dot-btree my_tree int -rb"""

    def __init__(self):
        super(DotBtree, self).__init__("dot-btree", gdb.COMMAND_DATA, gdb.COMPLETE_SYMBOL)

    def invoke(self, argument, from_tty):
        args = gdb.string_to_argv(argument)
        try:
            # Parse arguments
            parser = argparse.ArgumentParser(exit_on_error=False)
            parser.add_argument('tree', type=str)
            parser.add_argument('type', type=str, nargs='?')
            parser.add_argument('-rb', '--red-black', action=argparse.BooleanOptionalAction)
            try:
                args = parser.parse_args(args)
            except argparse.ArgumentError as e:
                raise gdb.GdbError(f"Error: {e}")

            value = pointed2(gdb.parse_and_eval(args.tree))

            # Get the data type if provided
            data_type = try_infer_type(args.type, int(value["size_bytes"]))

            printers = {
                "btree_t": dot_btree,
                "bsearch_tree_t": dot_btree,
            }

            printer = printers.get(value.type.name)
            if printer is None:
                raise gdb.GdbError(f"Type {value.type.name} is not supported")

            dot_content = printer(value, data_type, args)
            if dot_content is None:
                return

            run_dot(dot_content, from_tty)

        except gdb.error as e:
            print(f"GDB error: {e}")

DotBtree()

class DotGraph(gdb.Command):
    """Print graph structures using the graphviz tools.
    Usage: dot-graph <graph_variable>
    Example: dot-graph my_graph"""

    def __init__(self):
        super(DotGraph, self).__init__("dot-graph", gdb.COMMAND_DATA, gdb.COMPLETE_SYMBOL)

    def invoke(self, argument, from_tty):
        args = gdb.string_to_argv(argument)
        try:
            # Parse arguments
            parser = argparse.ArgumentParser(exit_on_error=False)
            parser.add_argument('graph', type=str)
            parser.add_argument('tree', type=str, default=None, nargs='?')
            try:
                args = parser.parse_args(args)
            except argparse.ArgumentError as e:
                raise gdb.GdbError(f"Error: {e}")

            graph = pointed2(gdb.parse_and_eval(args.graph))
            tree = None
            if args.tree:
                tree = pointed2(gdb.parse_and_eval(args.tree))

            printers = {
                "graph_mat_t": dot_graph_mat,
                "graph_list_t": dot_graph_list,
            }

            printer = printers.get(graph.type.name)
            if printer is None:
                raise gdb.GdbError(f"Type {graph.type.name} is not supported")

            if tree is not None and (not tree.type.is_array_like or tree.type.target().code != gdb.TYPE_CODE_INT):
                raise gdb.GdbError(f"{tree} should be an array of integers")

            dot_content = printer(graph, tree)
            if dot_content is None:
                return

            run_dot(dot_content, from_tty)

        except gdb.error as e:
            print(f"GDB error: {e}")

DotGraph()
