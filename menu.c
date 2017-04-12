#include "defs.h"

void menu(void)
{
    Square square[8][8];
    SDL_Window *window=NULL;
    SDL_Renderer *renderer=NULL;
    SDL_Event event;
    int continuer=1;
    init(&window,&renderer);
    render_menu_background(renderer,square);
    render_game_title(renderer);
    render_menu_buttons(renderer);
    render_menu_buttons_text(renderer);
    while(continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            continuer=0;
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer=0;
                break;
            case SDLK_1:
                //if(NULL!=menu)
                //    SDL_DestroyTexture(menu);
                SDL_RenderClear(renderer);
                game(renderer,square,0); // 0 = sans ia
                continuer=0;
                break;
            case SDLK_2:
                continuer=0;
                break;
            default:
                break;
            }
            break;
        }
    }
}

void render_menu_buttons(SDL_Renderer *renderer)
{
    SDL_Rect menu_tile[NB_MENU_BUTTONS];
    for(int i=0; i<=(NB_MENU_BUTTONS-1); i++)
    {
        menu_tile[i].h=WINDOW_HEIGHT/(NB_MENU_BUTTONS*3);
        menu_tile[i].w=WINDOW_WIDTH/2;
        menu_tile[i].x=(WINDOW_WIDTH-menu_tile[i].w)/2;
        menu_tile[i].y=WINDOW_HEIGHT/4+(menu_tile[i].h+MENU_SPACING)*i;
        SDL_SetRenderDrawColor(renderer, 43, 120, 255, 255);
        SDL_RenderDrawRect(renderer, &menu_tile[i]);
        SDL_RenderPresent(renderer);
    }
}

void render_menu_buttons_text(SDL_Renderer *renderer)
{
    SDL_Texture *button_text=NULL;
    SDL_Rect font_rect;
    TTF_Font *font_ConcertOne=NULL;
    font_ConcertOne=TTF_OpenFont("ttf/ConcertOne-Regular.ttf",40);
    button_text=loadFont_Blended(renderer, font_ConcertOne,"Jouer contre l'IA", 255, 255, 255);
    SDL_QueryTexture(button_text,NULL,NULL,&font_rect.w,&font_rect.h);
    RendTex(button_text,renderer,(WINDOW_WIDTH-font_rect.w)/2,WINDOW_HEIGHT/4+font_rect.h/2);
    SDL_RenderPresent(renderer);
}

void render_menu_background(SDL_Renderer *renderer,Square square[][8])
{
    SDL_Rect background= {0,0,WINDOW_WIDTH,WINDOW_HEIGHT};
    render_background(renderer);
    render_base(renderer);
    render_squares(renderer);
    init_random_pawns_pos(square);
    render_pawns(renderer,square);
    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_RenderFillRect(renderer, &background);
    SDL_RenderPresent(renderer);
}

void render_game_title(SDL_Renderer *renderer)
{
    SDL_Texture *title=NULL;
    SDL_Rect font_rect;
    TTF_Font *font_fipps=NULL;
    font_fipps=TTF_OpenFont("ttf/Fipps-Regular.ttf",40);
    title=loadFont_Blended(renderer, font_fipps,"Chess Royal Alpha", 255, 144, 0);
    SDL_QueryTexture(title,NULL,NULL,&font_rect.w,&font_rect.h);
    RendTex(title,renderer,(WINDOW_WIDTH-font_rect.w)/2,(WINDOW_HEIGHT-font_rect.h)/50);
    SDL_RenderPresent(renderer);
}

void init_random_pawns_pos(Square square[][8])
{
    srand(time(NULL));
    for(int i=0; i<=7; i++)
    {
        for(int j=0; j<=7; j++)
        {
            square[i][j].pawn=rand()%3-1;
        }
    }
}
