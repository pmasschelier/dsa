benchmarks = [
  'list_ref_fill_and_clean.c'
]

foreach bench_source: benchmarks
    bench_name = bench_source.split('.')[0].underscorify()
    exe = executable(bench_name, bench_source, dependencies: lib_dep, include_directories: inc_dir)
    benchmark(bench_name, exe)
endforeach
