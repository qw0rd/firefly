
project('Firefly', 
  'cpp',
  version: '0.1',
  default_options: ['cpp_std=c++20', 'warning_level=3'])


cxx_flags = [
  '-ffreestanding',
  '-nostdlib',
  '-mno-red-zone',
  '-no-pie',
  '-mcmodel=kernel',

  '-fno-rtti',
  '-fno-exceptions'
]


link_flags = [
  '-nostdlib',
  '-zmax-page-size=0x1000',
  '-mcmodel=kernel',
  '-static'
]

subdir('kernel')

