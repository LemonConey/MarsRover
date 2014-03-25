namespace :raw do
  desc "control motor on arduino"
  task :movement do

    smc = SMessageClient.new ip: ENV["remote"]
    ap "connect to #{smc.ip}:#{smc.port}"

    power = (ENV["power"] || 120).to_i
    duration = (ENV["duration"] || 300).to_i

    loop do
      ch = STDIN.getch

      mm = Movement.new
      mm.power = power
      mm.duration = duration
      mm.direction = 0

      case ch
        when 'q'
          exit
        when 'w'
        when 's'
          mm.power = -power
        when 'a'
          mm.direction = -100
        when 'd'
          mm.direction = 100
        else
          ap "illegal key #{ch}"
          next
      end

      smc.send mm
    end

  end
end