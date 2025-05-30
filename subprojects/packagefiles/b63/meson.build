project('b63', 'c', meson_version : '>=1.0.0')

cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required : false)

lib_dep = declare_dependency(include_directories: 'include', dependencies: [m_dep])
set_variable(meson.project_name() + '_dep', lib_dep)
