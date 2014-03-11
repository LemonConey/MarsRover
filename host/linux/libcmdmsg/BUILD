cc_library(
  name = 'cmdmsg',
  incs = ['.'],
  srcs = ['CmdMessenger.cpp',
         'host-serial.cpp'],
  extra_cppflags = ['-Wno-conversion-null',
                    '-Wno-unused-value',
                    '-Wno-format-nonliteral',
                    '-Wno-type-limits']
)

cc_test(
  name = 'cmdmsgd-spec',
  srcs = ['spec/host-serial-spec.cpp'],
  deps = [':cmdmsg']
)
