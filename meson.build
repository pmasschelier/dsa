project('structures', 'c',
  version: '1.0',
  default_options: [
    'buildtype=debug',
    # 'buildtype=debugoptimized',
    'warning_level=3',
    'default_library=both'
  ]
)

# Option to build shared or static library
# lib_type = get_option('default_library')

inc_dir = include_directories('include')

doxygen = find_program('doxygen', required : false)
if not doxygen.found()
  error('MESON_SKIP_TEST doxygen not found.')
endif

cdata = configuration_data()
cdata.set('VERSION', meson.project_version())

if find_program('dot', required : false).found()
  cdata.set('HAVE_DOT', 'YES')
else
  cdata.set('HAVE_DOT', 'NO')
endif

subdir('src')
subdir('include')
subdir('tests')
subdir('doc')


