#include "defs.h"

void select_players(SDL_Renderer *renderer, int isAIGame, Game *partie)
{
    Square square[8][8];
    SDL_Event event;
    int stop=0;
    SDL_Rect newPlayerButton= {(WINDOW_WIDTH-BUTTON_WIDTH)/2,20,BUTTON_WIDTH,BUTTON_HEIGHT};
    int nbPlayers=isAIGame+1;
    SDL_RenderClear(renderer);
    render_menu_background(renderer,square);
    render_playersList(renderer);
    render_button(renderer,"Nouveau joueur",newPlayerButton);
    while(!stop)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_KEYDOWN:
            stop=1;
            break;
        }
    }
}

void render_playersList(SDL_Renderer *renderer)
{
    SDL_Rect playerButton= {(WINDOW_WIDTH-BUTTON_WIDTH)/2,100,BUTTON_WIDTH,BUTTON_HEIGHT};
    char playerName[100];
    Joueur players[nbJoueur()];
    listerJoueur(players);
    for(int i=0; i<nbJoueur(); i++)
    {
        sprintf(playerName,players[i].Nom);
        playerButton.y=playerButton.y+BUTTON_WIDTH+15;
        render_button(renderer,playerName,playerButton);
    }
}

void render_newPlayerMenu(SDL_Renderer *renderer)
{
    SDL_Rect cancelButton= {WINDOW_WIDTH/2-BUTTON_WIDTH,400,BUTTON_WIDTH,BUTTON_HEIGHT};
    SDL_RenderClear(renderer);
    render_button(renderer,"Annuler",cancelButton);
    render_button(renderer,"Créer",cancelButton);
}
