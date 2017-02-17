#include "TDynamicScrollBar.h"

#define Uses_TEvent
#define Uses_TGroup
#define Uses_TKeys
#include <tv.h>

static TPoint mouse;
static int32 p, s;
static TRect extent;


void TDynamicScrollBar::handleEvent( TEvent& event )
{
    Boolean Tracking;
    int i=0, clickPart;

    TView::handleEvent(event);
    switch( event.what )
	{
	case evMouseDown:
	    message(owner, evBroadcast, cmScrollBarClicked,this); // Clicked()
	    mouse = makeLocal( event.mouse.where );
	    extent = getExtent();
	    extent.grow(1, 1);
	    p = getPos();
	    s = getSize() - 1;
	    clickPart= getPartCode();
	    if( clickPart != sbIndicator )
		{
		do  {
		    mouse = makeLocal( event.mouse.where );
		    if( getPartCode() == clickPart )
			setValue(value + scrollStep(clickPart) );
		    } while( mouseEvent(event, evMouseAuto) );
                }
            else
                {
                do  {
                    mouse = makeLocal( event.mouse.where );
                    Tracking = extent.contains(mouse);
                    if( Tracking )
                        {
                        if( size.x == 1 )
                            i = mouse.y;
                        else
                            i = mouse.x;
                        i = max( i, 1 );
                        i = min( i, s-1 );
                        }
                    else
                        i = getPos();
                    if(i != p )
                        {
                        drawPos(i);
                        p = i;
                        if (s>2){
                          setValue( int(((long(p - 1) * (maxVal - minVal) + ((s-2) >> 1)) / (s-2)) + minVal));
                        }
                        }
                    } while( mouseEvent(event,evMouseMove) );
                if( Tracking && s > 2 )
                    {
                    s -= 2;
                    setValue( int(((long(p - 1) * (maxVal - minVal) + (s >> 1)) / s) + minVal));
                    }
                }
            clearEvent(event);
            break;
        case  evKeyDown:
            if( (state & sfVisible) != 0 )
                {
                clickPart = sbIndicator;
                if( size.y == 1 )
                    switch( ctrlToArrow(event.keyDown.keyCode) )
                        {
                        case kbLeft:
                            clickPart = sbLeftArrow;
                            break;
                        case kbRight:
                            clickPart = sbRightArrow;
                            break;
                        case kbCtrlLeft:
                            clickPart = sbPageLeft;
                            break;
                        case kbCtrlRight:
                            clickPart = sbPageRight;
                            break;
                        case kbHome:
                            i = minVal;
                            break;
                        case kbEnd:
                            i = maxVal;
                            break;
                        default:
                            return;
                        }
                else
                    switch( ctrlToArrow(event.keyDown.keyCode) )
                        {
                        case kbUp:
                            clickPart = sbUpArrow;
                            break;
                        case kbDown:
                            clickPart = sbDownArrow;
                            break;
                        case kbPgUp:
                            clickPart = sbPageUp;
                            break;
                        case kbPgDn:
                            clickPart = sbPageDown;
                            break;
                        case kbCtrlPgUp:
                            i = minVal;
                            break;
                        case kbCtrlPgDn:
                            i = maxVal;
                            break;
                        default:
                            return;
                        }
                message(owner,evBroadcast,cmScrollBarClicked,this); // Clicked
                if( clickPart != sbIndicator )
                    i = value + scrollStep(clickPart);
                setValue(i);
                clearEvent(event);
                }
        }
}



int TDynamicScrollBar::getPartCode()
{
    int part= - 1;
    if( extent.contains(mouse) )
	{
	int mark = (size.x == 1) ? mouse.y : mouse.x;

	if (mark == p)
	    part= sbIndicator;
	else
	    {
	    if( mark < 1 )
		part = sbLeftArrow;
	    else if( mark < p )
		part= sbPageLeft;
	    else if( mark < s )
		part= sbPageRight;
	    else
		part= sbRightArrow;

	    if( size.x == 1 )
		part += 4;
	    }
	}
    return part;
}
