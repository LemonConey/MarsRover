require 'bindata'

module Protocol
  class Header < BinData::Record
    endian :little
    uint8 :type
    uint8 :datasize
    uint16 :checksum
  end
end