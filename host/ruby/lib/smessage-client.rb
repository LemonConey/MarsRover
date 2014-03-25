require 'socket'

class SMessageClient
  attr_reader :ip, :port

  def initialize options = {}
    options[:port] ||= 4000
    options[:ip] ||= "localhost"

    @ip = options[:ip]
    @port = options[:port]
    
    create_socket
  end

  def send msg
    ap "send [#{serialize_message(msg)}] to #{@ip}:#{@port}"
    @socket.send serialize_message(msg), 0, @ip, @port
  end

  private

  def create_socket
    @socket = UDPSocket.new
  end

  def serialize_message(msg)
    SMessagePDU.serialize msg
  rescue
    msg.to_s
  end
end
