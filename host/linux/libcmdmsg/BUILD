cc_library(
  name = 'cmdmsg',
  incs = ['.'],
  srcs = ['CmdMessenger.cpp',
         'arduino-stream.cpp'],
  extra_cppflags = ['-Wno-conversion-null',
                    '-Wno-unused-value',
                    '-Wno-format-nonliteral',
                    '-Wno-type-limits']
)

cc_test(
  name = 'cmdmsgd-spec',
  srcs = ['spec/arduino-stream-spec.cpp'],
  deps = [':cmdmsg'],
  extra_cppflags = ['-Wno-sign-compare', '-Wno-sequence-point']
)
