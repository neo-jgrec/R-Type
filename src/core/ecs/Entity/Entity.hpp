
namespace core {
    namespace ecs {
        class Entity {
            private:
                size_t id;

            public:
                explicit Entity(size_t id) : id(id) {}
                operator size_t() const { return id; }
        };
    }
}
