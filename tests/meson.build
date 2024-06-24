passing_test_sources = [
  'btree_node_to_path_node.c',
  'btree_node_to_path_root.c',
  'list_clean.c',
  'list_create_empty.c',
  'list_find_in.c',
  'list_find_not_in.c',
  'list_from_tab.c',
  'list_insert_back.c',
  'list_insert_even.c',
  'list_insert_null.c',
  'list_push_back.c',
  'list_push_front.c',
  'list_push_and_pop_back.c',
  'list_push_and_pop_front.c',
  'list_pop_back_to_empty.c',
  'list_pop_front_to_empty.c',
  'list_remove_back_to_empty.c',
  'list_remove_even.c',
  'list_remove_front_to_empty.c',
  'list_transform_square.c',
  'tab_from_list.c',
]

failing_test_sources = [
]

# exec = executable('list', 'list_from_tab.c', dependencies: lib_dep, include_directories: inc_dir)
# test('List basic tests', exec)
#
# exec = executable('', 'list_push_back.c', dependencies: lib_dep, include_directories: inc_dir)
# test('List basic tests', exec)

foreach test_source : passing_test_sources
  test_name = test_source.split('.')[0]
  exe = executable(test_name, test_source, dependencies: lib_dep, include_directories: inc_dir)
  test(test_name, exe)
endforeach

foreach test_source : failing_test_sources
  test_name = test_source.split('.')[0]
  exe = executable(test_name, test_source, dependencies: lib_dep, include_directories: inc_dir)
  test(test_name, exe, should_fail: true)
endforeach
