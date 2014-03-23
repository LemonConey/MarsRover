require 'bindata'

module Protocol
  class SMessagePDU
    class Header < BinData::Record
      endian :little
      uint8 :type
      uint8 :datasize
      rest  :payload
    end

    def self.get_buffer type, data
      buffer = [data].flatten.map(&:to_s).join
      header = Header.new
      header.type = type
      header.datasize = buffer.size
      header.to_binary_s + buffer
    end
  end

end