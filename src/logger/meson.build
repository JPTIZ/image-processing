fmt = dependency('fmt')

logger_inc = include_directories('.')
logger_lib = library('logger',
  dependencies : fmt)
logger = declare_dependency(
  include_directories : logger_inc,
  dependencies : fmt)
