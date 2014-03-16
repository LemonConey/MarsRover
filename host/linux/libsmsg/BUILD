cc_library(
  name = 'smsg',
  srcs = ['smsg-coder.cpp',
         'smsg-pdu.cpp',
         'smessage.cpp'
         ]
)

cc_test(
  name = 'smsg-spec',
  incs = ['.'],
  srcs = [
  'spec/smsg-coder-spec.cpp',
  'spec/smsg-pdu-spec.cpp',
  'spec/smessage-spec.cpp'
  ],
  deps = [':smsg']
)
