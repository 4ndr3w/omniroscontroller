#include <Arduino.h>

template <class RecvMsg, class SendMsg>
class SerialPort {
public:
    SerialPort(int baud) {
        Serial.begin(9600);
    }

    RecvMsg getMessage() {
        while ( Serial.available() < 1 );
        
        RecvMsg data;
        Serial.readBytes((char*)&data, sizeof(RecvMsg));
        return data;
    }

    void sendMessage(SendMsg data) {
        Serial.write((char*)&data, sizeof(SendMsg));
    }
};
