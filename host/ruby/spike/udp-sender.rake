require 'bindata'
require 'socket'
namespace :spike do


  task :udpsender do

    mm = Protocol::Movement.new
    mm.power = 255
    mm.duration = 1000

    pdu = Protocol::Header.new
    pdu.type = 1
    pdu.datasize = mm.to_binary_s.size * 2
    pdu.checksum = 0


    cmd = [pdu, mm, mm];
    cmdbuf = cmd.map(&:to_binary_s).join
    ap cmdbuf
    sock = UDPSocket.new
    loop do
      sock.send cmdbuf, 0, "localhost", 1314
      sleep 5
    end

  end

end
