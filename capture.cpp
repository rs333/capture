#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <algorithm>

#include <X11/Xlib.h>
#include <X11/Xutil.h> // Needed for XGetPixel

const std::vector<char> BRIGHTNESS_CHARS = {' ', '.', ',', 'o', 'x', 'X', '0', '@'};

//
// A simple program to demonstrate capture the screen and outputing the data 
// as ascii art.
//
int main(int argc, char *argv[])
{
    int colStart=0;
    int rowStart=0;
    int width=100;
    int height=100;
    switch( argc ){
        case 5:
            colStart = atoi(argv[1]);
            rowStart = atoi(argv[2]);
            width = atoi(argv[3]);
            height = atoi(argv[4]);
            break;
        case 3:
            width = atoi(argv[1]);
            height = atoi(argv[2]);
            break;
        case 1:
            break;
        default:
            std::cout << "Usages: \n\n"
                    << "  " << argv[0] << " [upper left column] [upper left row] [width] [height]\n"
                    << "  " << argv[0] << " [width] [height]\n"
                    << "  " << argv[0] << "\n\n"
                    << "  - Failure to enter any arguments is equivalent to \n"
                    << "    " << argv[0] << " " << colStart << " " << rowStart << " " << width << " " << height << std::endl;
            return 0;
    }
    
    Display *display = XOpenDisplay(NULL);
    Window rootWindow = RootWindow(display, DefaultScreen(display));

    XWindowAttributes rootAttributes;
    XGetWindowAttributes(display, rootWindow, &rootAttributes);

    width = ( width > rootAttributes.width - colStart ? rootAttributes.width -colStart : width );
    height = ( height > rootAttributes.height - rowStart ? rootAttributes.height- rowStart : height );

    XColor colors;
    XImage *image;

    // move the mouse to the middle of the portion you are capturing.
    XWarpPointer(display, rootWindow, rootWindow, 0, 0, 0, 0, width/2, height/2 );
    // To simulate a click, check out XTestFakeButtonEvent & XSendEvent

    image = XGetImage( display, rootWindow, colStart, rowStart, width, height, AllPlanes, ZPixmap);
    

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            colors.pixel = XGetPixel(image, j, i);

            uint16_t red = (colors.pixel & image->red_mask) >> 16;
            uint16_t green = (colors.pixel & image->green_mask) >> 8;
            uint16_t blue = (colors.pixel & image->blue_mask);
            uint16_t brightness = (red + green + blue) / (3 * 255 / 8);
            brightness = (brightness > BRIGHTNESS_CHARS.size() - 1 ? BRIGHTNESS_CHARS.size() - 1 : brightness);

            std::cout << BRIGHTNESS_CHARS[brightness];
        }
        std::cout << std::endl;
    }

    XFree(image);
}
