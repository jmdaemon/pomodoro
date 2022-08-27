project('pomodoro', 'c',
version: '0.1.0',
#subproject_dir: 'subprojects')
subproject_dir: 'meson-subprojects')

inc = include_directories('include')

#dep_utility = dependency('utility')
#dep_logc    = dependency('logc')
#dep_tomlc99 = dependency('tomlc99')

# Build subproject dependencies first
subdir('subprojects')
#subproject('subprojects')
#include_directories('subprojects')

# Declare dependencies

proj_utility = subproject('utility')
dep_utility = proj_utility.get_variable('dep_utility')

deps = [ dep_utility, dep_logc, dep_tomlc99 ]


subdir('src')

