cc_library(
  name = 'smsg',
  srcs = ['smsg-coder.cpp',
         'smsg-pdu.cpp',
         'smessage.cpp',
         'smsg-pdu.cpp'
         ]
)

cc_test(
  name = 'smsg-spec',
  incs = ['.'],
  srcs = [
  'spec/smsg-coder-spec.cpp',
  'spec/smsg-pdu-spec.cpp'
  ],
  deps = [':smsg'],
  extra_cppflags = ['-std=gnu++0x']
)
