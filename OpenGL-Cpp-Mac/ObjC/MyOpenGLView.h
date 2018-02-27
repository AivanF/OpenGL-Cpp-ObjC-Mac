//
//  MyOpenGLView.h
//  OpenGL-Cpp-Mac
//
//  Created by Barsehv Ivan on 02/12/2017.
//

#import <Cocoa/Cocoa.h>

@interface MyOpenGLView : NSOpenGLView

@property (strong) IBOutlet NSTextField *output;

- (BOOL)acceptsFirstResponder;
- (void)scrollWheel:(NSEvent *)event;
- (void)magnifyWithEvent:(NSEvent *)event;
- (void)keyDown:(NSEvent *)event;

@end
