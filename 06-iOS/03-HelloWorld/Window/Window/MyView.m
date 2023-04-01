//
//  MyView.m
//  Window
//
//  Created by user224299 on 12/25/22.
//
#import "MyView.h"
@implementation MyView
{
    @private
    NSString *string;
}
- (id)initWithFrame:(CGRect)frame
{
    // Code
    self = [super initWithFrame:frame];
    if(self)
    {
        string = @"Hello World!!!";
        // GestureRecognizer Code
        // Single Tap
        // Target Action pattern is followed by all gestureRecognizers
        UITapGestureRecognizer* singleTapGestureRecognizer = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(onSingleTap:)];
        [singleTapGestureRecognizer setNumberOfTapsRequired:1];
        [singleTapGestureRecognizer setNumberOfTouchesRequired:1];
        [singleTapGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:singleTapGestureRecognizer];

        // Double Tap
        UITapGestureRecognizer *doubleTapGestureRecognizer = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(onDoubleTap:)];
        [doubleTapGestureRecognizer setNumberOfTapsRequired:2];
        [doubleTapGestureRecognizer setNumberOfTouchesRequired:1];
        [doubleTapGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:doubleTapGestureRecognizer];

        // Differentiate between single and double tap
        [singleTapGestureRecognizer requireGestureRecognizerToFail:doubleTapGestureRecognizer];

        // Swipe
        UISwipeGestureRecognizer* swipeGestureRecognizer = [[UISwipeGestureRecognizer alloc]initWithTarget:self action:@selector(onSwipe:)];
        [self addGestureRecognizer:swipeGestureRecognizer];

        // Long press
        UILongPressGestureRecognizer* longPressGestureRecognizer = [[UILongPressGestureRecognizer alloc]initWithTarget:self action:@selector(onLongPress:)];
        [self addGestureRecognizer:longPressGestureRecognizer];
    }
    return self;
}

// WM_PAINT / XPose
- (void)drawRect:(CGRect)dirtyRect
{
    // Code
    UIColor *backgroundColor = [UIColor blackColor];
    [backgroundColor set];
    UIRectFill(dirtyRect);
    UIFont *font = [UIFont fontWithName:@"Helvetica" size:24];
    UIColor *textColor = [UIColor greenColor];
    NSDictionary *dictionary = [NSDictionary dictionaryWithObjectsAndKeys:font,NSFontAttributeName,
                                                                          textColor,NSForegroundColorAttributeName, nil];
    CGSize textSize = [string sizeWithAttributes:dictionary];
    CGPoint point;
    point.x = dirtyRect.size.width/2 - textSize.width/2;
    point.y = dirtyRect.size.height/2 - textSize.height/2 + 12;
    [string drawAtPoint:point withAttributes:dictionary];
}

- (BOOL)acceptsFirstResponder
{
    // Code
    return YES;
}

- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)withEvent
{
    // Code
}

- (void)onSingleTap:(UITapGestureRecognizer*)gr
{
    // Code
    string = @"Single Tap";
    [self setNeedsDisplay];
}

- (void)onDoubleTap:(UITapGestureRecognizer*)gr
{
    // Code
    string = @"Double Tap";
    [self setNeedsDisplay];
}

- (void)onSwipe:(UISwipeGestureRecognizer*)sgr
{
    // Code
    [self release];
    exit(0);
}

- (void)onLongPress:(UILongPressGestureRecognizer*)lpgr
{
    // Code
    string = @"Long Press";
    [self setNeedsDisplay];
}

- (void)dealloc
{
    // Code
    [super dealloc];
}
@end
