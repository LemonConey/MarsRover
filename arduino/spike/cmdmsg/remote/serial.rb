#!/usr/bin/env ruby

require "serialport"
require "awesome_print"

SerialPort.open("/dev/ttyUSB0", 9600, 8, 1, SerialPort::NONE) do |serial|
  loop do
    ap serial.readline
  end
end