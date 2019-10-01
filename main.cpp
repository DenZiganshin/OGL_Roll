#include<iostream>
#include<vector>
#include<map>
#include<string>

#include "utils.h"

static float c_default_speed = 30.0f;
static int c_roll_size = 30.0f;
static int c_roll_cnt = 3;
static float c_slowdown = 2.0f;
static int c_number_cnt = 7;
static int c_visible_number_cnt = 3;

std::map<int , Color> colors = {
    {0,   Color(0,255,0)},
    {1,   Color(255,0,0)},
    {2,   Color(0,0,255)},
    {3,   Color(0,255,255)},
    {4,   Color(255,0,255)},
    {5,   Color(255,255,0)},
    {6,   Color(255,255,255)},
    {7,   Color(0,0,0)},
    {8,   Color(78,22,235)}
};

class Roll{
public:
    enum class RollState{
        ROLL,
        STOP,
        STOP_FIRST_PHASE,
        STOP_SECOND_PHASE
    };
private:
    Point top_pos;
    std::vector<int> numbers;
    std::vector<int> draw_numbers;
    Rect frame;
    float offset;
    float speed;
    RollState state;
private:
    int numbers_next(){
        static int last_number = 0;
        if(++last_number >= numbers.size()){
            last_number = 0;
        }
        return last_number;
    }
public:
    Roll();
    Roll(uint32_t x, uint32_t y):top_pos(x,y), numbers(c_number_cnt), draw_numbers(c_visible_number_cnt+1),
        frame(0,0,c_roll_size, c_roll_size), offset(0.0f), speed(c_default_speed), state(RollState::ROLL){
        //this->additional_rects = add;
        for(int i=0; i<numbers.size(); i++){
            numbers[i] = i;
        }
        for(int i=0; i<draw_numbers.size(); i++){
            draw_numbers[i] = numbers_next();
        }
    }
    void update(){
        if(this->state == RollState::STOP){
            return;
        }
        //move down
        this->offset += this->speed;
        if( this->state == RollState::STOP_FIRST_PHASE ){
            this->speed -= c_slowdown;
        }
        if( (this->state == RollState::STOP_FIRST_PHASE) && (this->speed <= c_slowdown)){
            this->speed = c_slowdown;
            this->state = RollState::STOP_SECOND_PHASE;
        }
        if(this->offset >= this->frame.h){
            for(int i=draw_numbers.size()-1; i>0; i--){
                draw_numbers[i] = draw_numbers[i-1];
            }
            draw_numbers[0] = numbers_next();
            this->offset = 0;
            if(this->state == RollState::STOP_FIRST_PHASE){
                this->state = RollState::STOP;
            }
        }
	}
    Roll::RollState isRoll(){
        return this->state;
    }
    void play(){
        this->state = RollState::ROLL;
        this->speed = c_default_speed;
    }
    void stop(){
        this->state = RollState::STOP_FIRST_PHASE;
    }

    std::vector<int> get();
    void draw(){
        for(int i=0; i<this->draw_numbers.size(); i++){
            //do or append
            int px = this->top_pos.x,
                py = this->top_pos.y + (i*this->frame.h) + this->offset;
            TextureManager::inst().draw_rect( Rect(px, py, this->frame.w, this->frame.h),
                                      colors.at(this->draw_numbers[i]) );

        }
	}
};


int main(int argc, char* argv[]){
    App& app = App::instance();
    EventManager& event_manager = EventManager::instance();

    app.init_opengl(-1, -1);

    c_roll_size = app.getDisplayHeight() / 5;

    TextureManager::inst().init();

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0, app.getWindowWidth(), app.getWindowHeight(), 0.0, 1.0, -1.0 );

    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    uint32_t posx = app.getWindowWidth()/2 - c_roll_size*c_roll_cnt/2;
    //uint32_t posy = app.getWindowHeight()/2 - c_roll_size*c_roll_cnt/2;
    std::vector<Roll> rolls = {
        {posx, 0},
        {posx+c_roll_size, 0},
        {posx+c_roll_size*2, 0}
    };

    Point p(0,0);

    while(! event_manager.isClose() ){
        event_manager.update();

        glClear( GL_COLOR_BUFFER_BIT );

        if(event_manager.get_mouse(p)){
            for( auto &r : rolls){
                if(r.isRoll() == Roll::RollState::ROLL)
                    r.stop();
                else
                    r.play();
            }
        }

        for( auto &r : rolls){
            r.update();
            r.draw();
        }


        TextureManager::inst().draw_rect(Rect(0, 0, app.getDisplayWidth(), c_roll_size), Color(0,0,0) );
        TextureManager::inst().draw_rect(Rect(0, app.getDisplayHeight() - c_roll_size, app.getDisplayWidth(),c_roll_size), Color(0,0,0) );

        SDL_Delay(10);

        SDL_GL_SwapWindow( app.getWindow() );
    }

    app.quit();
    return EXIT_SUCCESS;
}
