sfml = [dependency('sfml-graphics')]

viewer_inc = include_directories('.')

viewer_lib = library('viewer',
  'window.cpp',
  'image.cpp',
  include_directories : include_directories('.'),
  dependencies: [sfml, logger],
  install : true)

viewer = declare_dependency(
  include_directories : viewer_inc,
  link_with : viewer_lib,
  dependencies : [sfml, logger])
