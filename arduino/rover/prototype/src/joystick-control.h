#ifndef __JOYSTICK_CONTROL_H__
#define __JOYSTICK_CONTROL_H__

class JoystickControl {
public:
    struct Message {
        enum MessageId {
            Up, Down, Left, Right,
            A, B, C, D,
            LX, LY, LPress,
            RX, RY, RPress,
            L2, R2, Select, Start
        };
        char  id;
        char  value;
    };

    typedef void (*OnMessageProc)(Message *message, void *arg);
    JoystickControl(OnMessageProc callback, void *arg)
        : m_callback(callback), m_arg(arg) {
            initKeyMap();
    }

    void readFromSerial(const char *data, size_t len) {
        for (size_t i = 0; i < len; ++i) {
            char current = data[i];
            if (current - 'A' >= sizeof(m_keymap) ||
                m_keymap[current - 'A'] == -1) {
                    continue;
            }
            Message message = {m_keymap[current - 'A']};
            m_callback(&message, m_arg);
        }
    }

private:
    void initKeyMap() {
        memset(m_keymap, -1, sizeof(m_keymap));

        // map to the MessageId
        char charmap[] = {
            'A', 'B', 'C', 'D',
            'I', 'J', 'K', 'L',
            'P', 'O', 'E',
            'S', 'Q', 'M',
            'F', 'N', 'G', 'H'
        };
        for(int i = 0; i < sizeof(charmap); ++i) {
            m_keymap[charmap[i] - 'A'] = i;
        }
    }
    void            *m_arg;
    OnMessageProc   m_callback;
    char            m_keymap[26];
};

#endif