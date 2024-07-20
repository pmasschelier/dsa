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
  'byte': ['signed char', 'SCHAR_MAX'],
  'short': ['short', 'SHRT_MAX'],
  'int': ['int', 'INT_MAX'],
  'long': ['long int', 'LONG_MAX'],
  'llong': ['long long int', 'LLONG_MAX'],
}
dist_type = dist_types[get_option('weight_type')]
cdata.set('GRAPH_WEIGHT_TYPE', dist_type[0])
cdata.set('GRAPH_WEIGHT_INF', dist_type[1])

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

fail_test_tokens = {
  'abort': 'TEST_FAIL_FUNC_ABORT',
  'return': 'TEST_FAIL_FUNC_RET',
  'nocheck': 'TEST_FAIL_FUNC_NOCHECK'
}
fail_test_token = fail_test_tokens[get_option('test_fail')]
add_project_arguments(f'-D@fail_test_token@', language : 'c')

subdir('src')
subdir('tests')
subdir('benchmarks')

