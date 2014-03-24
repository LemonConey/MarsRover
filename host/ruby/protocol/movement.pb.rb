##
# This file is auto-generated. DO NOT EDIT!
#
require 'protobuf/message'


##
# Message Classes
#
class Movement < ::Protobuf::Message
  class Message < ::Protobuf::Enum
    define :Id, 2
  end

end



##
# Message Fields
#
class Movement
  required :sint32, :power, 1
  required :sint32, :direction, 2
  required :int32, :duration, 3
end

