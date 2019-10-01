#include <iostream>

#include "utils.h"


Rect::Rect():SDL_Rect(){}
Rect::Rect(int _x,int _y, int _w,int _h){
	x =_x;
	y = _y;
	w = _w;
	h = _h;
}
bool Rect::hitTest( const SDL_Rect &r){
	return false;
}
bool Rect::hitTest( const Rect &r){
	return false;
}
bool Rect::hitTest( const SDL_Point &p){
	return hitTest(p.x, p.y);
}
bool Rect::hitTest( int _x, int _y){
	if( _x < x){
		return false;
	}
	if( _x > x+w){
		return false;
	}
	if( _y < y){
		return false;
	}
	if( _y > y+h){
		return false;
	}
	return true;
}

int App::getWindowHeight() const
{
    return this->WindowHeight;
}

int App::getDisplayWidth() const
{
    return this->DisplayWidth;
}


int App::getDisplayHeight() const
{
    return this->DisplayHeight;
}

App::App():is_opengl(false)
{
    
}

App::~App()
{
    
}

int App::getWindowWidth() const
{
    return this->WindowWidth;
}

SDL_Renderer *App::getRenderer()
{
    return this->renderer;
}

SDL_Window *App::getWindow()
{
    return this->window;
}

bool App::init(int WindowWidth, int WindowHeight)
{
    this->WindowWidth = WindowWidth;
    this->WindowHeight = WindowHeight;
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return false;
    }

    SDL_DisplayMode displayMode;
    if( SDL_GetCurrentDisplayMode( 0, &displayMode ) == 0 )
    {
        this->DisplayWidth = displayMode.w;
        this->DisplayHeight = displayMode.h;
    }
    if( (this->WindowWidth == -1) || (this->WindowHeight == -1) ){
        this->WindowWidth = this->DisplayWidth;
        this->WindowHeight = this->DisplayHeight;
    }
    this->window = SDL_CreateWindow( "Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->WindowWidth, this->WindowHeight, SDL_WINDOW_SHOWN );
    if(this->window == NULL){
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        return false;
    }
    this->renderer = SDL_CreateRenderer( this->window, -1, SDL_RENDERER_ACCELERATED );
    if(this->renderer == NULL){
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        return false;
    }

    return true;
}
bool App::init_opengl(int WindowWidth, int WindowHeight)
{
    this->WindowWidth = WindowWidth;
    this->WindowHeight = WindowHeight;
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cout<< "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_DisplayMode displayMode;
    if( SDL_GetCurrentDisplayMode( 0, &displayMode ) == 0 )
    {
        this->DisplayWidth = displayMode.w;
        this->DisplayHeight = displayMode.h;
    }
    if( (this->WindowWidth == -1) || (this->WindowHeight == -1) ){
        this->WindowWidth = this->DisplayWidth;
        this->WindowHeight = this->DisplayHeight;
    }

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );

    this->window = SDL_CreateWindow( "Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->WindowWidth, this->WindowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
    if(this->window == NULL){
        std::cout<< "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    gContext = SDL_GL_CreateContext( this->window );
    if( gContext == NULL )
    {
       std::cout<< "OpenGL context could not be created! SDL Error: " << SDL_GetError() << std::endl;
       return false;
    }
    //Use Vsync
    if( SDL_GL_SetSwapInterval( 1 ) < 0 )
    {
        std::cout<< "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << std::endl;
    }
    GLenum error = GL_NO_ERROR;

    //Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        std::cout<< "Error initializing OpenGL! "<< (int)error << std::endl;
        return false;
    }

    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        std::cout<< "Error initializing OpenGL! "<< (int)error << std::endl;
        return false;
    }

    glClearColor( 0.f, 0.f, 0.f, 1.f );

    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        std::cout<< "Error initializing OpenGL! "<< (int)error << std::endl;
        return false;
    }

    this->is_opengl = true;
    return true;
}

void App::quit()
{
    SDL_DestroyRenderer( this->renderer );
    SDL_DestroyWindow( this->window );
    SDL_Quit();
}

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

Color::Color(uint8_t r, uint8_t g, uint8_t b)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = 255;
}

EventManager::EventManager():is_close(false)
{

}

EventManager::~EventManager()
{

}

void EventManager::update()
{
    if( SDL_PollEvent( &this->last_event ) == 0)
        return;

    this->event_processed = false;
    if( this->last_event.type == SDL_QUIT){
        this->is_close = true;
        this->event_processed = true;
    }
    /*
    if( e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED ){
        //Get screen dimensions
        //int w = e.window.data1;
        //int h = e.window.data2;

        //Update screen
        //SDL_RenderPresent( gRenderer );
    }
    */
}

bool EventManager::isClose()
{
    return this->is_close;
}

bool EventManager::get_mouse(Point& p)
{
    if(this->event_processed){
        return false;
    }
    if(this->last_event.type == SDL_MOUSEBUTTONDOWN){
        SDL_GetMouseState(&p.x, &p.y);
        this->event_processed = true;
        return true;
    }
    return false;
}

Point::Point(int _x, int _y)
{
    this->x = _x;
    this->y = _y;
}


bool TextureManager::load_tex(const std::string &name){
    SDL_Surface *surf = NULL;
    surf = SDL_LoadBMP( name.c_str() );
    if(!surf)
        return false;
    if(surf->format->BitsPerPixel != 24){
        std::cout << "load tex error: "<< (int)surf->format->BitsPerPixel <<std::endl;
        return false;
    }
    surf->format->Amask = 0xFF000000;
    surf->format->Ashift = 24;
    int res = SDL_SetColorKey( surf, true, SDL_MapRGB(surf->format, 255,255,255) );
    if( res != 0){
        std::cout << "Unable to add alpha" << std::endl;
    }
    SDL_Surface *surf_w_a = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_RGBA32, 0);
    if(surf_w_a == NULL){
        std::cout << "unable convert surf" << std::endl;
    }
    std::cout << (int)surf_w_a->format->BitsPerPixel << std::endl;
    GLuint texid;
    glGenTextures( 1, &texid );
    glBindTexture( GL_TEXTURE_2D, texid );

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, surf->w, surf->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surf_w_a->pixels );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glBindTexture( GL_TEXTURE_2D, 0 );

    textures.insert(std::make_pair(name, texid));

    return true;
}

void TextureManager::draw_rect(const Rect &dst, const Color &c){
    glColor3d(c.r, c.g, c.b);
    glBegin( GL_QUADS );
        glVertex2i( dst.x,          dst.y );
        glVertex2i( dst.x+dst.w,    dst.y );
        glVertex2i( dst.x+dst.w,    dst.y+dst.h );
        glVertex2i( dst.x,          dst.y+dst.h );
    glEnd();
}

void TextureManager::draw_frame(const std::string &name, const Rect &src, const Rect &dst){
    glBindTexture( GL_TEXTURE_2D, textures.at(name) );
    glBegin( GL_QUADS );
        glTexCoord2f(0,0); glVertex2i( dst.x, dst.y );
        glTexCoord2f(1,0); glVertex2i( dst.x+dst.w, dst.y );
        glTexCoord2f(1,1); glVertex2i( dst.x+dst.w, dst.y+dst.h );
        glTexCoord2f(0,1); glVertex2i( dst.x, dst.y+dst.h );
    glEnd();
    glBindTexture( GL_TEXTURE_2D, 0 );
}

GLuint TextureManager::get_id(const std::string &name){
    return textures.at(name);
}
void TextureManager::init(){
    glEnable( GL_TEXTURE_2D );
}
