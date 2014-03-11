#include "host-serial.h"

HostSerial::HostSerial( SendProc sender )
{
    m_sender = sender;
}

HostSerial::~HostSerial()
{

}

int HostSerial::print( byte data )
{
    return m_sender(data);
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

