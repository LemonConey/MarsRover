require 'bindata'
require 'socket'
require 'io/console'
require 'protobuf'
namespace :spike do

  @remote_ip = "192.168.1.150"
  @remote_port = 4000
  @remote_ip = ENV["remote"] unless ENV["remote"].nil?

  desc "register commands"
  task :register do
    sock = UDPSocket.new
    sock.send "register all", 0, @remote_ip, @remote_port
    p "register sent"
    loop do
      bytes = sock.recv 256
      header = Protocol::SMessagePDU::Header.read bytes
      ap header
      begin
        hb = Heartbeat.new
        hb.parse_from_string header.payload.to_s
        ap hb
      rescue => e
        ap e
      end
    end
  end

  desc "send commands"
  task :control do

    ap "remote #{@remote_ip}:#{@remote_port}"

    sock = UDPSocket.new

    Thread.new {
      loop do
        sock.recv 256
      end
    }

    ap Movement::Message::Id

    power = 120
    duration = 300

    loop do
      ch = STDIN.getch
      if ch == 'q'
        exit
      end


      mm = Movement.new

      case ch
        when 'q'
          exit
        when 'w'
          mm.power = power
          mm.direction = 0
        when 's'
          mm.power = -power
          mm.direction = 0
        when 'a'
          mm.power = power
          mm.direction = -100
        when 'd'
          mm.power = power
          mm.direction = 100
      end
      mm.duration = duration
      cmdbuf = Protocol::SMessagePDU.get_buffer mm.class::Message::Id.to_i, mm
      ap mm
      ap "send #{cmdbuf} size #{cmdbuf.size}"
      sock.send cmdbuf, 0, @remote_ip, @remote_port

    end

  end

end
