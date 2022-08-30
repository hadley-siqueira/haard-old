#ifndef HAARD_IR_H
#define HAARD_IR_H

namespace haard {
    class IR {
        public:
            virtual ~IR();

        public:
            void set_kind(int kind);
            int get_kind();

        private:
            int kind;
    };
}

#endif
