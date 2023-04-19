#include <graphics.h> // Looks like it won't be compatible ?
#include <time.h>
#include <X11/Xlib.h>

int main()
{
	int i,X[100],Y[100],rx,ry,gm,gd,l,d=2,s=16;

	srand(time(NULL));

	detectgraph(&gd,&gm);
	initgraph(&gd,&gm,NULL);

	setfillstyle(1,1);

	// Draw board
	bar(s-s/2,s-s/2,(s/2)+s*(1350/s),s+s/2);
	bar(s-s/2,(-s/2)+s*(700/s),(s/2)+s*(1350/s),(s/2)+s*(700/s));
	bar(s-s/2,s-s/2,s+s/2,(s/2)+s*(700/s));
	bar((-s/2)+s*(1350/s),s-s/2,(s/2)+s*(1350/s),(s/2)+s*(700/s));

	// Initialize and draw snake head
	X[0]=s*(1350/(2*s)); Y[0]=s*(700/(2*s));
    bar(X[0]-s/2,Y[0]-s/2,X[0]+s/2,Y[0]+s/2);

	// Draw snake body
    l=5;
    for(i=1;i<l;i++)
    {
        X[i]=X[0]-(i*s);
        Y[i]=Y[0];
        bar(X[i]-s/2,Y[i]-s/2,X[i]+s/2,Y[i]+s/2);
    }

	// Initialize and draw food if not occupied (not black)
    rx=s; ry=s;
    setfillstyle(1,2);
    while(getpixel(rx,ry)!=0)
	{
		rx=s*(1+rand()%(1350/s-1));
		ry=s*(1+rand()%(700/s-1));
	}
	bar(rx-s/2,ry-s/2,rx+s/2,ry+s/2);

	// Pause 2000ms
    delay(2000);

	// Game loop
	while(1)
	{
		//Erases last segment by drawing black over it
    	setfillstyle(1,0);
		bar(X[l-1]-s/2,Y[l-1]-s/2,X[l-1]+s/2,Y[l-1]+s/2);

		// Update segments based on previous segment
		for(i=l-1;i>0;i--)
    	{
    		X[i]=X[i-1];
    		Y[i]=Y[i-1];
		}

		// Update position of head based on direction (d)
    	if(d==0)
			X[0]=X[0]-s;
		if(d==1)
			Y[0]=Y[0]-s;	
    	else if(d==2)
			X[0]=X[0]+s;
		else if(d==3)
			Y[0]=Y[0]+s;
		
		// Check for body collison (pixel changed to color 1)
		if(getpixel(X[0],Y[0])==1)
			break;
		
		// Update direction of snake from keyboard input
		if((GetAsyncKeyState(VK_RIGHT))&&(d!=0))
			d=2;
		else if((GetAsyncKeyState(VK_UP))&&(d!=3))
			d=1;
		else if((GetAsyncKeyState(VK_LEFT))&&(d!=2))
			d=0;
		else if((GetAsyncKeyState(VK_DOWN))&&(d!=1))
			d=3;

		// Check for food collision (pixel changed to color 2)
		if(getpixel(X[0],Y[0])==2)
		{
			rx=s; ry=s;
			setfillstyle(1,2);

			// Update new food location
			while(getpixel(rx,ry)!=0)
			{
				rx=s*(1+rand()%(1350/s-1));
				ry=s*(1+rand()%(700/s-1));
			}

			// Draw new food and update snake lenght
			bar(rx-s/2,ry-s/2,rx+s/2,ry+s/2);
			l=l+1;
		}

		// Redraw the snake
		setfillstyle(1,1);
		for(i=0;i<l;i++)
       		bar(X[i]-s/2,Y[i]-s/2,X[i]+s/2,Y[i]+s/2);	

		delay(100);
    }

	// Print final score
    printf("score : %d",l-5);

	// Wait for user input return key before closing window
	while(!GetAsyncKeyState(VK_RETURN));

	// Close graphics window and end game
	closegraph();
	getch();

	return 0;	
}
