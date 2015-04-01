#include "graphics.h"
#include <stdlib.h>

#define MAP_W 40
#define MAP_H 30
const int GCOLOR[] = {DARKGRAY, GREEN, RED};

int gw, gh;

struct SNAKE {
    int dir, head, inc, tail;
    int pool[MAP_W* MAP_H];
} game;

inline void drawAt( const int &i ) {
    int x = ( i % MAP_W ) * gw, y = ( i / MAP_W ) * gh;
    setfillcolor( GCOLOR[game.pool[i] >> 16] );
    bar( x, y, x + gw, y + gh );
}

void newFruit( void ) {
    int nf;
    while ( game.pool[nf = random( MAP_W * MAP_H )] >> 16 );
    game.pool[nf] = 0x20000, drawAt( nf );
}

int moveSnake( const int dx, const int dy, const bool u = false ) {
    if ( u && dx + ( game.dir & 3 ) == 1 && dy + ( game.dir >> 2 ) == 1 ) return 1;
    int nh;
    if ( dx && !dy ) {
        nh = game.head % MAP_W + dx;
        if ( nh < 0 || nh >= MAP_W ) return 0;
        nh = game.head + dx;
    } else {
        nh = game.head / MAP_W + dy;
        if ( nh < 0 || nh >= MAP_H ) return 0;
        nh = game.head + dy * MAP_W;
    }
    int s = game.pool[nh] >> 16;
    if ( s == 1 ) return 0;
    if ( s == 2 ) game.inc += 5, newFruit();
    if ( game.inc > 0 ) --game.inc;
    else {
        game.tail = game.pool[s = game.tail] & 0xffff;
        game.pool[s] = 0, drawAt( s );
    }
    game.pool[game.head] |= nh;
    game.pool[game.head = nh] = 0x10000, drawAt( nh );
    game.dir = ( dx + 1 ) | ( ( dy + 1 ) << 2 );
    return 1;
}

void gameInit( void ) {
    int data[] = {6, 0, 2, 0, 0x10000};
    memset( game.pool, 0, sizeof( game.pool ) );
    memmove( &game, data, sizeof( data ) );
}

void gameScene( void ) {
    setbkcolor( DARKGRAY );
    setfillcolor( GREEN );
    bar( 0, 0, gw, gh );
    newFruit();
    for ( int c = -1; is_run(); delay_fps( 60 ), --c ) {
        while ( kbhit() ) {
            int key = getch() | 0x20;
            if ( key == ( 27 | 0x20 ) ) return;
            if ( key == 'a' || key == 'd' ) {
                if ( !moveSnake( ( ( key - 'a' ) >> 1 << 1 ) - 1, 0, true ) ) return;
            } else if ( key == 's' || key == 'w' ) {
                if ( !moveSnake( 0, 1 - ( ( key - 's' ) >> 2 << 1 ), true ) ) return;
            }
        }
        if ( c < 0 ) {
            if ( !moveSnake( ( game.dir & 3 ) - 1, ( game.dir >> 2 ) - 1 ) ) return;
            c = 20;
        }
    }
}

int main( void ) {
    setinitmode( INIT_ANIMATION );
    initgraph( 640, 480 );
    gw = getwidth() / MAP_W, gh = getheight() / MAP_H;
    randomize();
    gameInit();
    gameScene();
    return 0;
}

