#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>


#include <string>
#include <map>

    
    
class App{
    SDL_Renderer* renderer;
    SDL_Window* window;
    SDL_GLContext gContext;
    bool is_opengl;
    int WindowWidth, WindowHeight;
    int DisplayWidth, DisplayHeight;
private:
    App(App const&) = delete;             // Copy construct
    App(App&&) = delete;                  // Move construct
    App& operator=(App const&) = delete;  // Copy assign
    App& operator=(App &&) = delete;      // Move assign
protected:
    App();
    ~App();
public:
    static App& instance(){
        static App inst;
        return inst;
    }
    int getWindowWidth() const;
    SDL_Renderer *getRenderer();
    SDL_Window *getWindow();
    bool init(int WindowWidth, int WindowHeight);
    bool init_opengl(int WindowWidth, int WindowHeight);
    void quit();
    int getWindowHeight() const;
    int getDisplayWidth() const;
    int getDisplayHeight() const;
};


struct Point : public SDL_Point{
    Point();
    Point(int _x,int _y);
};

/*
* 	расширение SDL_Rect
*/
struct Rect : public SDL_Rect {
    Rect();
    Rect(int _x,int _y, int _w,int _h);
    bool hitTest( const SDL_Rect &r);
    bool hitTest( const Rect &r);
    
    bool hitTest( const SDL_Point &p);
    bool hitTest( int _x, int _y);
};

class Color : public SDL_Color{
public:
    uint8_t r,g,b,a;
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    Color(uint8_t r, uint8_t g, uint8_t b);
};

//event cycle
class EventManager{
    bool is_close;
    bool event_processed;
    SDL_Event last_event;
private:
    EventManager(EventManager const&) = delete;             // Copy construct
    EventManager(EventManager&&) = delete;                  // Move construct
    EventManager& operator=(EventManager const&) = delete;  // Copy assign
    EventManager& operator=(EventManager &&) = delete;      // Move assign
protected:
    EventManager();
    ~EventManager();
public:
    static EventManager& instance(){
        static EventManager e;
        return e;
    }
    void update();
    bool isClose();
    void get_keyboard();
    bool get_mouse(Point& p);
    void get_touch();
};

class TextureManager{
    std::map<std::string, GLuint> textures;

    TextureManager(TextureManager const &) = delete;
    TextureManager(TextureManager&&) = delete;
    TextureManager& operator=(TextureManager const&) = delete;
    TextureManager& operator=(TextureManager const&&) = delete;
protected:
    TextureManager(){
    }
    ~TextureManager(){

    }
public:
    static TextureManager& inst(){
        static TextureManager tm;
        return tm;
    }
    void init();
    GLuint get_id(const std::string &name);
    void draw_frame(const std::string &name, const Rect &src, const Rect &dst);
    void draw_rect(const Rect &dst, const Color &c);
    bool load_tex(const std::string &name);
};



#endif // UTILS_H
