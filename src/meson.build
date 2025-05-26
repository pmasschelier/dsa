src_files = files(
  'btree_ref/avl_tree_ref.c',
  'btree_ref/rb_tree_ref.c',
  'btree_ref/btree_ref.c',
  'btree_ref/bsearch_tree_ref.c',
  'btree_ref/path.c',
  'graph/graph_cast.c',
  'graph/graph_list.c',
  'graph/graph_mat.c',
  'htable_ref/htable_list_ref.c',
  'list_ref/linked_list_ref.c',
  'list_ref/algorithms.c',
  'circular_buffer.c',
  'array_list.c',
  'heap_view.c',
  'weight_type.c',
)

lib = library('struct', src_files,
  include_directories: inc_dir,
  install: true,
  install_dir: get_option('libdir'))

# Install header files

# Specify dependencies for the library (if any)
lib_dep = declare_dependency(include_directories: inc_dir,
                         link_with: lib)
