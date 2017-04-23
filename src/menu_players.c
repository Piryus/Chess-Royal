#include "defs.h"

void select_players_menu(SDL_Renderer *renderer, int isAIGame, int *quit, int *IDPlayerB, int *IDPlayerW)
{
    Square square[8][8];
    SDL_Event event;
    int stop = 0;
    int adder=0;
    SDL_Rect newPlayerButton = {(WINDOW_WIDTH - BUTTON_WIDTH) / 2, 20, BUTTON_WIDTH, BUTTON_HEIGHT};
    SDL_Rect playerButton = {(WINDOW_WIDTH - BUTTON_WIDTH) / 2, 0, BUTTON_WIDTH, BUTTON_HEIGHT};
    SDL_Rect goBackButton= {30,15,BUTTON_WIDTH-150,BUTTON_HEIGHT};
    int nbPlayers = 2-isAIGame;
    int selectedPlayerNb=0;
    while((selectedPlayerNb!=nbPlayers)&&(*quit!=-1))
    {
        SDL_RenderClear(renderer);
        render_menu_background(renderer, square);
        render_playersList(renderer,adder);
        newPlayerButton.y=20+adder;
        goBackButton.y=15+adder;
        render_button(renderer, "Nouveau joueur", newPlayerButton);
        render_button(renderer,"Retour", goBackButton);
        if(selectedPlayerNb==0)
        {
            renderCenteredText(renderer,"Sélectionnez le joueur blanc :", WINDOW_WIDTH/2, 200+adder,255,255,255,60);
        }
        else if(selectedPlayerNb==1)
        {
            renderCenteredText(renderer,"Sélectionnez le joueur noir :", WINDOW_WIDTH/2, 200+adder,255,255,255,60);
        }
        stop=0;
        while(!stop)
        {
            SDL_WaitEvent(&event);
            switch(event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                if(isCursorOnButton(newPlayerButton))
                {
                    render_newPlayerMenu(renderer);
                    stop=1;
                }
                else if(isCursorOnButton(goBackButton))
                {
                    stop=1;
                    *quit=-1;
                }
                for(int i=0; i<nbJoueur(); i++)
                {
                    playerButton.y = 300 + (BUTTON_HEIGHT + 15) * i +adder;
                    if(isCursorOnButton(playerButton))
                    {
                        selectPlayer(i,selectedPlayerNb,IDPlayerB,IDPlayerW);
                        selectedPlayerNb++;
                        stop=1;
                    }
                }
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                case SDLK_UP:
                    if(adder<=-100)
                    {
                        adder=adder+100;
                        SDL_RenderClear(renderer);
                        render_menu_background(renderer, square);
                        render_playersList(renderer,adder);
                        newPlayerButton.y=20+adder;
                        goBackButton.y=15+adder;
                        render_button(renderer, "Nouveau joueur", newPlayerButton);
                        render_button(renderer,"Retour", goBackButton);
                        if(selectedPlayerNb==0)
                        {
                            renderCenteredText(renderer,"Sélectionnez le joueur blanc :", WINDOW_WIDTH/2, 200+adder,255,255,255,60);
                        }
                        else if(selectedPlayerNb==1)
                        {
                            renderCenteredText(renderer,"Sélectionnez le joueur noir :", WINDOW_WIDTH/2, 200+adder,255,255,255,60);
                        }
                    }
                    break;
                case SDLK_DOWN:
                    adder=adder-100;
                    SDL_RenderClear(renderer);
                    render_menu_background(renderer, square);
                    render_playersList(renderer,adder);
                    newPlayerButton.y=20+adder;
                    goBackButton.y=15+adder;
                    render_button(renderer, "Nouveau joueur", newPlayerButton);
                    render_button(renderer,"Retour", goBackButton);
                    if(selectedPlayerNb==0)
                    {
                        renderCenteredText(renderer,"Sélectionnez le joueur blanc :", WINDOW_WIDTH/2, 200+adder,255,255,255,60);
                    }
                    else if(selectedPlayerNb==1)
                    {
                        renderCenteredText(renderer,"Sélectionnez le joueur noir :", WINDOW_WIDTH/2, 200+adder,255,255,255,60);
                    }
                    break;
                }
            }
        }
    }
}

void render_playersList(SDL_Renderer *renderer, int adder)
{
    SDL_Rect playerButton = {(WINDOW_WIDTH - BUTTON_WIDTH) / 2, 0, BUTTON_WIDTH, BUTTON_HEIGHT};
    Joueur players[nbJoueur()];
    listerJoueur(players);
    for(int i = 0; i < nbJoueur(); i++)
    {
        playerButton.y = 300 + (BUTTON_HEIGHT + 15) * i +adder;
        render_button(renderer, players[i].Nom, playerButton);
    }
}

void render_newPlayerMenu(SDL_Renderer *renderer)
{
    SDL_Event event;
    int stop = 0;
    Square square[8][8];
    char name[15];
    name[0]='\0';
    SDL_Rect cancelButton = {WINDOW_WIDTH / 2 - 500, 700, BUTTON_WIDTH, BUTTON_HEIGHT};
    SDL_Rect createPlayerButton = {WINDOW_WIDTH / 2 + (500 - BUTTON_WIDTH), 700, BUTTON_WIDTH, BUTTON_HEIGHT};
    SDL_Rect textArea = {WINDOW_WIDTH / 2 - 450, (WINDOW_HEIGHT - 100) / 2, 900, 100};
    SDL_RenderClear(renderer);
    render_menu_background(renderer, square);
    render_button(renderer, "Annuler", cancelButton);
    render_button(renderer, "Créer", createPlayerButton);
    renderFillRect(renderer, textArea, 255, 255, 255, 255);
    SDL_StartTextInput();
    while(!stop)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_MOUSEBUTTONDOWN:
            printf("%s", name);
            if(isCursorOnButton(cancelButton))
            {
                stop = 1;
            }
            else if((isCursorOnButton(createPlayerButton))&&(strlen(name)>3))
            {
                nouveauJoueur(name);
                stop = 1;
            }
            break;
        case SDL_TEXTINPUT:
            if(strlen(name)<=15)
            {
                strcat(name, event.text.text);
                renderFillRect(renderer, textArea, 255, 255, 255, 255);
                renderTextInRect(renderer,name,textArea);
            }
            break;
        case SDL_KEYDOWN:
            if((event.key.keysym.sym==SDLK_BACKSPACE)&&(strlen(name)>0))
            {
                name[strlen(name)-1]='\0';
                renderFillRect(renderer, textArea, 255, 255, 255, 255);
                if(strlen(name)!=0)
                {
                    renderTextInRect(renderer,name,textArea);
                }
            }
        }
    }
    SDL_StopTextInput();
}

void selectPlayer(int ID, int selectedPlayerNb, int *IDPlayerB, int *IDPlayerW)
{
    if(selectedPlayerNb==0)
    {
        *IDPlayerW=ID;
    }
    if(selectedPlayerNb==1)
    {
        *IDPlayerB=ID;
    }
}
