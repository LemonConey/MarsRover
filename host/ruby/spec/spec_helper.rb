require "rspec"
require 'awesome_print'

# load all protocol
Dir.glob(File.expand_path("../../protocol/*.rb", __FILE__)).each do |file|
  require file
end
