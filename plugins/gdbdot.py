import gdb
import subprocess
import tempfile
import os

def try_infer_type(void_ptr: gdb.Value) -> gdb.Type:
    """Try to infer the type of data based on pointer size."""
    try:
        ptr_size = void_ptr.type.sizeof
        size_to_type = {
            4: 'int',
            8: 'double',
            1: 'char',
            2: 'short'
        }
        if ptr_size in size_to_type:
            return gdb.lookup_type(size_to_type[ptr_size])
    except:
        pass
    return None

def is_node_red(node: gdb.Value) -> bool:
    """Check if a node is red using the C function."""
    try:
        return bool(gdb.parse_and_eval(f"rb_tree_is_node_red({node})"))
    except:
        return False

def dot_btree(tree: gdb.Value, data_type: gdb.Type = None, is_rb_tree: bool = False):
    if tree["root"] == 0:
        print("Empty tree.")
        return
    
    if data_type is None and tree["root"] != 0:
        data_type = try_infer_type(tree["root"]["p"])
        if data_type is None:
            print("Could not infer type and no type argument provided")
            return
        print(f"Inferred type: {data_type}")
    
    stack = [tree["root"]]
    result = 'digraph {\n'
    result += '\tgraph [ordering="out"]\n'
    result += '\tnode [style=filled]\n'
    
    # Add a legend for red-black trees
    if is_rb_tree:
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
            value = node["p"].cast(data_type.pointer()).dereference()
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
            if is_rb_tree:
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

class DotBtree(gdb.Command):
    """Print binary tree structures using the graphviz tools.
    Usage: dot-btree <tree_variable> [type] [-rb]
    Options:
        type: Data type of the values stored in the tree (e.g., int, char*)
        -rb: Indicate that this is a red-black tree
    Example: dot-btree my_tree int -rb"""
    
    def __init__(self):
        super(DotBtree, self).__init__("dot-btree", gdb.COMMAND_DATA, gdb.COMPLETE_SYMBOL)
    
    def invoke(self, arg, from_tty):
        args = gdb.string_to_argv(arg)
        if not args:
            print("Missing tree variable argument")
            return
            
        try:
            # Parse arguments
            value = gdb.parse_and_eval(args[0])
            while value.type.code == gdb.TYPE_CODE_PTR:
                if value == 0:
                    print("Can't dereference a NULL pointer")
                    return
                value = value.dereference()
            
            # Get the data type if provided
            data_type = None
            is_rb_tree = False
            
            # Parse remaining arguments
            for arg in args[1:]:
                if arg == "-rb":
                    is_rb_tree = True
                else:
                    try:
                        data_type = gdb.lookup_type(arg)
                    except gdb.error:
                        print(f"Invalid type: {arg}")
                        return
            
            printer = supported_types.get(value.type.name)
            if printer is None:
                print(f"Type {value.type.name} is not supported")
                return
            
            dot_content = printer(value, data_type, is_rb_tree)
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
                except:
                    pass
                
        except gdb.error as e:
            print(f"GDB error: {e}")
        except Exception as e:
            print(f"Error: {e}")

supported_types = {
    "btree_ref_t": dot_btree,
    "bsearch_tree_ref_t": dot_btree,
}

DotBtree()
