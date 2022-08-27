project('pomodoro', 'c',
version: '0.1.0',
subproject_dir: 'meson-subprojects') # Don't cause conflict with subprojects directory

# Single source project version
project_version = meson.project_version().split('.')

conf_data = configuration_data()
conf_data.set('CMAKE_PROJECT_VERSION_MAJOR', project_version[0])
conf_data.set('CMAKE_PROJECT_VERSION_MINOR', project_version[1])
conf_data.set('CMAKE_PROJECT_VERSION_PATCH', project_version[2])

configure_file(input : 'include/version.h.in',
               output : 'version.h',
               configuration : conf_data)

inc = include_directories('include')

# Build subproject dependencies first
subdir('subprojects')

# Declare dependencies
proj_utility = subproject('utility')
dep_utility = proj_utility.get_variable('dep_utility')

deps = [ dep_utility, dep_logc, dep_tomlc99 ]

subdir('src')

