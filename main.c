#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

//#include "capture.h"


int map [31][31];
const int shiftX[8] = {-1,-1, 0, 1, 1, 1, 0, -1}, shiftY[8] = {0, -1, -1, -1, 0, 1, 1, 1};

int odw [31][31];
int que [90000][900][2];

void copy_que(int nque, int slot, int ad){
    for(int i=0; i < slot; i++){
        que[nque+ad][i][0]=que[nque][i][0];
        que[nque+ad][i][1]=que[nque][i][1];
    }
}

int cycle(int color, int x, int y, int finx, int finy, int *nque, int *slot){
    que [*nque][*slot][0]=x;
    que [*nque][*slot][1]=y;
    *slot= *slot + 1;
    odw[x][y]=1;
    for(int i=0; i < 8; i++){
        if(x + shiftX[i] == finx && y + shiftY[i] == finy){
            return 1; //cycle exist
        }
        else if(map[x+shiftX[i]][y+shiftY[i]] == color && odw[x+shiftX[i]][y+shiftY[i]] == 0 ){
            copy_que(*nque, *slot, i+1);

            cycle(color, x+shiftX[i], y + shiftY[i], finx, finy, nque+i+1, slot);
        }
    }
    return 0; //no cycle
}

int pom[31][31];

void ownership(int *count_p, int *count_o, int maxl, int maxu){
    for(int i = maxu +1 ; i < 31; i++){
        for(int j = maxl +1; i < 31; i++){
            int color = map[i][j];
            int nque=0;
            int slot=0;
            if(color != 0){
                if(cycle(color, i, j, i, j, &nque, &slot)==1){
                    //change the color
                    int up=-1, down=35, left=35, right=-1;
                    for(int h=0; h<slot; h++){
                        pom[que[nque][h+1][0]][que[nque][h+1][1]]=1;
                        if(que[nque][h][0] < down)
                            down = que[nque][h][0];
                        if(que[nque][h][0] > up)
                            up = que[nque][h][0];
                        if(que[nque][h][1] < left)
                            left = que[nque][h][0];
                        if(que[nque][h][1] > right)
                            right = que[nque][h][0];
                    }
                    int change=0;
                    for(int h = left; h <= right; h++ ){
                        for(int g = down; g <= up; g++){
                            if(pom[h][g]==1){
                                change++;
                            }
                            if(change%2==1){
                                map[h][g]=color;
                            }
                        }
                    }
                }
            }
            for(int k1=1; k1 < 31; k1++){
                for(int k2=1; k2 < 31; k2++){
                    odw[k1][k2]=0;
                    pom[k1][k2]=0;
                }
            }
        }
    }
}







void print_map(){
    for(int i = 0; i< 31; i++){
        for ( int j = 0; j < 31; j++){
            if(j==0 && i==0){
                printf("   ");
            }
            else if(i==0){
                if(j < 10)
                    printf("%d  ", j);
                else printf("%d ", j);
            }
            else if(j==0){
                if(i < 10)
                    printf("%d  ", i);
                else printf("%d ", i);
            }
            else if(map[i][j] == 1 ) {
                printf("\x1b[32m%d  ", 0);
                printf("\x1b[0m");
            }
            else if(map[i][j] == 2 ) {
                printf("\x1b[31m%d  ", 0);
                printf("\x1b[0m");
            }
            else
            printf("%d  ", map[i][j]);
        }
        printf("\n");
    }
}


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
        while (map[x_axis][y_axis]!=0){
            printf("You cant place a dot here, try again\n");
            scanf("%d %d", &x_axis, &y_axis);
        }
        map[x_axis][y_axis]=2;
        count_p++;
        if(maxl > y_axis)
            maxl=y_axis;
        if(maxu > x_axis)
            maxu=x_axis;
        print_map();

        //check if the moves that were made change the ownership of dots and change the array if needed
        ownership(&count_p, &count_o, maxl, maxu);

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