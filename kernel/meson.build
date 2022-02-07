

firefly_src = [
    'kmain.cc'
]

inc_root = include_directories('../include')

ld_script_loc = meson.source_root() + '/ldscripts/x86_64.ld'


executable('firefly', 
  firefly_src, 
  cpp_args: cxx_flags, 
  link_args: [link_flags, '-T' + ld_script_loc], 
  include_directories: [inc_root], 
  link_with: [klib])

