src_files = files(
  'btree_ref/avl_tree_ref.c',
  'btree_ref/btree_ref.c',
  'btree_ref/path.c',
  'graph/graph_cast.c',
  'graph/graph_list.c',
  'graph/graph_mat.c',
  'list_ref/list_ref.c',
  'list_ref/algorithms.c',
  'circular_buffer.c',
  'dynarray.c',
  'heap_view.c',
  'ptr.c',
)

lib = library('struct', src_files,
  include_directories: inc_dir,
  install: true,
  install_dir: get_option('libdir'))

# Install header files

# Specify dependencies for the library (if any)
lib_dep = declare_dependency(include_directories: inc_dir,
                         link_with: lib)

# Expose the library and dependency to other subdirectories
# lib_dep = [lib, dep]
