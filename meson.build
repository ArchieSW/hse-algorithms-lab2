project('lab2', 'cpp',
  version : '0.1',
  default_options : [
    'warning_level=0',
    # 'b_lundef=false',
    # 'b_sanitize=leak',
    'cpp_std=c++20'
  ]
)

deps = []

exe = executable(
  'lab2', 'lab2.cpp', 
  dependencies: deps,
  install : true,
  cpp_args: [
    # '-Wnullability-extension', 
    # '-stdlib=libc++',
    # '-fsanitize=undefined',
    # '-fsanitize=memory',
    '-lc++'
  ]
)

test('basic', exe)

benchmarks = executable(
  'bench', './src/benchmarks/Benchmark.cpp',
  cpp_args: [
    '-O0',
    '-lc++'
  ]
)
