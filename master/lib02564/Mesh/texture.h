#ifndef TEXTURE_H
#define TEXTURE_H

#include <GLGraphics/GLHeader.h>
#include <string>
#include <CGLA/Vec3f.h>


namespace Mesh
{
    /// A simple texture map class.

    class Texture
    {
    public:

        // Constuctors/destructor
        Texture():
            id(0),
            target(GL_TEXTURE_2D),
            name("tex"),
            data(std::vector<CGLA::Vec3f>(1)),
            width(1),
            height(1)
        {}

        Texture(const std::string& _name, GLenum target, unsigned int width, unsigned int height, std::vector<CGLA::Vec3f> & data):
            id(0),
            name(_name),
            target(target),
            data(data),
            width(width),
            height(height)
        {}


        /**
         * Use this constructor for a texture generated via a buffer or a fbo, so no data needs to be loaded.
         * So the texture wraps the data that can be sent to shader via a material.
         */
        Texture(const std::string& _name, GLint id, GLenum target):
            id(id),
            name(_name),
            target(target),
            data(std::vector<CGLA::Vec3f>(1)),
            width(1),
            height(1)
        {}


        ~Texture()
        {}

        // get the texture name.
        const std::string& get_name() const {return name;}

        // Initializes the texture wrt OpenGL.
        virtual void init();

        // Reloads data into GPU memory with the new data provided.
        virtual void reloadData(std::vector<CGLA::Vec3f> & data,int w,int h);

        // binds the data of the texutre
        virtual void bind();

        virtual std::vector<CGLA::Vec3f> getData();

        GLuint get_id()
        {
            if(id == 0)
            {
                init();
            }
            return id;
        }

        void set_id(GLuint id)
        {
            this->id = id;
        }

        GLenum get_target()
        {
            return target;
        }

        void set_name(std::string & name)
        {
            this->name = name;
        }


        unsigned int width;
        unsigned int height;

    protected:
        void load();
        GLuint id;
        GLenum target;
        std::string name;
        std::vector<CGLA::Vec3f> data;
    };

}

#endif // TEXTURE_H
