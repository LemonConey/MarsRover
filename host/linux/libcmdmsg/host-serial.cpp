#include "host-serial.h"

HostSerial::HostSerial()
{

}

HostSerial::~HostSerial()
{

}

int HostSerial::print( byte data )
{
    // todo - move data to serial
    return 0;
}

size_t HostSerial::available()
{
    return m_readBuffer.size();
}

byte HostSerial::read()
{
    // no sanity check
    byte data = m_readBuffer.front();
    m_readBuffer.pop_front();
    return data;
}

void HostSerial::push( byte data )
{
    m_readBuffer.push_back(data);
}

