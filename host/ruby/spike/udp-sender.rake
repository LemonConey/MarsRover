require 'bindata'
require 'socket'
namespace :spike do


  task :udpsender do

    mm = Protocol::Movement.new
    mm.power = 255
    mm.duration = 1000

    cmdbuf = Protocol::SMessagePDU.get_buffer 1, [mm, mm]

    sock = UDPSocket.new
    sock.bind "localhost", 5000

    Thread.new {
      loop do
        ap sock.recv 256
      end
    }

    loop do
      ap "send #{cmdbuf} size #{cmdbuf.size}"
      sock.send cmdbuf, 0, "127.0.0.1", 4000
      sleep 2
    end

  end

end
