require 'bindata'
require 'socket'
require 'io/console'
namespace :spike do


  task :udpsender do

    sock = UDPSocket.new
    sock.bind "localhost", 5000

    Thread.new {
      loop do
        sock.recv 256
      end
    }

    loop do
      ch = STDIN.getch
      if ch == 'q'
        exit
      end

      mm = [Protocol::Movement.new, Protocol::Movement.new]

      case ch
        when 'q'
          exit
        when 'w'
          mm[0].power = 255
          mm[1].power = 255
        when 's'
          mm[0].power = -255
          mm[1].power = -255
        when 'a'
          mm[0].power = -255
          mm[1].power = 255
        when 'd'
          mm[0].power = 255
          mm[1].power = -255
      end
      mm[0].duration = 1000
      mm[1].duration = 1000
      cmdbuf = Protocol::SMessagePDU.get_buffer 1, mm

      ap "send #{cmdbuf} size #{cmdbuf.size}"
      sock.send cmdbuf, 0, "127.0.0.1", 4000
    end

  end

end
