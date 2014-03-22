require 'bindata'
require 'socket'
require 'io/console'
namespace :spike do

  @remote_ip = "192.168.1.150"
  @remote_port = 4000

  desc "register commands"
  task :register do
    sock = UDPSocket.new 
    sock.send "register all", 0, @remote_ip, @remote_port
    p "register sent"
    loop do
      p "receive"
      ap sock.recv 256
    end
  end
  
  desc "send commands"
  task :control do

    sock = UDPSocket.new
    #sock.bind "localhost", 5000

    Thread.new {
      loop do
        sock.recv 256
      end
    }

    power = 100
    duration = 1000

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
          mm[0].power = power
          mm[1].power = power
        when 's'
          mm[0].power = -power
          mm[1].power = -power
        when 'a'
          mm[0].power = -power
          mm[1].power = power
        when 'd'
          mm[0].power = power
          mm[1].power = -power
      end
      mm[0].duration = duration
      mm[1].duration = duration
      cmdbuf = Protocol::SMessagePDU.get_buffer 1, mm

      ap "send #{cmdbuf} size #{cmdbuf.size}"
      sock.send cmdbuf, 0, @remote_ip, @remote_port
    end

  end

end
