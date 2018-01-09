#include "hwacc/comp_interface.hh"

CompInterface::CompInterface(CompInterfaceParams *p) :
    SimObject(p),
    acc(p->dummy_acc) {
    
}

void
CompInterface::memRead(int varID, size_t offset, size_t length) {

}

uint8_t *
CompInterface::readSync() {
    return acc->getCurData();
}

void
CompInterface::memWrite(int varID, size_t offset, size_t length, uint8_t data[]) {

}

void
CompInterface::writeSync() {

}

void
CompInterface::finish() {

}

void
CompInterface::runApp() {

}

uint8_t *
CompInterface::configRead(size_t offset) {
    return 0;
}

void
CompInterface::configWrite(size_t offset) {

}

CompInterface*
CompInterfaceParams::create() {
    return new CompInterface(this);
}
