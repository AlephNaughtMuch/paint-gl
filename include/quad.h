#ifndef QUAD_H
#define QUAD_H

class Quad {
    public:
        Quad();
        ~Quad();
        void draw();

        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
};

#endif
