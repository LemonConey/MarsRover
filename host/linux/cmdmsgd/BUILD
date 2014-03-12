cc_binary(
  name = 'cmdmsgd',
  srcs = [
    'main.cpp',
    'blocking-reader.cpp'
    ],
  deps = ['#serial', '#PocoFoundation', '#PocoNet', '//libcmdmsg/:cmdmsg'],
  extra_cppflags = ['-std=gnu++0x']
)

cc_test(
  name = 'cmdmsgd-spec',
  srcs = [],
)

