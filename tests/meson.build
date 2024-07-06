passing_test_sources = [
  'btree_create_empty.c',
  'btree_emplace_at_override.c',
  'btree_emplace_at_root.c',
  'btree_emplace_at_several.c',
  'btree_emplace_fail_no_root.c',
  'btree_emplace_path_begin_to_end.c',
  'btree_emplace_path_offset_to_end.c',
  'btree_emplace_path_offset_to_length.c',
  'btree_height.c',
  'btree_inorder_traversal_of_perfect_tree.c',
  'btree_levelorder_traversal_of_perfect_tree.c',
  'btree_no_free_element.c',
  'btree_node_to_path_node.c',
  'btree_node_to_path_root.c',
  'btree_perfect_tree_from_tab.c',
  'btree_perfect_tree_from_tab_null_length.c',
  'btree_postorder_traversal_of_perfect_tree.c',
  'btree_preorder_traversal_of_perfect_tree.c',
  'graph_mat_bellman_unit_weights.c',
  'graph_mat_bfs.c',
  'graph_mat_create_no_edges.c',
  'graph_mat_dijkstra_unit_weights.c',
  'graph_mat_indegree.c',
  'graph_mat_outdegree.c',
  'graph_mat_preorder_dfs.c',
  'graph_mat_postorder_dfs.c',
  'graph_mat_set_get_edge.c',
  'graph_mat_topological_ordering_cycle.c',
  'list_clean.c',
  'list_create_empty.c',
  'list_find_in.c',
  'list_find_not_in.c',
  'list_from_tab.c',
  'list_insert_back.c',
  'list_insert_even.c',
  'list_insert_null.c',
  'list_no_free_element.c',
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
  'path_append_branch.c',
  'path_append_lhs_rhs.c',
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
