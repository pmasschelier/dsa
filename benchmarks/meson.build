benchmarks = [
  'selection_sort.c',
  'insertion_sort.c'
]

# cbench = dependency('cbench', fallback: ['cbench', 'cbench_dep'])
b63 = subproject('b63')
b63_dep = b63.get_variable('b63_dep')

foreach bench_source: benchmarks
    bench_name = bench_source.split('.')[0].underscorify()
    exe = executable(
      bench_name,
      bench_source,
      dependencies: [lib_dep, b63_dep],
      c_args: ['-Wno-pedantic', '-Wno-implicit-function-declaration', '-Wno-unused-variable', '-Wno-unused-parameter', '-std=gnu99'])
endforeach
