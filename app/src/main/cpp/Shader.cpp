
#include "Shader.h"
#include "Log.h"
#include <GLES2/gl2.h>

//vertex shader glsl
#define GET_STR(x) #x
static const char *vertexShader = GET_STR(
        attribute vec4 aPosition; //vertex coord
        attribute vec2 aTexCoord; //texture coord
        varying vec2 vTexCoord;   //output texture coord
        void main(){
            vTexCoord = vec2(aTexCoord.x,1.0-aTexCoord.y);
            gl_Position = aPosition;
        }
);

//fragment shader, software and x86 decoding
static const char *fragYUV420P = GET_STR(
        precision mediump float;    //precision
        varying vec2 vTexCoord;     //vertex texture coord
        uniform sampler2D yTexture; //input texture
        uniform sampler2D uTexture;
        uniform sampler2D vTexture;
        void main(){
            vec3 yuv;
            vec3 rgb;
            yuv.r = texture2D(yTexture,vTexCoord).r;
            yuv.g = texture2D(uTexture,vTexCoord).r - 0.5;
            yuv.b = texture2D(vTexture,vTexCoord).r - 0.5;
            rgb = mat3(1.0,     1.0,    1.0,
                       0.0,-0.39465,2.03211,
                       1.13983,-0.58060,0.0)*yuv;
            //output pixel color
            gl_FragColor = vec4(rgb,1.0);
        }
);



static const char *fragNV12 = GET_STR(
        precision mediump float;
        varying vec2 vTexCoord;
        uniform sampler2D yTexture;
        uniform sampler2D uvTexture;
        void main(){
            vec3 yuv;
            vec3 rgb;
            yuv.r = texture2D(yTexture,vTexCoord).r;
            yuv.g = texture2D(uvTexture,vTexCoord).r - 0.5;
            yuv.b = texture2D(uvTexture,vTexCoord).a - 0.5;
            rgb = mat3(1.0,     1.0,    1.0,
                       0.0,-0.39465,2.03211,
                       1.13983,-0.58060,0.0)*yuv;

            gl_FragColor = vec4(rgb,1.0);
        }
);


static const char *fragNV21 = GET_STR(
        precision mediump float;
        varying vec2 vTexCoord;
        uniform sampler2D yTexture;
        uniform sampler2D uvTexture;
        void main(){
            vec3 yuv;
            vec3 rgb;
            yuv.r = texture2D(yTexture,vTexCoord).r;
            yuv.g = texture2D(uvTexture,vTexCoord).a - 0.5;
            yuv.b = texture2D(uvTexture,vTexCoord).r - 0.5;
            rgb = mat3(1.0,     1.0,    1.0,
                       0.0,-0.39465,2.03211,
                       1.13983,-0.58060,0.0)*yuv;

            gl_FragColor = vec4(rgb,1.0);
        }
);

static GLuint InitShader(const char *code,GLint type)
{

    //create shader
    GLuint sh = glCreateShader(type);
    if(sh == 0)
    {
        LOGE("glCreateShader %d failed!",type);
        return 0;
    }
    //load shader
    glShaderSource(sh,
                   1,    //number of shaders
                   &code, //shader code
                   0);   //length of code
    //compile
    glCompileShader(sh);

    //compile status
    GLint status;
    glGetShaderiv(sh,GL_COMPILE_STATUS,&status);
    if(status == 0)
    {
        LOGE("glCompileShader failed!");
        return 0;
    }
    LOGE("glCompileShader success!");
    return sh;
}

bool Shader::Init(ShaderType type)
{
    Close();
    mux.lock();
    //vertex shader/fragment initialization
    //vertex shader init
    vsh = InitShader(vertexShader,GL_VERTEX_SHADER);
    if(vsh == 0)
    {
        mux.unlock();
        LOGE("InitShader GL_VERTEX_SHADER failed!");
        return false;
    }
    LOGE("InitShader GL_VERTEX_SHADER success! %d",type);

    //yuv 420p fragment shader init
    switch (type)
    {
        case SHADER_YUV420P:
            fsh = InitShader(fragYUV420P,GL_FRAGMENT_SHADER);
            break;
        case SHADER_NV12:
            fsh = InitShader(fragNV12,GL_FRAGMENT_SHADER);
            break;
        case SHADER_NV21:
            fsh = InitShader(fragNV21,GL_FRAGMENT_SHADER);

            break;
        default:
            mux.unlock();
            LOGE("XSHADER format is error");
            return false;
    }

    if(fsh == 0)
    {
        mux.unlock();
        LOGE("InitShader GL_FRAGMENT_SHADER failed!");
        return false;
    }
    LOGE("InitShader GL_FRAGMENT_SHADER success!");


    /////////////////////////////////////////////////////////////
    //create rendring application
    program = glCreateProgram();
    if(program == 0)
    {
        mux.unlock();
        LOGE("glCreateProgram failed!");
        return false;
    }
    //adding shaders to the program
    glAttachShader(program,vsh);
    glAttachShader(program,fsh);

    //linking program
    glLinkProgram(program);
    GLint status = 0;
    glGetProgramiv(program,GL_LINK_STATUS,&status);
    if(status != GL_TRUE)
    {
        mux.unlock();
        LOGE("glLinkProgram failed!");
        return false;
    }
    glUseProgram(program);
    LOGE("glLinkProgram success!");
    /////////////////////////////////////////////////////////////


    //add 3d vertexs, 2 triangle makes a rectangle
    static float vers[] = {
            1.0f,-1.0f,0.0f,
            -1.0f,-1.0f,0.0f,
            1.0f,1.0f,0.0f,
            -1.0f,1.0f,0.0f,
    };
    GLuint apos = (GLuint)glGetAttribLocation(program,"aPosition");
    glEnableVertexAttribArray(apos);
    //passing in vertices
    glVertexAttribPointer(apos,3,GL_FLOAT,GL_FALSE,12,vers);

    //pass in texture vertices
    static float txts[] = {
            1.0f,0.0f ,
            0.0f,0.0f,
            1.0f,1.0f,
            0.0,1.0
    };
    GLuint atex = (GLuint)glGetAttribLocation(program,"aTexCoord");
    glEnableVertexAttribArray(atex);
    glVertexAttribPointer(atex,2,GL_FLOAT,GL_FALSE,8,txts);


    //texture uniform initialization
    //configuration
    glUniform1i( glGetUniformLocation(program,"yTexture"),0);
    switch (type) {
        case SHADER_YUV420P:
            glUniform1i(glGetUniformLocation(program, "uTexture"), 1);
            glUniform1i(glGetUniformLocation(program, "vTexture"), 2);
            break;
        case SHADER_NV21:
        case SHADER_NV12:
            glUniform1i(glGetUniformLocation(program, "uvTexture"), 1);
            break;
    }

    mux.unlock();
    LOGI("initializtion shader succeess");

    return true;
}


void Shader::Draw()
{
    mux.lock();
    if(!program)
    {
        mux.unlock();
        return;
    }


    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    mux.unlock();
}

void Shader::GetTexture(unsigned int index,int width,int height, unsigned char *buf,bool isa)
{

    unsigned int format =GL_LUMINANCE;
    if(isa)
        format = GL_LUMINANCE_ALPHA;
    mux.lock();
    if(texts[index] == 0)
    {
        //texture init
        glGenTextures(1,&texts[index]);

        //configure textrure properties
        glBindTexture(GL_TEXTURE_2D,texts[index]);
        //zoom out
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     format,
                     width,height,
                     0,
                     format, //must be the same as internalformat
                     GL_UNSIGNED_BYTE,
                     NULL
        );
    }

    mux.unlock();
    //initialize first level texture, bind it to opengl
    glActiveTexture(GL_TEXTURE0+index);
    glBindTexture(GL_TEXTURE_2D,texts[index]);
    glTexSubImage2D(GL_TEXTURE_2D,0,0,0,width,height,format,GL_UNSIGNED_BYTE,buf);
    mux.unlock();
}

void Shader::Close()
{
    mux.lock();

    if(program)
        glDeleteProgram(program);
    if(fsh)
        glDeleteShader(fsh);
    if(vsh)
        glDeleteShader(vsh);

    for(int i = 0; i < sizeof(texts)/sizeof(unsigned int); i++)
    {
        if(texts[i])
        {
            glDeleteTextures(1,&texts[i]);
        }
        texts[i] = 0;
    }

    mux.unlock();
}

