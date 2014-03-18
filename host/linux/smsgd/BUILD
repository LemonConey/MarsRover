cc_binary(
  name = 'smsgd',
  srcs = [
    'main.cpp',
    'client-mgr.cpp',
    'text-client-mgr.cpp'
    ],
  deps = ['#serial', '#PocoFoundation', '#PocoNet', '//libsmsg/:smsg'],
  extra_cppflags = ['-std=gnu++0x']
)

