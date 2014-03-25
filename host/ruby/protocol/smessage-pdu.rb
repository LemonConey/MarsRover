require 'bindata'
require 'protobuf'

class Class
  def descendants
    ObjectSpace.each_object(::Class).select { |klass| klass < self }
  end
end

class SMessagePDU
  class Header < BinData::Record
    endian :little
    uint8 :type
    uint8 :datasize
    rest :payload
  end

  def self.serialize pbinstance
    return nil unless pbinstance.class < Protobuf::Message
    msgid = pbinstance.class::Message::Id.to_i
    pbmsg = pbinstance.serialize
    Header.new.tap { |header|
      header.type = msgid
      header.datasize = pbmsg.size
      header.payload = pbmsg
    }.to_binary_s
  rescue => e
    ap e
    nil
  end

  def self.parse buffer
    msgpdu = Header.read buffer
    return nil if msgpdu.datasize != msgpdu.payload.size

    Protobuf::Message.descendants.each do |msg_class|
      begin
        return msg_class.new.tap { |pbinst|
          pbinst.parse_from_string msgpdu.payload.to_s
        } if msgpdu.type == msg_class::Message::Id
      rescue => e
      end
    end

    nil
  end
end
