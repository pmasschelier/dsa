testdirs = [
  'array_list',
  'linked_list',
  'btree_ref',
  'graph_cast',
  'graph_mat',
  'graph_list',
  'htable_ref',
  'path', 'heap_view',
  'circular_buffer',
  'avl_tree_ref',
  'rb_tree_ref',
  'bsearch_tree_ref',
  'weight_type',
]

libtap = dependency('libtap')

foreach dirname : testdirs
  subdir(dirname)
  foreach test_source : passing_tap_sources
    test_name = test_source.split('.')[0].underscorify()
    exe = executable(test_name, dirname / test_source, dependencies: [lib_dep, libtap], include_directories: inc_dir)
    test(test_name, exe, suite: dirname, should_fail: false, protocol: 'tap')
  endforeach

  foreach test_source : passing_return_sources
    test_name = test_source.split('.')[0].underscorify()
    exe = executable(test_name, dirname / test_source, dependencies: [lib_dep], include_directories: inc_dir)
    test(test_name, exe, suite: dirname, should_fail: false)
  endforeach
endforeach
