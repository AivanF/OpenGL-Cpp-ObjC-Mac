//
//  MyOpenGLView.m
//  OpenGL-Cpp-Mac
//
//  Created by Barsehv Ivan on 02/12/2017.
//

#import "MyOpenGLView.h"
#include <OpenGL/gl3.h>
#include <ApplicationServices/ApplicationServices.h>
#include "World.hpp"
#include "helper.hpp"

@interface MyOpenGLView () {
    NSTimer *Timer;
    int screenWidth, screenHeight;
    World *w;
}
@end

@implementation MyOpenGLView

- (void)awakeFromNib {
    [self setAcceptsTouchEvents:YES];
    NSScreen *screen = [[NSScreen screens] firstObject];
    NSRect screenRect = [screen frame];
    screenWidth = (int)screenRect.size.width;
    screenHeight = (int)screenRect.size.height;
    
    
    NSOpenGLPixelFormatAttribute attrs[] = {
        NSOpenGLPFADepthSize, 24,
        NSOpenGLPFAOpenGLProfile,
        NSOpenGLProfileVersion4_1Core,
        0
    };
    NSOpenGLPixelFormat *pf = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
    if (!pf) {
        NSLog(@"No OpenGL pixel format");
    }
    NSOpenGLContext* context = [[NSOpenGLContext alloc] initWithFormat:pf shareContext:nil];
    [self setPixelFormat:pf];
    [self setOpenGLContext:context];
    NSLog(@"%s", glGetString(GL_VERSION));
    
    Timer = [NSTimer scheduledTimerWithTimeInterval:0.05
                                             target:self
                                           selector:@selector(fire)
                                           userInfo:nil
                                            repeats:YES];
    w = new World();
}

- (void)dealloc {
    delete w;
}

- (void)fire {
    [self setNeedsDisplay:YES];
    
    NSString *fogger;
    switch (w->fogtype()) {
        case 0:
            fogger = [NSString stringWithFormat:@"Lin fog"];
            break;
        case 1:
            fogger = [NSString stringWithFormat:@"Exp fog"];
            break;
        case 2:
            fogger = [NSString stringWithFormat:@"Exp^2 fog"];
            break;
            
        default:
            fogger = [NSString stringWithFormat:@"No fog"];
            break;
    }
    [_output setStringValue:[NSString stringWithFormat:@"Ry: %d° Rx: %d°\nP: %d %d %d\n%@",
                             (int)w->_ry, (int)w->_rx,
                             (int)w->_px, (int)w->_py, (int)w->_pz,
                             fogger]];
}

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    w->draw(_bounds.size.width, _bounds.size.height);
}

- (BOOL)acceptsFirstResponder {
    return YES;
}

- (void)scrollWheel:(NSEvent *)event {
    w->rotate([event deltaY], [event deltaX]);
}

- (void)magnifyWithEvent:(NSEvent *)event {
    int ch = (int)[event deltaZ];
//    NSLog(@"Zoom: %d", ch);
    if (ch > 0) {
        w->move(0, 1);
    } else if (ch < 0) {
        w->move(0, -1);
    }
}

- (void)screenshot {
    // https://www.opengl.org/discussion_boards/showthread.php/182873-Screenshot-using-Cocoa
    
    NSString *file_name = @"some_screen.png";
    // Get the size of the image in a retina safe way
    NSRect backRect = [self convertRectToBacking: [self bounds]];
    int W = NSWidth(backRect);
    int H = NSHeight(backRect);
    // Create image. Note no alpha channel. I don't copy that.
    NSBitmapImageRep *rep = [[NSBitmapImageRep alloc] initWithBitmapDataPlanes: NULL
                                                                    pixelsWide: W pixelsHigh: H bitsPerSample: 8 samplesPerPixel: 3 hasAlpha: NO
                                                                      isPlanar: NO colorSpaceName: NSCalibratedRGBColorSpace bytesPerRow: 3*W bitsPerPixel: 0];
    GetError();
    
    // The following block does the actual reading of the image
    glReadBuffer(GL_FRONT);
    glPixelStorei(GL_PACK_ALIGNMENT, 1); // Dense packing
    glReadPixels(0, 0, W, H, GL_RGB, GL_UNSIGNED_BYTE, [rep bitmapData]);
    GetError();
    
    CIImage* ciimag = [[CIImage alloc] initWithBitmapImageRep: rep];
    CGAffineTransform trans = CGAffineTransformIdentity;
    trans = CGAffineTransformMakeTranslation(0.0f, H);
    trans = CGAffineTransformScale(trans, 1.0, -1.0);
    ciimag = [ciimag imageByApplyingTransform:trans];
    rep = [[NSBitmapImageRep alloc] initWithCIImage: ciimag];
    if ([file_name length] < 1) { //save to clipboard
        NSImage *imag = [[NSImage alloc] init];
        [imag addRepresentation:rep];
        NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
        [pasteboard clearContents];
        NSArray *copiedObjects = [NSArray arrayWithObject:imag];
        [pasteboard writeObjects:copiedObjects];
        return;
    }
    NSData *data = [rep representationUsingType: NSPNGFileType properties: [NSDictionary new]];
    [data writeToFile: file_name atomically: NO];
    GetError();
}

- (void)keyDown:(NSEvent *)event {
    unsigned short key = [event keyCode];
    switch(key) {
        case 13:// W
            w->move(0, 1);
            break;
        case 0:// A
            w->move(-1, 0);
            break;
        case 1:// S
            w->move(0, -1);
            break;
        case 2:// D
            w->move(1, 0);
            break;
        case 36:// Enter
            w->keyEnter();
            break;
        case 48:// Tab
            w->keyTab();
            break;
        case 49:// Space
            w->keySpace();
            break;
        case 51:// Backspace
            w->keyShift();
            break;
        case 53:// Escape
            [self screenshot];
            break;
        case 123:// left
            w->move(-1, 0);
            break;
        case 124:// right
            w->move(1, 0);
            break;
        case 126:// up
            w->move(0, 1);
            break;
        case 125:// down
            w->move(0, -1);
            break;
        default:
//            NSLog(@"Pressed key: %d", (int)key);
//            [self interpretKeyEvents:[NSArray arrayWithObject:event]];
//            [super keyDown:event];
            break;
    }
}

@end


