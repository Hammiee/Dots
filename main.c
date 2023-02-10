#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>



int map [31][31];

void print_map(){
    for(int i = 1; i< 31; i++){
        for ( int j = 1; j < 31; j++){
            if(map[i][j] == 1 ) {
                printf("\x1b[32m%d ", 0);
                printf("\x1b[0m");
            }
            else if(map[i][j] == 2 ) {
                printf("\x1b[31m%d ", 0);
                printf("\x1b[0m");
            }
            else
            printf("%d ", map[i][j]);
        }
        printf("\n");
    }
}

const int shiftX[8] = {-1,-1, 0, 1, 1, 1, 0, -1}, shiftY[8] = {0, -1, -1, -1, 0, 1, 1, 1};

void bestof8(int *x_axis, int *y_axis, int *quality, int h, int i, int j){
    int this_row= 0;
    int x_try=0, y_try=0;
    if( map [i + shiftX[h] ][ j + shiftY[h] ] == 1){
        if(*quality < 1 ){
            this_row=1;
            *quality = 1;
            *x_axis = i;
            *y_axis = j;
        }
    }
    if( map [i + shiftX[h] ][ j + shiftY[h] ] == 2){
        x_try = i;
        y_try = j;
    }
    if( this_row == 1 && x_try != 0){
        *x_axis = x_try;
        *y_axis = y_try;
        *quality = 2;
    }
}

int place_dot(int maxu, int maxl){
    int end=0;
    int quality=0;//0 - random dot, 1 - dot close to comp dots(1), 2- 1st dot which is close to player dot(2)
    int x_axis, y_axis;
    for(int i= maxu; i < 31; i++){
        for(int j = maxl; j < 31; j++){
            if(map[i][j]==0){
                end = 1;
                for( int h= 0; h < 9 ;h++){
                    if( i+shiftX[h] < 30 && i+shiftX[h] > 1 && j+shiftY[h] < 30 && j+shiftY[h] > 1){
                        bestof8(&x_axis, &y_axis, &quality, h, i, j);
                    }
                }
            }
        }
    }
    if (end==0){
        if (maxu==1 && maxl==1){
            //end game, no empty slots
            return 0;
        }
        else{
            //drop dot somewhere else
            if(maxu > 1)
                map[maxu-1][maxl] = 1;
            else map[maxu][maxl-1] = 1;
        }
    }
    else{
        map[x_axis][y_axis] = 1;
        printf("Your opponent placed a dot at (%d, %d)\n", x_axis, y_axis);
    }
    return 1;

}

void ownership(int *count_p, int *count_o, int maxl, int maxu){
    for(int i = maxu +1 ; i < 30; i++){
        for(int j = maxl +1; i < 30; i++){
            int color = map[i][j];
            if(map[i][j])
            //this wont work, should do it with backtracking or bfs
        }
    }
}

int main(){
    int count_p=0, count_o=0;
    int maxl=15, maxu=15;
    //intro
    printf("Starting a Dots game \nYour opponent placed a dot at (15, 15)\n");
    map[15][15]=1;
    count_o++;
    print_map();
    printf("Where do you want to place your dot? First row, then column \n");
    int x_axis;
    int y_axis;
    scanf("%d %d", &x_axis, &y_axis);
    map[x_axis][y_axis] = 2;
    count_p++;
    if(maxl > y_axis){
        maxl=y_axis;
    }
    if(maxu > x_axis){
        maxu=x_axis;
    }
    print_map();

    //game
    while(true){
        //oponent move
        if(place_dot(maxu, maxl)==0) break;
        count_o++;
        print_map();
        //player move
        printf("Where do you want to place your dot? First row, then column \n");
        scanf("%d %d", &x_axis, &y_axis);
        map[x_axis][y_axis]=2;
        count_p++;
        if(maxl > y_axis)
            maxl=y_axis;
        if(maxu > x_axis)
            maxu=x_axis;
        print_map();

        //check if the moves that were made change the ownership of dots and change the array if needed
        //ownership();

        printf("Wanna surrender? 1 or 0\n");
        int sur;
        scanf("%d", &sur);
        if (sur == 1){
            break;
        }
    }
    //print out the winner
    if(count_o > count_p)
        printf("yOU lOsT!");
    if(count_p > count_o)
        printf("YOU WON");
    else
        printf("it's a draw");

    return 0;
}