#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
  

struct arrChar{
char string[9];
};

int main(){
    char buf[1000];
    //char pru[48];
    int num=8;
    int numr1=0;
    int numr2=0;
    int numr3=0;
    int numr4=0;
    int valgr1=0;
    int valgr2=0;
    int valgr3=0;
    int valgr4=0;
    int pru=0;
    FILE *resultadografi;
    char caracter;

    resultadografi =fopen("resultado.txt","r");
    printf("Resultado final, aplicado el movimiento: \n\n");
    printf("  ABCDEFGH\n");
    while (fgets(buf,1000, resultadografi)!=NULL){
        if(num == 0){
            numr1=buf[1]-'0';
            numr2=buf[3]-'0';
            numr3=buf[5]-'0';
            numr4=buf[7]-'0';
        }
        else{
            printf("%d ",num);
            printf("%s",buf);
        }
        num--;
    }

    //https://www.geeksforgeeks.org/bar-function-c-graphics/

    // gm is Graphics mode which is
    // a computer display mode that
    // generates image using pixels.
    // DETECT is a macro defined in
    // "graphics.h" header file
    int gd = DETECT, gm;
  
    // initgraph initializes the
    // graphics system by loading a
    // graphics driver from disk
    initgraph(&gd, &gm, "");
  
    // location of sides
    int left, top, right, bottom;
    
    valgr1=350-(numr1*30);
    valgr2=350-(numr2*30);
    valgr3=350-(numr3*30);
    valgr4=350-(numr4*30);
    // left, top, right, bottom denotes
    // location of rectangular bar
    bar(left = 150, top = valgr1, 
    right = 190, bottom = 350);
  
    bar(left = 220, top = valgr2,
    right = 260, bottom = 350);
  
    bar(left = 290, top = valgr3,
    right = 330, bottom = 350);

    bar(left = 360, top = valgr4,
    right = 400, bottom = 350);
  
    // y axis line
    line(100, 50, 100, 350);
  
    // x axis line
    line(100, 350, 470, 350);

    //valores y
    outtextxy(65,50,"10");
    outtextxy(65,80,"9");
    outtextxy(65,110,"8");
    outtextxy(65,140,"7");
    outtextxy(65,170,"6");
    outtextxy(65,200,"5");
    outtextxy(65,230,"4");
    outtextxy(65,260,"3");
    outtextxy(65,290,"2");
    outtextxy(65,320,"1");
    outtextxy(65,350,"0");

    //valores x

    outtextxy(370,370,"R4");
    outtextxy(300,370,"R3");
    outtextxy(230,370,"R2");
    outtextxy(160,370,"R1");

    getch();
      
    // closegraph function closes the
    // graphics mode and deallocates
    // all memory allocated by
    // graphics system .
    closegraph();
      
    return 0;
}

//gcc final.c -o final -lgraph