#include "rover-common.h"
#include "arduino-serial-source.h"

static SMessage *smessage = NULL;

void unhandled_message(SMessagePDU::Message *msg, void *) {
    get_lcd1602()->printAt(1, "type:%02hhu size:%02hhu", msg->type, msg->size);
}

void init_smessage( Stream *comm )
{
    smessage = new SMessage(new SerialSource(comm));
    get_smessage()->onUnhandledMessage(unhandled_message);
}

SMessage *get_smessage() {
    return smessage;
}