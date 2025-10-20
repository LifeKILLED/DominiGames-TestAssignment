#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>

namespace Renderer {

    struct MeshAttribute {
        enum Type {
            Position = 0,
            Normal,
            UV,
            Color,
        };
        Type type {};
        int size {};
    };

    struct BasicVertex {
        float position[3]{};
        float normal[3]{};
        float uv[2]{};
        float color[4]{};
    };

    class MeshData {
    public:
        MeshData() = default;
        ~MeshData() = default;

        std::vector<MeshAttribute> attributes;
        std::vector<BasicVertex> vertices;
        std::vector<uint32_t> indices;

        bool LoadFromString(const std::string &content) {
            std::stringstream ss(content);
            std::string line;

            enum class ReadingMode {
                None,
                Attributes,
                Vertices,
                Indices,
            };

            ReadingMode mode = ReadingMode::None;

            while (std::getline(ss, line)) {
                Trim(line);
                if (line.empty() || line[0] == '#')
                    continue;

                if (line == "[Attributes]") {
                    mode = ReadingMode::Attributes;
                    continue;
                }
                if (line == "[Vertices]") {
                    mode = ReadingMode::Vertices;
                    continue;
                }
                if (line == "[Indices]") {
                    mode = ReadingMode::Indices;
                    continue;
                }

                switch (mode) {
                    case ReadingMode::Attributes:
                        ParseAttribute(line);
                        break;
                    case ReadingMode::Vertices:
                        ParseVertex(line);
                        break;
                    case ReadingMode::Indices:
                        ParseIndex(line);
                        break;
                    default:
                        break;
                }
            }

            return !vertices.empty();
        }

    private:
        static MeshAttribute::Type AttributeTypeFromString(const std::string &name) {
            if (name == "Position") return MeshAttribute::Position;
            if (name == "Normal") return MeshAttribute::Normal;
            if (name == "UV") return MeshAttribute::UV;
            if (name == "Color") return MeshAttribute::Color;
            throw std::runtime_error("Unknown attribute: " + name);
        }

        static void Trim(std::string &s) {
            s.erase(std::remove(s.begin(), s.end(), '\r'), s.end());
            while (!s.empty() && std::isspace((unsigned char)s.back()))
                s.pop_back();
            while (!s.empty() && std::isspace((unsigned char)s.front()))
                s.erase(s.begin());
        }

        void ParseAttribute(const std::string &line) {
            if (line.empty() || line[0] == '#') return;
            std::stringstream ls(line);
            std::string name;
            int size;
            if (ls >> name >> size)
                attributes.push_back({AttributeTypeFromString(name), size});
        }

        void ParseVertex(const std::string &line) {
            if (line.empty() || line[0] == '#') return;
            std::stringstream ls(line);
            BasicVertex v{};
            for (auto &attr : attributes) {
                switch (attr.type) {
                    case MeshAttribute::Position:
                        for (int i = 0; i < attr.size; ++i) ls >> v.position[i];
                        break;
                    case MeshAttribute::Normal:
                        for (int i = 0; i < attr.size; ++i) ls >> v.normal[i];
                        break;
                    case MeshAttribute::UV:
                        for (int i = 0; i < attr.size; ++i) ls >> v.uv[i];
                        break;
                    case MeshAttribute::Color:
                        for (int i = 0; i < attr.size; ++i) ls >> v.color[i];
                        break;
                }
            }
            vertices.push_back(v);
        }

        void ParseIndex(const std::string &line) {
            if (line.empty() || line[0] == '#') return;
            std::stringstream ls(line);
            uint32_t i;
            while (ls >> i)
                indices.push_back(i);
        }
    };
}
