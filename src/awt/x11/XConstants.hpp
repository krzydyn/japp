#ifndef __AWT_X11_XCONSTANTS_HPP
#define __AWT_X11_XCONSTANTS_HPP

namespace awt { namespace x11 {

class XConstants final {
private:
	XConstants(){}
public:
    /*****************************************************************
     * RESERVED RESOURCE AND CONSTANT DEFINITIONS
     *****************************************************************/

    static const long None = 0L ; /* universal null resource or null atom */

    /* background pixmap in CreateWindow and ChangeWindowAttributes */
    static const long ParentRelative = 1L ;

    /* border pixmap in CreateWindow and ChangeWindowAttributes special
     * VisualID and special window class passed to CreateWindow */
    static const long CopyFromParent = 0L ;

    static const long PointerWindow = 0L ; /* destination window in SendEvent */
    static const long InputFocus = 1L ; /* destination window in SendEvent */

    static const long PointerRoot = 1L ; /* focus window in SetInputFocus */

    static const long AnyPropertyType = 0L ; /* special Atom, passed to GetProperty */

    static const long AnyKey = 0L ; /* special Key Code, passed to GrabKey */

    static const long AnyButton = 0L ; /* special Button Code, passed to GrabButton */

    static const long AllTemporary = 0L ; /* special Resource ID passed to KillClient */

    static const long CurrentTime = 0L ; /* special Time */

    static const long NoSymbol = 0L ; /* special KeySym */


    /*****************************************************************
     * EVENT DEFINITIONS
     *****************************************************************/

    /* Input Event Masks. Used as event-mask window attribute and as arguments
       to Grab requests.  Not to be confused with event names.  */

    static const long NoEventMask = 0L ;
    static const long KeyPressMask = (1L<<0) ;
    static const long KeyReleaseMask = (1L<<1) ;
    static const long ButtonPressMask = (1L<<2) ;
    static const long ButtonReleaseMask = (1L<<3) ;
    static const long EnterWindowMask = (1L<<4) ;
    static const long LeaveWindowMask = (1L<<5) ;
    static const long PointerMotionMask = (1L<<6) ;
    static const long PointerMotionHintMask = (1L<<7) ;
    static const long Button1MotionMask = (1L<<8) ;
    static const long Button2MotionMask = (1L<<9) ;
    static const long Button3MotionMask = (1L<<10) ;
    static const long Button4MotionMask = (1L<<11) ;
    static const long Button5MotionMask = (1L<<12) ;
    static const long ButtonMotionMask = (1L<<13) ;
    static const long KeymapStateMask = (1L<<14) ;
    static const long ExposureMask = (1L<<15) ;
    static const long VisibilityChangeMask = (1L<<16) ;
    static const long StructureNotifyMask = (1L<<17) ;
    static const long ResizeRedirectMask = (1L<<18) ;
    static const long SubstructureNotifyMask = (1L<<19) ;
    static const long SubstructureRedirectMask = (1L<<20) ;
    static const long FocusChangeMask = (1L<<21) ;
    static const long PropertyChangeMask = (1L<<22) ;
    static const long ColormapChangeMask = (1L<<23) ;
    static const long OwnerGrabButtonMask = (1L<<24) ;

    static const int MAX_BUTTONS = 5;
    static const int ALL_BUTTONS_MASK = (int) (Button1MotionMask | Button2MotionMask | Button3MotionMask | Button4MotionMask | Button5MotionMask);


    /* Event names.  Used in "type" field in XEvent structures.  Not to be
    confused with event masks above.  They start from 2 because 0 and 1
    are reserved in the protocol for errors and replies. */

    static const int KeyPress = 2 ;
    static const int KeyRelease = 3 ;
    static const int ButtonPress = 4 ;
    static const int ButtonRelease = 5 ;
    static const int MotionNotify = 6 ;
    static const int EnterNotify = 7 ;
    static const int LeaveNotify = 8 ;
    static const int FocusIn = 9 ;
    static const int FocusOut = 10 ;
    static const int KeymapNotify = 11 ;
    static const int Expose = 12 ;
    static const int GraphicsExpose = 13 ;
    static const int NoExpose = 14 ;
    static const int VisibilityNotify = 15 ;
    static const int CreateNotify = 16 ;
    static const int DestroyNotify = 17 ;
    static const int UnmapNotify = 18 ;
    static const int MapNotify = 19 ;
    static const int MapRequest = 20 ;
    static const int ReparentNotify = 21 ;
    static const int ConfigureNotify = 22 ;
    static const int ConfigureRequest = 23 ;
    static const int GravityNotify = 24 ;
    static const int ResizeRequest = 25 ;
    static const int CirculateNotify = 26 ;
    static const int CirculateRequest = 27 ;
    static const int PropertyNotify = 28 ;
    static const int SelectionClear = 29 ;
    static const int SelectionRequest = 30 ;
    static const int SelectionNotify = 31 ;
    static const int ColormapNotify = 32 ;
    static const int ClientMessage = 33 ;
    static const int MappingNotify = 34 ;
    static const int LASTEvent = 35 ; /* must be bigger than any event # */


    /* Key masks. Used as modifiers to GrabButton and GrabKey, results of QueryPointer,
       state in various key-, mouse-, and button-related events. */

    static const int ShiftMask = (1<<0) ;
    static const int LockMask = (1<<1) ;
    static const int ControlMask = (1<<2) ;
    static const int Mod1Mask = (1<<3) ;
    static const int Mod2Mask = (1<<4) ;
    static const int Mod3Mask = (1<<5) ;
    static const int Mod4Mask = (1<<6) ;
    static const int Mod5Mask = (1<<7) ;

    /* modifier names.  Used to build a SetModifierMapping request or
       to read a GetModifierMapping request.  These correspond to the
       masks defined above. */
    static const int ShiftMapIndex = 0 ;
    static const int LockMapIndex = 1 ;
    static const int ControlMapIndex = 2 ;
    static const int Mod1MapIndex = 3 ;
    static const int Mod2MapIndex = 4 ;
    static const int Mod3MapIndex = 5 ;
    static const int Mod4MapIndex = 6 ;
    static const int Mod5MapIndex = 7 ;

    static const int AnyModifier = (1<<15) ; /* used in GrabButton, GrabKey */


    /* button names. Used as arguments to GrabButton and as detail in ButtonPress
       and ButtonRelease events.  Not to be confused with button masks above.
       Note that 0 is already defined above as "AnyButton".  */

    //static const int buttons [] = new int [] {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};

    /* Notify modes */

    static const int NotifyNormal = 0 ;
    static const int NotifyGrab = 1 ;
    static const int NotifyUngrab = 2 ;
    static const int NotifyWhileGrabbed = 3 ;

    static const int NotifyHint = 1 ; /* for MotionNotify events */


    /* Notify detail */

    static const int NotifyAncestor = 0 ;
    static const int NotifyVirtual = 1 ;
    static const int NotifyInferior = 2 ;
    static const int NotifyNonlinear = 3 ;
    static const int NotifyNonlinearVirtual = 4 ;
    static const int NotifyPointer = 5 ;
    static const int NotifyPointerRoot = 6 ;
    static const int NotifyDetailNone = 7 ;

    /* Visibility notify */

    static const int VisibilityUnobscured = 0 ;
    static const int VisibilityPartiallyObscured = 1 ;
    static const int VisibilityFullyObscured = 2 ;

    /* Circulation request */

    static const int PlaceOnTop = 0 ;
    static const int PlaceOnBottom = 1 ;

    /* protocol families */

    static const int FamilyInternet = 0 ;
    static const int FamilyDECnet = 1 ;
    static const int FamilyChaos = 2 ;

    /* Property notification */

    static const int PropertyNewValue = 0 ;
    static const int PropertyDelete = 1 ;

    /* Color Map notification */

    static const int ColormapUninstalled = 0 ;
    static const int ColormapInstalled = 1 ;

    /* GrabPointer, GrabButton, GrabKeyboard, GrabKey Modes */

    static const int GrabModeSync = 0 ;
    static const int GrabModeAsync = 1 ;

    /* GrabPointer, GrabKeyboard reply status */

    static const int GrabSuccess = 0 ;
    static const int AlreadyGrabbed = 1 ;
    static const int GrabInvalidTime = 2 ;
    static const int GrabNotViewable = 3 ;
    static const int GrabFrozen = 4 ;

    /* AllowEvents modes */

    static const int AsyncPointer = 0 ;
    static const int SyncPointer = 1 ;
    static const int ReplayPointer = 2 ;
    static const int AsyncKeyboard = 3 ;
    static const int SyncKeyboard = 4 ;
    static const int ReplayKeyboard = 5 ;
    static const int AsyncBoth = 6 ;
    static const int SyncBoth = 7 ;

    /* Used in SetInputFocus, GetInputFocus */

    static const int RevertToNone = (int)None ;
    static const int RevertToPointerRoot = (int)PointerRoot ;
    static const int RevertToParent = 2 ;

    /* Used in XEventsQueued */
    static const int QueuedAlready = 0;
    static const int QueuedAfterReading = 1;
    static const int QueuedAfterFlush = 2;

    /*****************************************************************
     * ERROR CODES
     *****************************************************************/

    static const int Success = 0 ; /* everything's okay */
    static const int BadRequest = 1 ; /* bad request code */
    static const int BadValue = 2 ; /* int parameter out of range */
    static const int BadWindow = 3 ; /* parameter not a Window */
    static const int BadPixmap = 4 ; /* parameter not a Pixmap */
    static const int BadAtom = 5 ; /* parameter not an Atom */
    static const int BadCursor = 6 ; /* parameter not a Cursor */
    static const int BadFont = 7 ; /* parameter not a Font */
    static const int BadMatch = 8 ; /* parameter mismatch */
    static const int BadDrawable = 9 ; /* parameter not a Pixmap or Window */
    static const int BadAccess = 10 ; /* depending on context:
                     - key/button already grabbed
                     - attempt to free an illegal
                       cmap entry
                    - attempt to store into a read-only
                       color map entry.
                    - attempt to modify the access control
                       list from other than the local host.
                    */
    static const int BadAlloc = 11 ; /* insufficient resources */
    static const int BadColor = 12 ; /* no such colormap */
    static const int BadGC = 13 ; /* parameter not a GC */
    static const int BadIDChoice = 14 ; /* choice not in range or already used */
    static const int BadName = 15 ; /* font or color name doesn't exist */
    static const int BadLength = 16 ; /* Request length incorrect */
    static const int BadImplementation = 17 ; /* server is defective */

    static const int FirstExtensionError = 128 ;
    static const int LastExtensionError = 255 ;

    /*****************************************************************
     * WINDOW DEFINITIONS
     *****************************************************************/

    /* Window classes used by CreateWindow */
    /* Note that CopyFromParent is already defined as 0 above */

    static const int InputOutput = 1 ;
    static const int InputOnly = 2 ;

    /* Window attributes for CreateWindow and ChangeWindowAttributes */

    static const long CWBackPixmap = (1L<<0) ;
    static const long CWBackPixel = (1L<<1) ;
    static const long CWBorderPixmap = (1L<<2) ;
    static const long CWBorderPixel = (1L<<3) ;
    static const long CWBitGravity = (1L<<4) ;
    static const long CWWinGravity = (1L<<5) ;
    static const long CWBackingStore = (1L<<6) ;
    static const long CWBackingPlanes = (1L<<7) ;
    static const long CWBackingPixel = (1L<<8) ;
    static const long CWOverrideRedirect = (1L<<9) ;
    static const long CWSaveUnder = (1L<<10) ;
    static const long CWEventMask = (1L<<11) ;
    static const long CWDontPropagate = (1L<<12) ;
    static const long CWColormap = (1L<<13) ;
    static const long CWCursor = (1L<<14) ;

    /* ConfigureWindow structure */

    static const int CWX = (1<<0) ;
    static const int CWY = (1<<1) ;
    static const int CWWidth = (1<<2) ;
    static const int CWHeight = (1<<3) ;
    static const int CWBorderWidth = (1<<4) ;
    static const int CWSibling = (1<<5) ;
    static const int CWStackMode = (1<<6) ;

    /* Bit Gravity */

    static const int ForgetGravity = 0 ;
    static const int NorthWestGravity = 1 ;
    static const int NorthGravity = 2 ;
    static const int NorthEastGravity = 3 ;
    static const int WestGravity = 4 ;
    static const int CenterGravity = 5 ;
    static const int EastGravity = 6 ;
    static const int SouthWestGravity = 7 ;
    static const int SouthGravity = 8 ;
    static const int SouthEastGravity = 9 ;
    static const int StaticGravity = 10 ;

    /* Window gravity + bit gravity above */

    static const int UnmapGravity = 0 ;

    /* Used in CreateWindow for backing-store hint */

    static const int NotUseful = 0 ;
    static const int WhenMapped = 1 ;
    static const int Always = 2 ;

    /* Used in GetWindowAttributes reply */

    static const int IsUnmapped = 0 ;
    static const int IsUnviewable = 1 ;
    static const int IsViewable = 2 ;

    /* Used in ChangeSaveSet */

    static const int SetModeInsert = 0 ;
    static const int SetModeDelete = 1 ;

    /* Used in ChangeCloseDownMode */

    static const int DestroyAll = 0 ;
    static const int RetainPermanent = 1 ;
    static const int RetainTemporary = 2 ;

    /* Window stacking method (in configureWindow) */

    static const int Above = 0 ;
    static const int Below = 1 ;
    static const int TopIf = 2 ;
    static const int BottomIf = 3 ;
    static const int Opposite = 4 ;

    /* Circulation direction */

    static const int RaiseLowest = 0 ;
    static const int LowerHighest = 1 ;

    /* Property modes */

    static const int PropModeReplace = 0 ;
    static const int PropModePrepend = 1 ;
    static const int PropModeAppend = 2 ;

    /*****************************************************************
     * GRAPHICS DEFINITIONS
     *****************************************************************/

    /* graphics functions, as in GC.alu */

    static const int GXclear = 0x0 ; /* 0 */
    static const int GXand = 0x1 ; /* src AND dst */
    static const int GXandReverse = 0x2 ; /* src AND NOT dst */
    static const int GXcopy = 0x3 ; /* src */
    static const int GXandInverted = 0x4 ; /* NOT src AND dst */
    static const int GXnoop = 0x5 ; /* dst */
    static const int GXxor = 0x6 ; /* src XOR dst */
    static const int GXor = 0x7 ; /* src OR dst */
    static const int GXnor = 0x8 ; /* NOT src AND NOT dst */
    static const int GXequiv = 0x9 ; /* NOT src XOR dst */
    static const int GXinvert = 0xa ; /* NOT dst */
    static const int GXorReverse = 0xb ; /* src OR NOT dst */
    static const int GXcopyInverted = 0xc ; /* NOT src */
    static const int GXorInverted = 0xd ; /* NOT src OR dst */
    static const int GXnand = 0xe ; /* NOT src OR NOT dst */
    static const int GXset = 0xf ; /* 1 */

    /* LineStyle */

    static const int LineSolid = 0 ;
    static const int LineOnOffDash = 1 ;
    static const int LineDoubleDash = 2 ;

    /* capStyle */

    static const int CapNotLast = 0 ;
    static const int CapButt = 1 ;
    static const int CapRound = 2 ;
    static const int CapProjecting = 3 ;

    /* joinStyle */

    static const int JoinMiter = 0 ;
    static const int JoinRound = 1 ;
    static const int JoinBevel = 2 ;

    /* fillStyle */

    static const int FillSolid = 0 ;
    static const int FillTiled = 1 ;
    static const int FillStippled = 2 ;
    static const int FillOpaqueStippled = 3 ;

    /* fillRule */

    static const int EvenOddRule = 0 ;
    static const int WindingRule = 1 ;

    /* subwindow mode */

    static const int ClipByChildren = 0 ;
    static const int IncludeInferiors = 1 ;

    /* SetClipRectangles ordering */

    static const int Unsorted = 0 ;
    static const int YSorted = 1 ;
    static const int YXSorted = 2 ;
    static const int YXBanded = 3 ;

    /* CoordinateMode for drawing routines */

    static const int CoordModeOrigin = 0 ; /* relative to the origin */
    static const int CoordModePrevious = 1 ; /* relative to previous point */

    /* Polygon shapes */

    static const int Complex = 0 ; /* paths may intersect */
    static const int Nonconvex = 1 ; /* no paths intersect, but not convex */
    static const int Convex = 2 ; /* wholly convex */

    /* Arc modes for PolyFillArc */

    static const int ArcChord = 0 ; /* join endpoints of arc */
    static const int ArcPieSlice = 1 ; /* join endpoints to center of arc */

    /* GC components: masks used in CreateGC, CopyGC, ChangeGC, OR'ed into
       GC.stateChanges */

    static const long GCFunction = (1L<<0) ;
    static const long GCPlaneMask = (1L<<1) ;
    static const long GCForeground = (1L<<2) ;
    static const long GCBackground = (1L<<3) ;
    static const long GCLineWidth = (1L<<4) ;
    static const long GCLineStyle = (1L<<5) ;
    static const long GCCapStyle = (1L<<6) ;
    static const long GCJoinStyle = (1L<<7) ;
    static const long GCFillStyle = (1L<<8) ;
    static const long GCFillRule = (1L<<9) ;
    static const long GCTile = (1L<<10) ;
    static const long GCStipple = (1L<<11) ;
    static const long GCTileStipXOrigin = (1L<<12) ;
    static const long GCTileStipYOrigin = (1L<<13) ;
    static const long GCFont = (1L<<14) ;
    static const long GCSubwindowMode = (1L<<15) ;
    static const long GCGraphicsExposures = (1L<<16) ;
    static const long GCClipXOrigin = (1L<<17) ;
    static const long GCClipYOrigin = (1L<<18) ;
    static const long GCClipMask = (1L<<19) ;
    static const long GCDashOffset = (1L<<20) ;
    static const long GCDashList = (1L<<21) ;
    static const long GCArcMode = (1L<<22) ;

    static const int GCLastBit = 22 ;
    /*****************************************************************
     * FONTS
     *****************************************************************/

    /* used in QueryFont -- draw direction */

    static const int FontLeftToRight = 0 ;
    static const int FontRightToLeft = 1 ;

    static const int FontChange = 255 ;

    /*****************************************************************
     *  IMAGING
     *****************************************************************/

    /* ImageFormat -- PutImage, GetImage */

    static const int XYBitmap = 0 ; /* depth 1, XYFormat */
    static const int XYPixmap = 1 ; /* depth == drawable depth */
    static const int ZPixmap = 2 ; /* depth == drawable depth */

    /*****************************************************************
     *  COLOR MAP STUFF
     *****************************************************************/

    /* For CreateColormap */

    static const int AllocNone = 0 ; /* create map with no entries */
    static const int AllocAll = 1 ; /* allocate entire map writeable */


    /* Flags used in StoreNamedColor, StoreColors */

    static const int DoRed = (1<<0) ;
    static const int DoGreen = (1<<1) ;
    static const int DoBlue = (1<<2) ;

    /*****************************************************************
     * CURSOR STUFF
     *****************************************************************/

    /* QueryBestSize Class */

    static const int CursorShape = 0 ; /* largest size that can be displayed */
    static const int TileShape = 1 ; /* size tiled fastest */
    static const int StippleShape = 2 ; /* size stippled fastest */

    /*****************************************************************
     * KEYBOARD/POINTER STUFF
     *****************************************************************/

    static const int AutoRepeatModeOff = 0 ;
    static const int AutoRepeatModeOn = 1 ;
    static const int AutoRepeatModeDefault = 2 ;

    static const int LedModeOff = 0 ;
    static const int LedModeOn = 1 ;

    /* masks for ChangeKeyboardControl */

    static const long KBKeyClickPercent = (1L<<0) ;
    static const long KBBellPercent = (1L<<1) ;
    static const long KBBellPitch = (1L<<2) ;
    static const long KBBellDuration = (1L<<3) ;
    static const long KBLed = (1L<<4) ;
    static const long KBLedMode = (1L<<5) ;
    static const long KBKey = (1L<<6) ;
    static const long KBAutoRepeatMode = (1L<<7) ;

    static const int MappingSuccess = 0 ;
    static const int MappingBusy = 1 ;
    static const int MappingFailed = 2 ;

    static const int MappingModifier = 0 ;
    static const int MappingKeyboard = 1 ;
    static const int MappingPointer = 2 ;

    /*****************************************************************
     * SCREEN SAVER STUFF
     *****************************************************************/

    static const int DontPreferBlanking = 0 ;
    static const int PreferBlanking = 1 ;
    static const int DefaultBlanking = 2 ;

    static const int DisableScreenSaver = 0 ;
    static const int DisableScreenInterval = 0 ;

    static const int DontAllowExposures = 0 ;
    static const int AllowExposures = 1 ;
    static const int DefaultExposures = 2 ;

    /* for ForceScreenSaver */

    static const int ScreenSaverReset = 0 ;
    static const int ScreenSaverActive = 1 ;

    /*****************************************************************
     * HOSTS AND CONNECTIONS
     *****************************************************************/

    /* for ChangeHosts */

    static const int HostInsert = 0 ;
    static const int HostDelete = 1 ;

    /* for ChangeAccessControl */

    static const int EnableAccess = 1 ;
    static const int DisableAccess = 0 ;

    /* Display classes  used in opening the connection
     * Note that the statically allocated ones are even numbered and the
     * dynamically changeable ones are odd numbered */

    static const int StaticGray = 0 ;
    static const int GrayScale = 1 ;
    static const int StaticColor = 2 ;
    static const int PseudoColor = 3 ;
    static const int TrueColor = 4 ;
    static const int DirectColor = 5 ;


    /* Byte order  used in imageByteOrder and bitmapBitOrder */

    static const int LSBFirst = 0 ;
    static const int MSBFirst = 1 ;

    /* XKB support */
    static const int  XkbUseCoreKbd = 0x0100 ;
    static const int  XkbNewKeyboardNotify = 0;
    static const int  XkbMapNotify = 1;
    static const int  XkbStateNotify = 2;
    static const long XkbNewKeyboardNotifyMask = (1L << 0);
    static const long XkbMapNotifyMask = (1L << 1);
    static const long XkbStateNotifyMask = (1L << 2);
    static const long XkbGroupStateMask  = (1L << 4);
    static const long XkbKeyTypesMask = (1L<<0);
    static const long XkbKeySymsMask = (1L<<1);
    static const long XkbModifierMapMask = (1L<<2);
    static const long XkbVirtualModsMask = (1L<<6); //server map
};

class XUtilConstants final {
private:
	XUtilConstants(){}
public:
    /*
     * Bitmask returned by XParseGeometry().  Each bit tells if the corresponding
     * value (x, y, width, height) was found in the parsed string.
     */
    static const int NoValue = 0x0000 ;
    static const int XValue = 0x0001 ;
    static const int YValue = 0x0002 ;
    static const int WidthValue = 0x0004 ;
    static const int HeightValue = 0x0008 ;
    static const int AllValues = 0x000F ;
    static const int XNegative = 0x0010 ;
    static const int YNegative = 0x0020 ;

    /*
     * The next block of definitions are for window manager properties that
     * clients and applications use for communication.
     */

    /* flags argument in size hints */
    static const long USPosition = 1L << 0; /* user specified x, y */
    static const long USSize = 1L << 1; /* user specified width, height */

    static const long PPosition = 1L << 2; /* program specified position */
    static const long PSize = 1L << 3; /* program specified size */
    static const long PMinSize = 1L << 4; /* program specified minimum size */
    static const long PMaxSize = 1L << 5; /* program specified maximum size */
    static const long PResizeInc = 1L << 6; /* program specified resize increments */
    static const long PAspect = 1L << 7; /* program specified min and max aspect ratios */
    static const long PBaseSize = 1L << 8; /* program specified base for incrementing */
    static const long PWinGravity = 1L << 9; /* program specified window gravity */

    /* obsolete */
    static const long PAllHints = (PPosition|PSize|PMinSize|PMaxSize|PResizeInc|PAspect) ;

    /* definition for flags of XWMHints */

    static const long InputHint = 1L << 0;
    static const long StateHint = 1L << 1;
    static const long IconPixmapHint = 1L << 2;
    static const long IconWindowHint = 1L << 3;
    static const long IconPositionHint = 1L << 4;
    static const long IconMaskHint = 1L << 5;
    static const long WindowGroupHint = 1L << 6;
    static const long AllHints = (InputHint|StateHint|IconPixmapHint|IconWindowHint|
        IconPositionHint|IconMaskHint|WindowGroupHint);
    static const long XUrgencyHint = 1L << 8;

    /* definitions for initial window state */
    static const int WithdrawnState = 0 ; /* for windows that are not mapped */
    static const int NormalState = 1 ; /* most applications want to start this way */
    static const int IconicState = 3 ; /* application wants to start as an icon */

    /*
     * Obsolete states no longer defined by ICCCM
     */
    static const int DontCareState = 0 ; /* don't know or care */
    static const int ZoomState = 2 ; /* application wants to start zoomed */
    /* application believes it is seldom used; some wm's may put it on inactive menu */
    static const int InactiveState = 4 ;

    static const int XNoMemory = -1 ;
    static const int XLocaleNotSupported = -2 ;
    static const int XConverterNotFound = -3 ;

    /* Return values from XRectInRegion() */
    static const int RectangleOut = 0 ;
    static const int RectangleIn = 1 ;
    static const int RectanglePart = 2 ;

    /*
     * Information used by the visual utility routines to find desired visual
     * type from the many visuals a display may support.
     */
    static const int VisualNoMask = 0x0 ;
    static const int VisualIDMask = 0x1 ;
    static const int VisualScreenMask = 0x2 ;
    static const int VisualDepthMask = 0x4 ;
    static const int VisualClassMask = 0x8 ;
    static const int VisualRedMaskMask = 0x10 ;
    static const int VisualGreenMaskMask = 0x20 ;
    static const int VisualBlueMaskMask = 0x40 ;
    static const int VisualColormapSizeMask = 0x80 ;
    static const int VisualBitsPerRGBMask = 0x100 ;
    static const int VisualAllMask = 0x1FF ;

    /*
     * return codes for XReadBitmapFile and XWriteBitmapFile
     */
    static const int BitmapSuccess = 0 ;
    static const int BitmapOpenFailed = 1 ;
    static const int BitmapFileInvalid = 2 ;
    static const int BitmapNoMemory = 3 ;

    /****************************************************************
     *
     * Context Management
     *
     ****************************************************************
     */
    /* Associative lookup table return codes */
    static const int XCSUCCESS = 0 ; /* No error. */
    static const int XCNOMEM = 1 ; /* Out of memory */
    static const int XCNOENT = 2 ; /* No entry in table */

    // typedef int XContext;
};

}}

#endif
