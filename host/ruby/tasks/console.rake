require "irb"

desc "start a console with prepared environment"
task :console do
  ARGV.clear
  IRB.start
end
