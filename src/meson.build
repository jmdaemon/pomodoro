pomodoro_srcs = [ 'pomodoro.c', 'timer.c' ]

pomodoro = shared_library('pomodoro',
  pomodoro_srcs,
  include_directories: inc,
  dependencies: [deps])

pomodoro_static = static_library('pomodoro',
  pomodoro_srcs,
  include_directories: inc,
  dependencies: [deps])

dep_pomodoro = declare_dependency(
    link_with: [ pomodoro, deps ],
    include_directories: inc)

pomodoro_bin = executable('pomodoro',
  [ pomodoro_srcs, 'main.c' ],
  include_directories: inc,
  dependencies: [deps],
  link_with: pomodoro)
