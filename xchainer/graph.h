#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <ostream>

#include "xchainer/hash_combine.h"

namespace xchainer {

using GraphSubId = uint64_t;

class Context;

class GraphId {
public:
    GraphId(Context& context, GraphSubId sub_id) : context_{context}, sub_id_{sub_id} {}

    GraphId(const GraphId&) = default;
    GraphId(GraphId&&) = default;
    GraphId& operator=(const GraphId&) = default;
    GraphId& operator=(GraphId&&) = default;

    bool operator==(const GraphId& other) const { return &context_ == &other.context_ && sub_id_ == other.sub_id_; }

    bool operator!=(const GraphId& other) const { return !operator==(other); }

    Context& context() const { return context_; }
    GraphSubId sub_id() const { return sub_id_; }

private:
    // Using reference_wrapper to make this class move assignable
    std::reference_wrapper<Context> context_;

    GraphSubId sub_id_;
};

std::ostream& operator<<(std::ostream& os, const GraphId& graph_id);

// Used to represent any graph (id).
class AnyGraph {};

}  // namespace xchainer

namespace std {

template <>
struct hash<xchainer::GraphId> {
    size_t operator()(const xchainer::GraphId& graph_id) const {
        size_t seed = std::hash<xchainer::Context*>()(&graph_id.context());
        xchainer::internal::HashCombine(seed, std::hash<xchainer::GraphSubId>()(graph_id.sub_id()));
        return seed;
    }
};

}  // namespace std
