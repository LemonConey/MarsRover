require 'bindata'

module Protocol
  class Movement < BinData::Record
    MOVEMENT_MIN_POWER = 0
    MOVEMENT_MAX_POWER = 255

    endian :little
    int16 :power
    uint16 :duration
  end
end