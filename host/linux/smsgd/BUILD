cc_binary(
  name = 'smsgd',
  srcs = [
    'main.cpp',
    'client-mgr.cpp',
    'text-client-mgr.cpp',
    'smsg-app.cpp'
    ],
  deps = [
    '#serial', 
    '#pthread',
    '#PocoFoundation', 
    '#PocoUtil', 
    '#PocoNet', 
    '//libsmsg/:smsg'
    ],
  extra_cppflags = ['-std=gnu++0x']
)

