#include "defs.h"

int main(int argc, char *args[])
{
    SDL_Window *g_Window=NULL;
    SDL_Renderer *g_Renderer=NULL;
    SDL_Texture *menu=NULL, *title=NULL;
    SDL_Rect menu_tile[NB_MENU];
    SDL_Rect rect;
    SDL_Event event;
    TTF_Font *fipps=NULL;
    int statut = EXIT_FAILURE;
    int continuer=1;
    init(&g_Window,&g_Renderer);
    menu=loadIMG("sprites/bg_menu.jpg",g_Renderer);
    if(menu==NULL)
    {
        goto Quit;
    }
    if(0!=RendTex(menu,g_Renderer,0,0))
    {
        goto Quit;
    }
    SDL_RenderPresent(g_Renderer);
    fipps=TTF_OpenFont("ttf/Fipps-Regular.ttf",40);
    if(!fipps)
        printf("Erreur TTF_OpenFont: %s\n", TTF_GetError());
    title=loadFont_Blended(g_Renderer, fipps,"Chess Royal Alpha", 255, 144, 0);
    if(0!=SDL_QueryTexture(title,NULL,NULL,&rect.w,&rect.h))
    {
        printf("Erreur SDL_QueryTexture : %s\n",SDL_GetError());
        return -1;
    }
    RendTex(title,g_Renderer,(WINDOW_WIDTH-rect.w)/2,(WINDOW_HEIGHT-rect.h)/50);
    SDL_RenderPresent(g_Renderer);
    for(int i=0; i<=(NB_MENU-1); i++)
    {
        menu_tile[i].h=WINDOW_HEIGHT/(NB_MENU*3);
        menu_tile[i].w=WINDOW_WIDTH/2;
        menu_tile[i].x=(WINDOW_WIDTH-menu_tile[i].w)/2;
        menu_tile[i].y=WINDOW_HEIGHT/4+(menu_tile[i].h+MENU_SPACING)*i;
        SDL_SetRenderDrawColor(g_Renderer, 255, 174, 0, 255);
        SDL_RenderDrawRect(g_Renderer, &menu_tile[i]);
        SDL_RenderPresent(g_Renderer);
    }
    //Boucle à vérifier
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
                SDL_RenderClear(g_Renderer);
                jeu(g_Window,g_Renderer);
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
    statut = EXIT_SUCCESS;
Quit:
    if(NULL!=menu)
        SDL_DestroyTexture(menu);
    if(NULL != g_Renderer)
        SDL_DestroyRenderer(g_Renderer);
    if(NULL != g_Window)
        SDL_DestroyWindow(g_Window);
    SDL_Quit();
    return statut;
}
