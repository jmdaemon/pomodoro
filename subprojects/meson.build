# Log.c
src_logc = 'log.c/src'
#srcs_logc = [ src_logc / 'log.c' ]
logc = shared_library('logc',
  #'log.c/src',
  #srcs_logc,
  src_logc / 'log.c',
  include_directories: [inc, src_logc])

  #srcs_logc,
  #files('log.c')
  #files(srcs_logc)
  #include_directories: [inc, src_logc])

dep_logc = declare_dependency(
  link_with: logc,
  include_directories: [inc, src_logc])

# Tomlc99
src_tomlc99 = 'tomlc99'
srcs_tomlc99 = [ src_tomlc99 / 'toml.c' ]
tomlc99 = shared_library('tomlc99',
  #srcs_tomlc99,
  src_tomlc99 / 'toml.c',
  include_directories: [inc, src_tomlc99])

dep_tomlc99 = declare_dependency(
  link_with: tomlc99,
  include_directories: [inc, src_tomlc99])

#inc += src_logc src_tomlc99

#subproject('utility')
#subdir('utility')
