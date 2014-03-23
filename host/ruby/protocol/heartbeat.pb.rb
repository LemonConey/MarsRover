##
# This file is auto-generated. DO NOT EDIT!
#
require 'protobuf/message'


##
# Message Classes
#
class Heartbeat < ::Protobuf::Message
  class Message < ::Protobuf::Enum
    define :Id, 1
  end

end



##
# Message Fields
#
class Heartbeat
  required :int32, :millis, 1
  required :int32, :ticks, 2
end

