require 'bindata'
require 'socket'
namespace :spike do
  class Header < BinData::Record
    endian :little
    uint8 :type
    uint8 :datasize
    uint16 :checksum
  end

  task :udpsender do
    h = Header.new
    h.type = 1
    h.datasize = 2
    h.checksum = 16

    sock = UDPSocket.new
    loop do
      sock.send h.to_binary_s, 0, "localhost", 1314
      sleep 1
    end

  end

end
