#pragma once

#include <functional>
#include <nlohmann/json.hpp>

namespace Scene3d {

    struct JsonSerializable {
        virtual nlohmann::json toJson() const = 0;
        virtual void fromJson(const nlohmann::json &json) = 0;
    };

}
