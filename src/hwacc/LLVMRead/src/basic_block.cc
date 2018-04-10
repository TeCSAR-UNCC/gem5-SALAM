#include <cassert>
#include "basic_block.hh"

BasicBlock::BasicBlock(std::string id, unsigned bbnum) {
    cnList = new std::list<ComputeNode *>();
    name = id;
    bbID = bbnum;
}

void
BasicBlock::addNode(ComputeNode * cn) {
    cnList->push_back(cn);
}


