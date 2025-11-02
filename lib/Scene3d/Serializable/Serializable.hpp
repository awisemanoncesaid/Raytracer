#pragma once

#include <functional>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Scene3d {

    struct JsonSerializable {
        virtual ~JsonSerializable() = default;
        virtual void toJson(json &json) const = 0;
        virtual void fromJson(const json &json) = 0;
    };

}
