#include "Event.hpp"
#include <utility>

Event::Event(RequestType type, const GDTPHeader& header, payload_t payload)
    : type(type), header(header), payload(std::move(payload)) {}

RequestType Event::getType() const
{
    return type;
}

const Event::payload_t & Event::getPayload() const
{
    return payload;
}
