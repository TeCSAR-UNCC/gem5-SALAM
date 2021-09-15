#include "hwacc/stream_port.hh"
#include "sim/sim_object.hh"

StreamRequestPort::StreamRequestPort(const std::string& name, SimObject* _owner, PortID _id)
	: RequestPort(name, _owner, _id) {
	//
}

StreamRequestPort::~StreamRequestPort() {
	//
}

void
StreamRequestPort::bind(Port &peer) {
	auto *stream_slave = dynamic_cast<StreamResponsePort *>(&peer);
	if (stream_slave) {
		_stream_slave = stream_slave;
	}
	RequestPort::bind(peer);
}

void
StreamRequestPort::unbind() {
	_stream_slave = nullptr;
	RequestPort::unbind();
}