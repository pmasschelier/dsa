project('structures', 'c',
  version: '0.1',
  meson_version: '>=1.1.0',
  license: 'MIT',
  license_files: 'LICENSE',
  default_options: [
    'buildtype=debug',
    # 'buildtype=debugoptimized',
    'warning_level=3',
    'default_library=both',
    'c_std=c99'
  ]
)

cdata = configuration_data()
version = meson.project_version().split('.')
cdata.set('VERSION_MAJOR', version[0])
cdata.set('VERSION_MINOR', version[1])

dist_types = {
  'byte': ['signed char', 'SCHAR'],
  'short': ['short', 'SHRT'],
  'int': ['int', 'INT'],
  'long': ['long int', 'LONG'],
  'llong': ['long long int', 'LLONG'],
}
dist_type = dist_types[get_option('weight_type')]
cdata.set('GRAPH_WEIGHT_TYPE', dist_type[0])
cdata.set('GRAPH_WEIGHT_TOKEN', dist_type[1])

install_incdir = join_paths(get_option('includedir'), 'struct')

conf = configure_file(input: 'config.h.in',
                      output: 'config.h',
                      configuration: cdata,
                      install: true,
                      install_dir: install_incdir)

# Option to build shared or static library
# lib_type = get_option('default_library')

inc_dir = include_directories('.', 'include')

subdir('include')

doxygen = find_program('doxygen', required : false)
if doxygen.found()
  subdir('doc')
else
  warning('doxygen not found: doc target will not be generated')
endif

stack_impls = {
  'list_ref': 'STACK_IMPL_LIST_REF',
  'dynarray': 'STACK_IMPL_DYNARRAY',
}
stack_impl = stack_impls[get_option('stack_impl')]
add_project_arguments(f'-D@stack_impl@', language : 'c')

fail_test_tokens = {
  'abort': 'TEST_FAIL_FUNC_ABORT',
  'return': 'TEST_FAIL_FUNC_RET',
  'nocheck': 'TEST_FAIL_FUNC_NOCHECK'
}
fail_test_token = fail_test_tokens[get_option('test_fail')]
add_project_arguments(f'-D@fail_test_token@', language : 'c')

if get_option('dijkstra_heap')
  add_project_arguments('-DDIJKSTRA_HEAP_IMPL', language : 'c')
endif

if get_option('test_fail') != 'return'
  add_project_arguments('-Wno-unused-label', language: 'c')
endif

if get_option('recursive')
  add_project_arguments(f'-DSTRUCT_RECURSIVE_IMPL', language : 'c')
else
  add_project_arguments('-Wno-implicit-fallthrough', language: 'c')
endif

subdir('src')
subdir('tests')
subdir('benchmarks')

