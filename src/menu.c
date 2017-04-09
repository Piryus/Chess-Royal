#include "defs.h"

void menu(void)
{
    SDL_Window *window=NULL;
    SDL_Renderer *renderer=NULL;
    SDL_Event event;
    int continuer=1;
    init(&window,&renderer);
    render_menu_background(renderer);
    render_game_title(renderer);
    render_menu_buttons(renderer);
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
                if(NULL!=menu)
                    SDL_DestroyTexture(menu);
                SDL_RenderClear(renderer);
                game(renderer);
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
        SDL_SetRenderDrawColor(renderer, 255, 174, 0, 255);
        SDL_RenderDrawRect(renderer, &menu_tile[i]);
        SDL_RenderPresent(renderer);
    }
}

void render_menu_background(SDL_Renderer *renderer)
{
    SDL_Texture *menu=NULL;
    menu=loadIMG("sprites/bg_menu.jpg",renderer);
    RendTex(menu,renderer,0,0);
    SDL_RenderPresent(renderer);
}

void render_game_title(SDL_Renderer *renderer)
{
    SDL_Texture *title=NULL;
    SDL_Rect font_rect;
    TTF_Font *font_fipps=NULL;
    font_fipps=TTF_OpenFont("ttf/Fipps-Regular.ttf",40);
    if(!font_fipps)
        printf("Erreur TTF_OpenFont: %s\n", TTF_GetError());
    title=loadFont_Blended(renderer, font_fipps,"Chess Royal Alpha", 255, 144, 0);
    SDL_QueryTexture(title,NULL,NULL,&font_rect.w,&font_rect.h);
    RendTex(title,renderer,(WINDOW_WIDTH-font_rect.w)/2,(WINDOW_HEIGHT-font_rect.h)/50);
    SDL_RenderPresent(renderer);
}
