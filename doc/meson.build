doxy_data = configuration_data()
doxy_data.set('VERSION', meson.project_version())

if find_program('dot', required : false).found()
  doxy_data.set('HAVE_DOT', 'YES')
else
  doxy_data.set('HAVE_DOT', 'NO')
endif

if find_program('pdf2svg', required : false).found() or find_program('inkscape', required: false).found()
  doxy_data.set('HTML_FORMULA_FORMAT', 'svg')
else
  doxy_data.set('HTML_FORMULA_FORMAT', 'png')
endif

if find_program('latex', required : false).found() and find_program('gs', required : false).found() and find_program('dvips', required : false).found()
  doxy_data.set('USE_MATHJAX', 'no')
else
  doxy_data.set('USE_MATHJAX', 'yes')
endif


doxy_data.set('TOP_SRCDIR', meson.project_source_root())
doxy_data.set('TOP_BUILDDIR', meson.project_build_root())

doxyfile = configure_file(input: 'Doxyfile.in',
                          output: 'Doxyfile',
                          configuration: doxy_data,
                          install: false)

datadir = join_paths(get_option('datadir'), 'doc', 'struct')

html_target = custom_target('doc',
                            input: doxyfile,
                            depend_files: header_files,
                            output: 'html',
                            command: [doxygen, doxyfile],
                            install: true,
                            install_dir: datadir)
