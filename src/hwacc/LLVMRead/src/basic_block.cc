#include <cassert>
#include "basic_block.hh"

//BasicBlockList::BasicBlockList() {
//    head = NULL;
//    tail = NULL;
//    numBasicBlocks = 0;
//}

//void
//BasicBlockList::addBasicBlock(BasicBlock *bb) {
//    if (!head) {
//        head = bb;
//        tail = bb;
//        numBasicBlocks = 1;
//    } else {
//        tail->next = bb;
//        tail = bb;
//        numBasicBlocks++;
//    }
//}

//BasicBlock *
//BasicBlockList::findBasicBlock(std::string name) {
//    BasicBlock * currBB = head;

//    while (currBB->name.compare(name) != 0) {
//        assert(currBB != tail);
//        currBB = currBB->next;
//    }
//    return currBB;
//}

BasicBlock::BasicBlock(std::string id, unsigned bbnum) {
//    start = NULL;
//    end = NULL;
//    currNode = NULL;
    cnList = new std::list<ComputeNode *>();
//    numNodes = 0;
    name = id;
    bbID = bbnum;
    //next = NULL;
}

void
BasicBlock::addNode(ComputeNode * cn) {
//    if (!start) {
//        start = cn;
//        end = cn;
//        numNodes = 1;
//    } else {
//        end->next = cn;
//        end = cn;
//        numNodes++;
//    }
    cnList->push_back(cn);
}

//ComputeNode *
//BasicBlock::step() {
//    assert(currNode != end);
//    currNode = currNode->next;
//    return currNode;
//}
