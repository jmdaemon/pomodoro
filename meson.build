project('pomodoro', 'c',
version: '0.1.0',
subproject_dir: 'meson-subprojects') # Don't cause conflict with subprojects directory

inc = include_directories('include')

# Build subproject dependencies first
subdir('subprojects')

# Declare dependencies
proj_utility = subproject('utility')
dep_utility = proj_utility.get_variable('dep_utility')

deps = [ dep_utility, dep_logc, dep_tomlc99 ]

subdir('src')

