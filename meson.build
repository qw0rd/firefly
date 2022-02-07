
project('Firefly', 
  'cpp',
  version: '0.1',
  default_options: ['cpp_std=c++20', 'warning_level=3'])


cxx_flags = [
  '-ffreestanding',
  '-nostdlib',
  '-mno-red-zone',
  '-fno-pic',
  '-no-pie',
  '-mcmodel=kernel',

  '-mno-mmx',
  '-mno-sse',
  '-mno-sse2',
  '-mno-3dnow',

  '-fno-rtti',
  '-fno-exceptions'
]


link_flags = [
  '-nostdlib',
  '-zmax-page-size=0x1000',
  '-static'
]

subdir('lib')
subdir('kernel')

