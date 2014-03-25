require 'bindata'
require 'protobuf'

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

  def self.get_buffer type, data
    buffer = [data].flatten.map(&:to_s).join
    header = Header.new
    header.type = type
    header.datasize = buffer.size
    header.to_binary_s + buffer
  end
end
