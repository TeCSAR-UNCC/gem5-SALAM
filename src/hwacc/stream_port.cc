#include "hwacc/stream_port.hh"
#include "sim/sim_object.hh"

StreamMasterPort::StreamMasterPort(const std::string& name, SimObject* _owner, PortID _id)
	: MasterPort(name, _owner, _id) {
	//
}

StreamMasterPort::~StreamMasterPort() {
	//
}

void
StreamMasterPort::bind(Port &peer) {
	auto *stream_slave = dynamic_cast<StreamSlavePort *>(&peer);
	if (stream_slave) {
		_stream_slave = stream_slave;
	}
	MasterPort::bind(peer);
}

void
StreamMasterPort::unbind() {
	_stream_slave = nullptr;
	MasterPort::unbind();
}