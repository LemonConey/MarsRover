cc_binary(
  name = 'cmdmsgd',
  srcs = [
    'main.cpp',
    'serial-reader.cpp'
    ],
  deps = ['#serial', '//libcmdmsg/:cmdmsg'],
  extra_cppflags = ['-std=c++11']
)

cc_test(
  name = 'cmdmsgd-spec',
  srcs = [],
)

