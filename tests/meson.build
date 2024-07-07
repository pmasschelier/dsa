testdirs = ['list_ref', 'btree_ref', 'graph_mat', 'graph_list', 'path']
foreach dirname : testdirs
  subdir(dirname)
  foreach test_source : passing_test_sources
    test_name = test_source.split('.')[0].underscorify()
    exe = executable(test_name, dirname / test_source, dependencies: lib_dep, include_directories: inc_dir)
    test(test_name, exe, suite: dirname, should_fail: false)
  endforeach
endforeach
