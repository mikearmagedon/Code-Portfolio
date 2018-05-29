#include "elock.h"

ELock::ELock()
{
    ELock_Pin=2;
    pinMode(ELock_Pin, OUTPUT);
}

void ELock::open()
{
    digitalWrite(ELock_Pin, HIGH);
}

void ELock::close()
{
    digitalWrite(ELock_Pin, LOW);
}
