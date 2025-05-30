/****************************************************************************
** $Id: qt/qscrollview.cpp   3.3.4   edited Feb 18 2004 $
**
** Implementation of QScrollView class
**
** Created : 950524
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of the widgets module of the Qt GUI Toolkit.
**
** This file may be distributed under the terms of the Q Public License
** as defined by Trolltech AS of Norway and appearing in the file
** LICENSE.QPL included in the packaging of this file.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid Qt Enterprise Edition or Qt Professional Edition
** licenses may use this file in accordance with the Qt Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.trolltech.com/pricing.html or email sales@trolltech.com for
**   information about Qt Commercial License Agreements.
** See http://www.trolltech.com/qpl/ for QPL licensing information.
** See http://www.trolltech.com/gpl/ for GPL licensing information.
**
** Contact info@trolltech.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#include "qwidget.h"
#ifndef QT_NO_SCROLLVIEW
#include "qscrollbar.h"
#include "qobjectlist.h"
#include "qpainter.h"
#include "qpixmap.h"
#include "qcursor.h"
#include "qfocusdata.h"
#include "qscrollview.h"
#include "qptrdict.h"
#include "qapplication.h"
#include "qtimer.h"
#include "qstyle.h"
#ifdef Q_WS_MAC
# include "qt_mac.h"
#endif

static const int coord_limit = 4000;
static const int autoscroll_margin = 16;
static const int initialScrollTime = 30;
static const int initialScrollAccel = 5;

struct QSVChildRec {
    QSVChildRec(QWidget* c, int xx, int yy) :
        child(c),
        x(xx), y(yy)
    {
    }

    void hideOrShow(QScrollView* sv, QWidget* clipped_viewport);
    void moveTo(QScrollView* sv, int xx, int yy, QWidget* clipped_viewport)
    {
        if ( x != xx || y != yy ) {
            x = xx;
            y = yy;
            hideOrShow(sv,clipped_viewport);
        }
    }
    QWidget* child;
    int x, y;
};

void QSVChildRec::hideOrShow(QScrollView* sv, QWidget* clipped_viewport)
{
    if ( clipped_viewport ) {
	if ( x+child->width() < sv->contentsX()+clipped_viewport->x()
	     || x > sv->contentsX()+clipped_viewport->width()
	     || y+child->height() < sv->contentsY()+clipped_viewport->y()
	     || y > sv->contentsY()+clipped_viewport->height() ) {
	    child->move(clipped_viewport->width(),
			clipped_viewport->height());
	} else {
	    child->move(x-sv->contentsX()-clipped_viewport->x(),
			y-sv->contentsY()-clipped_viewport->y());
	}
    } else {
	child->move(x-sv->contentsX(), y-sv->contentsY());
    }
}

class QViewportWidget : public QWidget
{
    Q_OBJECT

public:
    QViewportWidget( QScrollView* parent=0, const char* name=0, WFlags f = 0 )
	: QWidget( parent, name, f ) {}
};

class QClipperWidget : public QWidget
{
    Q_OBJECT

public:
    QClipperWidget( QWidget * parent=0, const char * name=0, WFlags f=0 )
        : QWidget ( parent,name,f) {}
};

#include "qscrollview.moc"

class QScrollViewData {
public:
    QScrollViewData(QScrollView* parent, int vpwflags) :
        hbar( new QScrollBar( QScrollBar::Horizontal, parent, "qt_hbar" ) ),
        vbar( new QScrollBar( QScrollBar::Vertical, parent, "qt_vbar" ) ),
        viewport( new QViewportWidget( parent, "qt_viewport", vpwflags ) ),
        clipped_viewport( 0 ),
        flags( vpwflags ),
        vx( 0 ), vy( 0 ), vwidth( 1 ), vheight( 1 ),
#ifndef QT_NO_DRAGANDDROP
        autoscroll_timer( parent, "scrollview autoscroll timer" ),
	drag_autoscroll( TRUE ),
#endif
	scrollbar_timer( parent, "scrollview scrollbar timer" ),
        inresize( FALSE ), use_cached_size_hint( TRUE )
    {
	l_marg = r_marg = t_marg = b_marg = 0;
	viewport->polish();
	viewport->setBackgroundMode( QWidget::PaletteDark );
	viewport->setBackgroundOrigin( QWidget::WidgetOrigin );
	vMode = QScrollView::Auto;
	hMode = QScrollView::Auto;
	corner = 0;
	defaultCorner = new QWidget( parent, "qt_default_corner" );
	defaultCorner->hide();
	vbar->setSteps( 20, 1/*set later*/ );
	hbar->setSteps( 20, 1/*set later*/ );
	policy = QScrollView::Default;
	signal_choke = FALSE;
	static_bg = FALSE;
	fake_scroll = FALSE;
	hbarPressed = FALSE;
	vbarPressed = FALSE;
    }
    ~QScrollViewData();

    QSVChildRec* rec(QWidget* w) { return childDict.find(w); }
    QSVChildRec* ancestorRec(QWidget* w);
    QSVChildRec* addChildRec(QWidget* w, int x, int y )
    {
        QSVChildRec *r = new QSVChildRec(w,x,y);
        children.append(r);
        childDict.insert(w, r);
        return r;
    }
    void deleteChildRec(QSVChildRec* r)
    {
        childDict.remove(r->child);
        children.removeRef(r);
        delete r;
    }

    void hideOrShowAll(QScrollView* sv, bool isScroll = FALSE );
    void moveAllBy(int dx, int dy);
    bool anyVisibleChildren();
    void autoMove(QScrollView* sv);
    void autoResize(QScrollView* sv);
    void autoResizeHint(QScrollView* sv);
    void viewportResized( int w, int h );

    QScrollBar*  hbar;
    QScrollBar*  vbar;
    bool hbarPressed;
    bool vbarPressed;
    QViewportWidget*	viewport;
    QClipperWidget*	clipped_viewport;
    int         flags;
    QPtrList<QSVChildRec>       children;
    QPtrDict<QSVChildRec>       childDict;
    QWidget*    corner, *defaultCorner;
    int         vx, vy, vwidth, vheight; // for drawContents-style usage
    int         l_marg, r_marg, t_marg, b_marg;
    QScrollView::ResizePolicy policy;
    QScrollView::ScrollBarMode  vMode;
    QScrollView::ScrollBarMode  hMode;
#ifndef QT_NO_DRAGANDDROP
    QPoint cpDragStart;
    QTimer autoscroll_timer;
    int autoscroll_time;
    int autoscroll_accel;
    bool drag_autoscroll;
#endif
    QTimer scrollbar_timer;

    uint static_bg : 1;
    uint fake_scroll : 1;

    // This variable allows ensureVisible to move the contents then
    // update both the sliders.  Otherwise, updating the sliders would
    // cause two image scrolls, creating ugly flashing.
    //
    uint signal_choke : 1;

    // This variables indicates in updateScrollBars() that we are
    // in a resizeEvent() and thus don't want to flash scrollbars
    uint inresize : 1;
    uint use_cached_size_hint : 1;
    QSize cachedSizeHint;

    inline int contentsX() const { return -vx; }
    inline int contentsY() const { return -vy; }
    inline int contentsWidth() const { return vwidth; }
};

inline QScrollViewData::~QScrollViewData()
{
    children.setAutoDelete( TRUE );
}

QSVChildRec* QScrollViewData::ancestorRec(QWidget* w)
{
    if ( clipped_viewport ) {
	while (w->parentWidget() != clipped_viewport) {
	    w = w->parentWidget();
	    if (!w) return 0;
	}
    } else {
	while (w->parentWidget() != viewport) {
	    w = w->parentWidget();
	    if (!w) return 0;
	}
    }
    return rec(w);
}

void QScrollViewData::hideOrShowAll(QScrollView* sv, bool isScroll )
{
    if ( !clipped_viewport )
	return;
    if ( clipped_viewport->x() <= 0
	 && clipped_viewport->y() <= 0
	 && clipped_viewport->width()+clipped_viewport->x() >=
	 viewport->width()
	 && clipped_viewport->height()+clipped_viewport->y() >=
	 viewport->height() ) {
	// clipped_viewport still covers viewport
	if( static_bg )
	    clipped_viewport->repaint( TRUE );
	else if ( ( !isScroll && !clipped_viewport->testWFlags( Qt::WStaticContents) )
		  || static_bg )
	    QApplication::postEvent( clipped_viewport,
		     new QPaintEvent( clipped_viewport->clipRegion(),
			      !clipped_viewport->testWFlags(Qt::WResizeNoErase) ) );
    } else {
	// Re-center
	int nx = ( viewport->width() - clipped_viewport->width() ) / 2;
	int ny = ( viewport->height() - clipped_viewport->height() ) / 2;
	clipped_viewport->move(nx,ny);
	// no need to update, we'll receive a paintevent after move
	// (with the safe assumption that the newly exposed area
	// covers the entire viewport)
    }
    for (QSVChildRec *r = children.first(); r; r=children.next()) {
	r->hideOrShow(sv, clipped_viewport);
    }
}

void QScrollViewData::moveAllBy(int dx, int dy)
{
    if ( clipped_viewport && !static_bg ) {
	clipped_viewport->move( clipped_viewport->x()+dx,
				clipped_viewport->y()+dy );
    } else {
	for (QSVChildRec *r = children.first(); r; r=children.next()) {
	    r->child->move(r->child->x()+dx,r->child->y()+dy);
	}
	if ( static_bg )
	    viewport->repaint( TRUE );
    }
}

bool QScrollViewData::anyVisibleChildren()
{
    for (QSVChildRec *r = children.first(); r; r=children.next()) {
	if (r->child->isVisible()) return TRUE;
    }
    return FALSE;
}

void QScrollViewData::autoMove(QScrollView* sv)
{
    if ( policy == QScrollView::AutoOne ) {
	QSVChildRec* r = children.first();
	if (r)
	    sv->setContentsPos(-r->child->x(),-r->child->y());
    }
}

void QScrollViewData::autoResize(QScrollView* sv)
{
    if ( policy == QScrollView::AutoOne ) {
	QSVChildRec* r = children.first();
	if (r)
	    sv->resizeContents(r->child->width(),r->child->height());
    }
}

void QScrollViewData::autoResizeHint(QScrollView* sv)
{
    if ( policy == QScrollView::AutoOne ) {
	QSVChildRec* r = children.first();
	if (r) {
	    QSize s = r->child->sizeHint();
	    if ( s.isValid() )
		r->child->resize(s);
	}
    } else if ( policy == QScrollView::AutoOneFit ) {
	QSVChildRec* r = children.first();
	if (r) {
	    QSize sh = r->child->sizeHint();
	    sh = sh.boundedTo( r->child->maximumSize() );
	    sv->resizeContents( sh.width(), sh.height() );
	}
    }
}

void QScrollViewData::viewportResized( int w, int h )
{
    if ( policy == QScrollView::AutoOneFit ) {
	QSVChildRec* r = children.first();
	if (r) {
	    QSize sh = r->child->sizeHint();
	    sh = sh.boundedTo( r->child->maximumSize() );
	    r->child->resize( QMAX(w,sh.width()), QMAX(h,sh.height()) );
	}

    }
}


/*!
    \class QScrollView qscrollview.h
    \brief The QScrollView widget provides a scrolling area with on-demand scroll bars.

    \ingroup abstractwidgets
    \mainclass

    The QScrollView is a large canvas - potentially larger than the
    coordinate system normally supported by the underlying window
    system. This is important because it is quite easy to go beyond
    these limitations (e.g. many web pages are more than 32000 pixels
    high). Additionally, the QScrollView can have QWidgets positioned
    on it that scroll around with the drawn content. These sub-widgets
    can also have positions outside the normal coordinate range (but
    they are still limited in size).

    To provide content for the widget, inherit from QScrollView,
    reimplement drawContents() and use resizeContents() to set the
    size of the viewed area. Use addChild() and moveChild() to
    position widgets on the view.

    To use QScrollView effectively it is important to understand its
    widget structure in the three styles of use: a single large child
    widget, a large panning area with some widgets and a large panning
    area with many widgets.

    \section1 Using One Big Widget

    \img qscrollview-vp2.png

    The first, simplest usage of QScrollView (depicted above), is
    appropriate for scrolling areas that are never more than about
    4000 pixels in either dimension (this is about the maximum
    reliable size on X11 servers). In this usage, you just make one
    large child in the QScrollView. The child should be a child of the
    viewport() of the scrollview and be added with addChild():
    \code
	QScrollView* sv = new QScrollView(...);
	QVBox* big_box = new QVBox(sv->viewport());
	sv->addChild(big_box);
    \endcode
    You can go on to add arbitrary child widgets to the single child
    in the scrollview as you would with any widget:
    \code
	QLabel* child1 = new QLabel("CHILD", big_box);
	QLabel* child2 = new QLabel("CHILD", big_box);
	QLabel* child3 = new QLabel("CHILD", big_box);
	...
    \endcode

    Here the QScrollView has four children: the viewport(), the
    verticalScrollBar(), the horizontalScrollBar() and a small
    cornerWidget(). The viewport() has one child: the big QVBox. The
    QVBox has the three QLabel objects as child widgets. When the view
    is scrolled, the QVBox is moved; its children move with it as
    child widgets normally do.

    \section1 Using a Very Big View with Some Widgets

    \img qscrollview-vp.png

    The second usage of QScrollView (depicted above) is appropriate
    when few, if any, widgets are on a very large scrolling area that
    is potentially larger than 4000 pixels in either dimension. In
    this usage you call resizeContents() to set the size of the area
    and reimplement drawContents() to paint the contents. You may also
    add some widgets by making them children of the viewport() and
    adding them with addChild() (this is the same as the process for
    the single large widget in the previous example):
    \code
	QScrollView* sv = new QScrollView(...);
	QLabel* child1 = new QLabel("CHILD", sv->viewport());
	sv->addChild(child1);
	QLabel* child2 = new QLabel("CHILD", sv->viewport());
	sv->addChild(child2);
	QLabel* child3 = new QLabel("CHILD", sv->viewport());
	sv->addChild(child3);
    \endcode
    Here, the QScrollView has the same four children: the viewport(),
    the verticalScrollBar(), the horizontalScrollBar() and a small
    cornerWidget(). The viewport() has the three QLabel objects as
    child widgets. When the view is scrolled, the scrollview moves the
    child widgets individually.

    \section1 Using a Very Big View with Many Widgets

    \target enableclipper
    \img qscrollview-cl.png

    The final usage of QScrollView (depicted above) is appropriate
    when many widgets are on a very large scrolling area that is
    potentially larger than 4000 pixels in either dimension. In this
    usage you call resizeContents() to set the size of the area and
    reimplement drawContents() to paint the contents. You then call
    enableClipper(TRUE) and add widgets, again by making them children
    of the viewport(), and adding them with addChild():
    \code
	QScrollView* sv = new QScrollView(...);
	sv->enableClipper(TRUE);
	QLabel* child1 = new QLabel("CHILD", sv->viewport());
	sv->addChild(child1);
	QLabel* child2 = new QLabel("CHILD", sv->viewport());
	sv->addChild(child2);
	QLabel* child3 = new QLabel("CHILD", sv->viewport());
	sv->addChild(child3);
    \endcode

    Here, the QScrollView has four children:  the clipper() (not the
    viewport() this time), the verticalScrollBar(), the
    horizontalScrollBar() and a small cornerWidget(). The clipper()
    has one child: the viewport(). The viewport() has the same three
    labels as child widgets. When the view is scrolled the viewport()
    is moved; its children move with it as child widgets normally do.

    \target allviews
    \section1 Details Relevant for All Views

    Normally you will use the first or third method if you want any
    child widgets in the view.

    Note that the widget you see in the scrolled area is the
    viewport() widget, not the QScrollView itself. So to turn mouse
    tracking on, for example, use viewport()->setMouseTracking(TRUE).

    To enable drag-and-drop, you would setAcceptDrops(TRUE) on the
    QScrollView (because drag-and-drop events propagate to the
    parent). But to work out the logical position in the view, you
    would need to map the drop co-ordinate from being relative to the
    QScrollView to being relative to the contents; use the function
    viewportToContents() for this.

    To handle mouse events on the scrolling area, subclass scrollview
    as you would subclass other widgets, but rather than
    reimplementing mousePressEvent(), reimplement
    contentsMousePressEvent() instead. The contents specific event
    handlers provide translated events in the coordinate system of the
    scrollview. If you reimplement mousePressEvent(), you'll get
    called only when part of the QScrollView is clicked: and the only
    such part is the "corner" (if you don't set a cornerWidget()) and
    the frame; everything else is covered up by the viewport, clipper
    or scroll bars.

    When you construct a QScrollView, some of the widget flags apply
    to the viewport() instead of being sent to the QWidget constructor
    for the QScrollView. This applies to \c WNoAutoErase, \c
    WStaticContents, and \c WPaintClever. See \l Qt::WidgetFlags for
    documentation about these flags. Here are some examples:

    \list

    \i An image-manipulation widget would use \c
    WNoAutoErase|WStaticContents because the widget draws all pixels
    itself, and when its size increases, it only needs a paint event
    for the new part because the old part remains unchanged.

    \i A scrolling game widget in which the background scrolls as the
    characters move might use \c WNoAutoErase (in addition to \c
    WStaticContents) so that the window system background does not
    flash in and out during scrolling.

    \i A word processing widget might use \c WNoAutoErase and repaint
    itself line by line to get a less-flickery resizing. If the widget
    is in a mode in which no text justification can take place, it
    might use \c WStaticContents too, so that it would only get a
    repaint for the newly visible parts.

    \endlist

    Child widgets may be moved using addChild() or moveChild(). Use
    childX() and childY() to get the position of a child widget.

    A widget may be placed in the corner between the vertical and
    horizontal scrollbars with setCornerWidget(). You can get access
    to the scrollbars using horizontalScrollBar() and
    verticalScrollBar(), and to the viewport with viewport(). The
    scroll view can be scrolled using scrollBy(), ensureVisible(),
    setContentsPos() or center().

    The visible area is given by visibleWidth() and visibleHeight(),
    and the contents area by contentsWidth() and contentsHeight(). The
    contents may be repainted using one of the repaintContents() or
    updateContents() functions.

    Coordinate conversion is provided by contentsToViewport() and
    viewportToContents().

    The contentsMoving() signal is emitted just before the contents
    are moved to a new position.

    \warning QScrollView currently does not erase the background when
    resized, i.e. you must always clear the background manually in
    scrollview subclasses. This will change in a future version of Qt
    and we recommend specifying the WNoAutoErase flag explicitly.

    <img src=qscrollview-m.png> <img src=qscrollview-w.png>
*/


/*!
    \enum QScrollView::ResizePolicy

    This enum type is used to control a QScrollView's reaction to
    resize events.

    \value Default  the QScrollView selects one of the other settings
    automatically when it has to. In this version of Qt, QScrollView
    changes to \c Manual if you resize the contents with
    resizeContents() and to \c AutoOne if a child is added.

    \value Manual  the contents stays the size set by resizeContents().

    \value AutoOne  if there is only one child widget the contents stays
    the size of that widget. Otherwise the behavior is undefined.

    \value AutoOneFit if there is only one child widget the contents stays
    the size of that widget's sizeHint(). If the scrollview is resized
    larger than the child's sizeHint(), the child will be resized to
    fit. If there is more than one child, the behavior is undefined.

*/
//####  The widget will be resized to its sizeHint() when a LayoutHint event
//#### is received

/*!
    Constructs a QScrollView called \a name with parent \a parent and
    widget flags \a f.

    The widget flags \c WStaticContents, \c WNoAutoErase and \c
    WPaintClever are propagated to the viewport() widget. The other
    widget flags are propagated to the parent constructor as usual.
*/

QScrollView::QScrollView( QWidget *parent, const char *name, WFlags f ) :
    QFrame( parent, name, f & (~WStaticContents) & (~WResizeNoErase) )
{
    WFlags flags = WResizeNoErase | (f&WPaintClever) | (f&WRepaintNoErase) | (f&WStaticContents);
    d = new QScrollViewData( this, flags );

#ifndef QT_NO_DRAGANDDROP
    connect( &d->autoscroll_timer, SIGNAL( timeout() ),
             this, SLOT( doDragAutoScroll() ) );
#endif

    connect( d->hbar, SIGNAL( valueChanged(int) ),
        this, SLOT( hslide(int) ) );
    connect( d->vbar, SIGNAL( valueChanged(int) ),
        this, SLOT( vslide(int) ) );

    connect( d->hbar, SIGNAL(sliderPressed()), this, SLOT(hbarIsPressed()) );
    connect( d->hbar, SIGNAL(sliderReleased()), this, SLOT(hbarIsReleased()) );
    connect( d->vbar, SIGNAL(sliderPressed()), this, SLOT(vbarIsPressed()) );
    connect( d->vbar, SIGNAL(sliderReleased()), this, SLOT(vbarIsReleased()) );


    d->viewport->installEventFilter( this );

    connect( &d->scrollbar_timer, SIGNAL( timeout() ),
             this, SLOT( updateScrollBars() ) );

    setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    setLineWidth( style().pixelMetric(QStyle::PM_DefaultFrameWidth, this) );
    setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );
}


/*!
    Destroys the QScrollView. Any children added with addChild() will
    be deleted.
*/
QScrollView::~QScrollView()
{
    // Be careful not to get all those useless events...
    if ( d->clipped_viewport )
        d->clipped_viewport->removeEventFilter( this );
    else
        d->viewport->removeEventFilter( this );

    // order is important
    // ~QWidget may cause a WM_ERASEBKGND on Windows
    delete d->vbar;
    d->vbar = 0;
    delete d->hbar;
    d->hbar = 0;
    delete d->viewport;
    d->viewport = 0;
    delete d;
    d = 0;
}

/*!
    \fn void QScrollView::horizontalSliderPressed()

    This signal is emitted whenever the user presses the horizontal slider.
*/
/*!
    \fn void QScrollView::horizontalSliderReleased()

    This signal is emitted whenever the user releases the horizontal slider.
*/
/*!
    \fn void QScrollView::verticalSliderPressed()

    This signal is emitted whenever the user presses the vertical slider.
*/
/*!
    \fn void QScrollView::verticalSliderReleased()

    This signal is emitted whenever the user releases the vertical slider.
*/
void QScrollView::hbarIsPressed()
{
    d->hbarPressed = TRUE;
    emit( horizontalSliderPressed() );
}

void QScrollView::hbarIsReleased()
{
    d->hbarPressed = FALSE;
    emit( horizontalSliderReleased() );
}

/*!
    Returns TRUE if horizontal slider is pressed by user; otherwise returns FALSE.
*/
bool QScrollView::isHorizontalSliderPressed()
{
    return d->hbarPressed;
}

void QScrollView::vbarIsPressed()
{
    d->vbarPressed = TRUE;
    emit( verticalSliderPressed() );
}

void QScrollView::vbarIsReleased()
{
    d->vbarPressed = FALSE;
    emit( verticalSliderReleased() );
}

/*!
    Returns TRUE if vertical slider is pressed by user; otherwise returns FALSE.
*/
bool QScrollView::isVerticalSliderPressed()
{
    return d->vbarPressed;
}

/*!
    \reimp
*/
void QScrollView::styleChange( QStyle& old )
{
    QWidget::styleChange( old );
    updateScrollBars();
    d->cachedSizeHint = QSize();
}

/*!
    \reimp
*/
void QScrollView::fontChange( const QFont &old )
{
    QWidget::fontChange( old );
    updateScrollBars();
    d->cachedSizeHint = QSize();
}

void QScrollView::hslide( int pos )
{
    if ( !d->signal_choke ) {
        moveContents( -pos, -d->contentsY() );
        QApplication::syncX();
    }
}

void QScrollView::vslide( int pos )
{
    if ( !d->signal_choke ) {
        moveContents( -d->contentsX(), -pos );
        QApplication::syncX();
    }
}

/*!
    Called when the horizontal scroll bar geometry changes. This is
    provided as a protected function so that subclasses can do
    interesting things such as providing extra buttons in some of the
    space normally used by the scroll bars.

    The default implementation simply gives all the space to \a hbar.
    The new geometry is given by \a x, \a y, \a w and \a h.

    \sa setVBarGeometry()
*/
void QScrollView::setHBarGeometry(QScrollBar& hbar,
    int x, int y, int w, int h)
{
    hbar.setGeometry( x, y, w, h );
}

/*!
    Called when the vertical scroll bar geometry changes. This is
    provided as a protected function so that subclasses can do
    interesting things such as providing extra buttons in some of the
    space normally used by the scroll bars.

    The default implementation simply gives all the space to \a vbar.
    The new geometry is given by \a x, \a y, \a w and \a h.

    \sa setHBarGeometry()
*/
void QScrollView::setVBarGeometry( QScrollBar& vbar,
    int x, int y, int w, int h)
{
    vbar.setGeometry( x, y, w, h );
}


/*!
    Returns the viewport size for size (\a x, \a y).

    The viewport size depends on \a (x, y) (the size of the contents),
    the size of this widget and the modes of the horizontal and
    vertical scroll bars.

    This function permits widgets that can trade vertical and
    horizontal space for each other to control scroll bar appearance
    better. For example, a word processor or web browser can control
    the width of the right margin accurately, whether or not there
    needs to be a vertical scroll bar.
*/

QSize QScrollView::viewportSize( int x, int y ) const
{
    int fw = frameWidth();
    int lmarg = fw+d->l_marg;
    int rmarg = fw+d->r_marg;
    int tmarg = fw+d->t_marg;
    int bmarg = fw+d->b_marg;

    int w = width();
    int h = height();

    bool needh, needv;
    bool showh, showv;
    int hsbExt = horizontalScrollBar()->sizeHint().height();
    int vsbExt = verticalScrollBar()->sizeHint().width();

    if ( d->policy != AutoOne || d->anyVisibleChildren() ) {
        // Do we definitely need the scrollbar?
        needh = w-lmarg-rmarg < x;
        needv = h-tmarg-bmarg < y;

        // Do we intend to show the scrollbar?
        if (d->hMode == AlwaysOn)
            showh = TRUE;
        else if (d->hMode == AlwaysOff)
            showh = FALSE;
        else
            showh = needh;

        if (d->vMode == AlwaysOn)
            showv = TRUE;
        else if (d->vMode == AlwaysOff)
            showv = FALSE;
        else
            showv = needv;

        // Given other scrollbar will be shown, NOW do we need one?
        if ( showh && h-vsbExt-tmarg-bmarg < y ) {
            if (d->vMode == Auto)
                showv=TRUE;
        }
        if ( showv && w-hsbExt-lmarg-rmarg < x ) {
            if (d->hMode == Auto)
                showh=TRUE;
        }
    } else {
        // Scrollbars not needed, only show scrollbar that are always on.
        showh = d->hMode == AlwaysOn;
        showv = d->vMode == AlwaysOn;
    }

    return QSize( w-lmarg-rmarg - (showv ? vsbExt : 0),
                  h-tmarg-bmarg - (showh ? hsbExt : 0) );
}


/*!
    Updates scroll bars: all possibilities are considered. You should
    never need to call this in your code.
*/
void QScrollView::updateScrollBars()
{
    if(!horizontalScrollBar() && !verticalScrollBar())
	return;

    // I support this should use viewportSize()... but it needs
    // so many of the temporary variables from viewportSize.  hm.
    int fw = frameWidth();
    int lmarg = fw+d->l_marg;
    int rmarg = fw+d->r_marg;
    int tmarg = fw+d->t_marg;
    int bmarg = fw+d->b_marg;

    int w = width();
    int h = height();

    int portw, porth;

    bool needh;
    bool needv;
    bool showh;
    bool showv;
    bool showc = FALSE;

    int hsbExt = horizontalScrollBar()->sizeHint().height();
    int vsbExt = verticalScrollBar()->sizeHint().width();

    QSize oldVisibleSize( visibleWidth(), visibleHeight() );

    if ( d->policy != AutoOne || d->anyVisibleChildren() ) {
        // Do we definitely need the scrollbar?
        needh = w-lmarg-rmarg < d->contentsWidth();
        if ( d->inresize )
            needh  = !horizontalScrollBar()->isHidden();
        needv = h-tmarg-bmarg < contentsHeight();

        // Do we intend to show the scrollbar?
        if (d->hMode == AlwaysOn)
            showh = TRUE;
        else if (d->hMode == AlwaysOff)
            showh = FALSE;
        else
            showh = needh;

        if (d->vMode == AlwaysOn)
            showv = TRUE;
        else if (d->vMode == AlwaysOff)
            showv = FALSE;
        else
            showv = needv;

#ifdef Q_WS_MAC
	bool mac_need_scroll = FALSE;
	if(!parentWidget()) {
	    mac_need_scroll = TRUE;
	} else {
	    QWidget *tlw = topLevelWidget();
	    QPoint tlw_br = QPoint(tlw->width(), tlw->height()),
		    my_br = posInWindow(this) + QPoint(w, h);
	    if(my_br.x() >= tlw_br.x() - 3 && my_br.y() >= tlw_br.y() - 3)
		mac_need_scroll = TRUE;
	}
	if(mac_need_scroll) {
	    WindowAttributes attr;
	    GetWindowAttributes((WindowPtr)handle(), &attr);
	    mac_need_scroll = (attr & kWindowResizableAttribute);
	}
	if(mac_need_scroll) {
	    showc = TRUE;
	    if(d->vMode == Auto)
		showv = TRUE;
	    if(d->hMode == Auto)
		showh = TRUE;
	}
#endif

        // Given other scrollbar will be shown, NOW do we need one?
        if ( showh && h-vsbExt-tmarg-bmarg < contentsHeight() ) {
            needv=TRUE;
            if (d->vMode == Auto)
                showv=TRUE;
        }
        if ( showv && !d->inresize && w-hsbExt-lmarg-rmarg < d->contentsWidth() ) {
            needh=TRUE;
            if (d->hMode == Auto)
                showh=TRUE;
        }
    } else {
        // Scrollbars not needed, only show scrollbar that are always on.
        needh = needv = FALSE;
        showh = d->hMode == AlwaysOn;
        showv = d->vMode == AlwaysOn;
    }

    bool sc = d->signal_choke;
    d->signal_choke=TRUE;

    // Hide unneeded scrollbar, calculate viewport size
    if ( showh ) {
        porth=h-hsbExt-tmarg-bmarg;
    } else {
        if (!needh)
            d->hbar->setValue(0);
        d->hbar->hide();
        porth=h-tmarg-bmarg;
    }
    if ( showv ) {
        portw=w-vsbExt-lmarg-rmarg;
    } else {
        if (!needv)
            d->vbar->setValue(0);
        d->vbar->hide();
        portw=w-lmarg-rmarg;
    }

    // Configure scrollbars that we will show
    if ( needv ) {
	d->vbar->setRange( 0, contentsHeight()-porth );
	d->vbar->setSteps( QScrollView::d->vbar->lineStep(), porth );
    } else {
	d->vbar->setRange( 0, 0 );
    }
    if ( needh ) {
	d->hbar->setRange( 0, QMAX(0, d->contentsWidth()-portw) );
	d->hbar->setSteps( QScrollView::d->hbar->lineStep(), portw );
    } else {
	d->hbar->setRange( 0, 0 );
    }

    // Position the scrollbars, viewport and corner widget.
    int bottom;
    bool reverse = QApplication::reverseLayout();
    int xoffset = ( reverse && (showv || cornerWidget() )) ? vsbExt : 0;
    int xpos = reverse ? 0 : w - vsbExt;
    bool frameContentsOnly =
	style().styleHint(QStyle::SH_ScrollView_FrameOnlyAroundContents);

    if( ! frameContentsOnly ) {
	if ( reverse )
            xpos += fw;
        else
            xpos -= fw;
    }
    if ( showh ) {
        int right = ( showc || showv || cornerWidget() ) ? w-vsbExt : w;
        if ( ! frameContentsOnly )
            setHBarGeometry( *d->hbar, fw + xoffset, h-hsbExt-fw,
			     right-fw-fw, hsbExt );
        else
            setHBarGeometry( *d->hbar, 0 + xoffset, h-hsbExt, right,
			     hsbExt );
        bottom=h-hsbExt;
    } else {
        bottom=h;
    }
    if ( showv ) {
	clipper()->setGeometry( lmarg + xoffset, tmarg,
				w-vsbExt-lmarg-rmarg,
				bottom-tmarg-bmarg );
	d->viewportResized( w-vsbExt-lmarg-rmarg, bottom-tmarg-bmarg );
	if ( ! frameContentsOnly )
	    changeFrameRect(QRect(0, 0, w, h) );
	else
	    changeFrameRect(QRect(xoffset, 0, w-vsbExt, bottom));
	if (showc || cornerWidget()) {
	    if ( ! frameContentsOnly )
		setVBarGeometry( *d->vbar, xpos,
				 fw, vsbExt,
				 h-hsbExt-fw-fw );
	    else
		setVBarGeometry( *d->vbar, xpos, 0,
				 vsbExt,
				 h-hsbExt );
	}
	else {
	    if ( ! frameContentsOnly )
		setVBarGeometry( *d->vbar, xpos,
				 fw, vsbExt,
				 bottom-fw-fw );
	    else
		setVBarGeometry( *d->vbar, xpos, 0,
				 vsbExt, bottom );
	}
    } else {
        if ( ! frameContentsOnly )
            changeFrameRect(QRect(0, 0, w, h));
        else
            changeFrameRect(QRect(0, 0, w, bottom));
        clipper()->setGeometry( lmarg, tmarg,
				w-lmarg-rmarg, bottom-tmarg-bmarg );
        d->viewportResized( w-lmarg-rmarg, bottom-tmarg-bmarg );
    }

    QWidget *corner = d->corner;
    if ( !d->corner )
	corner = d->defaultCorner;
    if ( ! frameContentsOnly )
	corner->setGeometry( xpos,
			     h-hsbExt-fw,
			     vsbExt,
			     hsbExt );
    else
	corner->setGeometry( xpos,
			     h-hsbExt,
			     vsbExt,
			     hsbExt );

    d->signal_choke=sc;

    if ( d->contentsX()+visibleWidth() > d->contentsWidth() ) {
        int x;
#if 0
        if ( reverse )
            x =QMIN(0,d->contentsWidth()-visibleWidth());
        else
#endif
            x =QMAX(0,d->contentsWidth()-visibleWidth());
        d->hbar->setValue(x);
        // Do it even if it is recursive
        moveContents( -x, -d->contentsY() );
    }
    if ( d->contentsY()+visibleHeight() > contentsHeight() ) {
        int y=QMAX(0,contentsHeight()-visibleHeight());
        d->vbar->setValue(y);
        // Do it even if it is recursive
        moveContents( -d->contentsX(), -y );
    }

    // Finally, show the scroll bars
    if ( showh && ( d->hbar->isHidden() || !d->hbar->isVisible() ) )
        d->hbar->show();
    if ( showv && ( d->vbar->isHidden() || !d->vbar->isVisible() ) )
        d->vbar->show();

    d->signal_choke=TRUE;
    d->vbar->setValue( d->contentsY() );
    d->hbar->setValue( d->contentsX() );
    d->signal_choke=FALSE;

    QSize newVisibleSize( visibleWidth(), visibleHeight() );
    if ( d->clipped_viewport && oldVisibleSize != newVisibleSize ) {
	QResizeEvent e( newVisibleSize, oldVisibleSize );
	viewportResizeEvent( &e );
    }
}


/*!
    \reimp
*/
void QScrollView::show()
{
    if ( isVisible() )
	return;
    QWidget::show();
    updateScrollBars();
    d->hideOrShowAll(this);
}

/*!
    \reimp
 */
void QScrollView::resize( int w, int h )
{
    QWidget::resize( w, h );
}

/*!
    \reimp
*/
void QScrollView::resize( const QSize& s )
{
    resize( s.width(), s.height() );
}

/*!
    \reimp
*/
void QScrollView::resizeEvent( QResizeEvent* event )
{
    QFrame::resizeEvent( event );

#if 0
    if ( QApplication::reverseLayout() ) {
        d->fake_scroll = TRUE;
        scrollBy( -event->size().width() + event->oldSize().width(), 0 );
        d->fake_scroll = FALSE;
    }
#endif

    bool inresize = d->inresize;
    d->inresize = TRUE;
    updateScrollBars();
    d->inresize = inresize;
    d->scrollbar_timer.start( 0, TRUE );

    d->hideOrShowAll(this);
}



/*!
    \reimp
*/
void  QScrollView::mousePressEvent( QMouseEvent * e) //#### remove for 4.0
{
    e->ignore();
}

/*!
    \reimp
*/
void  QScrollView::mouseReleaseEvent( QMouseEvent *e ) //#### remove for 4.0
{
    e->ignore();
}


/*!
    \reimp
*/
void  QScrollView::mouseDoubleClickEvent( QMouseEvent *e ) //#### remove for 4.0
{
    e->ignore();
}

/*!
    \reimp
*/
void  QScrollView::mouseMoveEvent( QMouseEvent *e ) //#### remove for 4.0
{
    e->ignore();
}

/*!
    \reimp
*/
#ifndef QT_NO_WHEELEVENT
void QScrollView::wheelEvent( QWheelEvent *e )
{
    QWheelEvent ce( viewport()->mapFromGlobal( e->globalPos() ),
                    e->globalPos(), e->delta(), e->state());
    viewportWheelEvent(&ce);
    if ( !ce.isAccepted() ) {
	if ( e->orientation() == Horizontal && horizontalScrollBar() )
	    QApplication::sendEvent( horizontalScrollBar(), e);
	else  if (e->orientation() == Vertical && verticalScrollBar() )
	    QApplication::sendEvent( verticalScrollBar(), e);
    } else {
	e->accept();
    }
}
#endif

/*!
    \reimp
*/
void QScrollView::contextMenuEvent( QContextMenuEvent *e )
{
    if ( e->reason() != QContextMenuEvent::Keyboard ) {
	e->ignore();
        return;
    }

    QContextMenuEvent ce( e->reason(), viewport()->mapFromGlobal( e->globalPos() ),
                          e->globalPos(), e->state() );
    viewportContextMenuEvent( &ce );
    if ( ce.isAccepted() )
        e->accept();
    else
        e->ignore();
}

QScrollView::ScrollBarMode QScrollView::vScrollBarMode() const
{
    return d->vMode;
}


/*!
    \enum QScrollView::ScrollBarMode

    This enum type describes the various modes of QScrollView's scroll
    bars.

    \value Auto  QScrollView shows a scroll bar when the content is
    too large to fit and not otherwise. This is the default.

    \value AlwaysOff  QScrollView never shows a scroll bar.

    \value AlwaysOn  QScrollView always shows a scroll bar.

    (The modes for the horizontal and vertical scroll bars are
    independent.)
*/


/*!
    \property QScrollView::vScrollBarMode
    \brief the mode for the vertical scroll bar

    The default mode is \c QScrollView::Auto.

    \sa hScrollBarMode
*/
void  QScrollView::setVScrollBarMode( ScrollBarMode mode )
{
    if (d->vMode != mode) {
        d->vMode = mode;
        updateScrollBars();
    }
}


/*!
    \property QScrollView::hScrollBarMode
    \brief the mode for the horizontal scroll bar

    The default mode is \c QScrollView::Auto.

    \sa vScrollBarMode
*/
QScrollView::ScrollBarMode QScrollView::hScrollBarMode() const
{
    return d->hMode;
}

void QScrollView::setHScrollBarMode( ScrollBarMode mode )
{
    if (d->hMode != mode) {
        d->hMode = mode;
        updateScrollBars();
    }
}


/*!
    Returns the widget in the corner between the two scroll bars.

    By default, no corner widget is present.
*/
QWidget* QScrollView::cornerWidget() const
{
    return d->corner;
}

/*!
    Sets the widget in the \a corner between the two scroll bars.

    You will probably also want to set at least one of the scroll bar
    modes to \c AlwaysOn.

    Passing 0 shows no widget in the corner.

    Any previous \a corner widget is hidden.

    You may call setCornerWidget() with the same widget at different
    times.

    All widgets set here will be deleted by the QScrollView when it is
    destroyed unless you separately reparent the widget after setting
    some other corner widget (or 0).

    Any \e newly set widget should have no current parent.

    By default, no corner widget is present.

    \sa setVScrollBarMode(), setHScrollBarMode()
*/
void QScrollView::setCornerWidget(QWidget* corner)
{
    QWidget* oldcorner = d->corner;
    if (oldcorner != corner) {
        if (oldcorner) oldcorner->hide();
        d->corner = corner;

        if ( corner && corner->parentWidget() != this ) {
            // #### No clean way to get current WFlags
            corner->reparent( this, (((QScrollView*)corner))->getWFlags(),
                              QPoint(0,0), FALSE );
        }

        updateScrollBars();
        if ( corner ) corner->show();
    }
}


void QScrollView::setResizePolicy( ResizePolicy r )
{
    d->policy = r;
}

/*!
    \property QScrollView::resizePolicy
    \brief the resize policy

    The default is \c Default.

    \sa ResizePolicy
*/
QScrollView::ResizePolicy QScrollView::resizePolicy() const
{
    return d->policy;
}

/*!
    \reimp
*/
void QScrollView::setEnabled( bool enable )
{
    QFrame::setEnabled( enable );
}

/*!
    Removes the \a child widget from the scrolled area. Note that this
    happens automatically if the \a child is deleted.
*/
void QScrollView::removeChild(QWidget* child)
{
    if ( !d || !child ) // First check in case we are destructing
        return;

    QSVChildRec *r = d->rec(child);
    if ( r ) d->deleteChildRec( r );
}

/*!
    \reimp
*/
void QScrollView::removeChild(QObject* child)
{
    QFrame::removeChild(child);
}

/*!
    Inserts the widget, \a child, into the scrolled area positioned at
    (\a x, \a y). The position defaults to (0, 0). If the child is
    already in the view, it is just moved.

    You may want to call enableClipper(TRUE) if you add a large number
    of widgets.
*/
void QScrollView::addChild(QWidget* child, int x, int y)
{
    if ( !child ) {
#if defined(QT_CHECK_NULL)
	qWarning( "QScrollView::addChild(): Cannot add null child" );
#endif
	return;
    }
    child->polish();
    child->setBackgroundOrigin(WidgetOrigin);

    if ( child->parentWidget() == viewport() ) {
        // May already be there
        QSVChildRec *r = d->rec(child);
        if (r) {
            r->moveTo(this,x,y,d->clipped_viewport);
            if ( d->policy > Manual ) {
                d->autoResizeHint(this);
                d->autoResize(this); // #### better to just deal with this one widget!
            }
            return;
        }
    }

    if ( d->children.isEmpty() && d->policy != Manual ) {
        if ( d->policy == Default )
            setResizePolicy( AutoOne );
        child->installEventFilter( this );
    } else if ( d->policy == AutoOne ) {
        child->removeEventFilter( this ); //#### ?????
        setResizePolicy( Manual );
    }
    if ( child->parentWidget() != viewport() ) {
            child->reparent( viewport(), 0, QPoint(0,0), FALSE );
    }
    d->addChildRec(child,x,y)->hideOrShow(this, d->clipped_viewport);

    if ( d->policy > Manual ) {
        d->autoResizeHint(this);
        d->autoResize(this); // #### better to just deal with this one widget!
    }
}

/*!
    Repositions the \a child widget to (\a x, \a y). This function is
    the same as addChild().
*/
void QScrollView::moveChild(QWidget* child, int x, int y)
{
    addChild(child,x,y);
}

/*!
    Returns the X position of the given \a child widget. Use this
    rather than QWidget::x() for widgets added to the view.

    This function returns 0 if \a child has not been added to the view.
*/
int QScrollView::childX(QWidget* child)
{
    QSVChildRec *r = d->rec(child);
    return r ? r->x : 0;
}

/*!
    Returns the Y position of the given \a child widget. Use this
    rather than QWidget::y() for widgets added to the view.

    This function returns 0 if \a child has not been added to the view.
*/
int QScrollView::childY(QWidget* child)
{
    QSVChildRec *r = d->rec(child);
    return r ? r->y : 0;
}

/*! \fn bool QScrollView::childIsVisible(QWidget*)
  \obsolete

  Returns TRUE if \a child is visible. This is equivalent
  to child->isVisible().
*/

/*! \fn void QScrollView::showChild(QWidget* child, bool y)
  \obsolete

  Sets the visibility of \a child. Equivalent to
  QWidget::show() or QWidget::hide().
*/

/*!
    This event filter ensures the scroll bars are updated when a
    single contents widget is resized, shown, hidden or destroyed; it
    passes mouse events to the QScrollView. The event is in \a e and
    the object is in \a obj.
*/

bool QScrollView::eventFilter( QObject *obj, QEvent *e )
{
    if ( !d )
	return FALSE; // we are destructing
    if ( obj == d->viewport || obj == d->clipped_viewport ) {
        switch ( e->type() ) {
            /* Forward many events to viewport...() functions */
        case QEvent::Paint:
            viewportPaintEvent( (QPaintEvent*)e );
            break;
        case QEvent::Resize:
	    if ( !d->clipped_viewport )
		viewportResizeEvent( (QResizeEvent *)e );
            break;
        case QEvent::MouseButtonPress:
            viewportMousePressEvent( (QMouseEvent*)e );
	    if ( ((QMouseEvent*)e)->isAccepted() )
		return TRUE;
            break;
        case QEvent::MouseButtonRelease:
            viewportMouseReleaseEvent( (QMouseEvent*)e );
	    if ( ((QMouseEvent*)e)->isAccepted() )
		return TRUE;
            break;
        case QEvent::MouseButtonDblClick:
            viewportMouseDoubleClickEvent( (QMouseEvent*)e );
	    if ( ((QMouseEvent*)e)->isAccepted() )
		return TRUE;
            break;
        case QEvent::MouseMove:
            viewportMouseMoveEvent( (QMouseEvent*)e );
	    if ( ((QMouseEvent*)e)->isAccepted() )
		return TRUE;
            break;
#ifndef QT_NO_DRAGANDDROP
        case QEvent::DragEnter:
            viewportDragEnterEvent( (QDragEnterEvent*)e );
            break;
        case QEvent::DragMove: {
            if ( d->drag_autoscroll ) {
                QPoint vp = ((QDragMoveEvent*) e)->pos();
                QRect inside_margin( autoscroll_margin, autoscroll_margin,
                                     visibleWidth() - autoscroll_margin * 2,
                                     visibleHeight() - autoscroll_margin * 2 );
                if ( !inside_margin.contains( vp ) ) {
                    startDragAutoScroll();
                    // Keep sending move events
                    ( (QDragMoveEvent*)e )->accept( QRect(0,0,0,0) );
                }
            }
            viewportDragMoveEvent( (QDragMoveEvent*)e );
        } break;
        case QEvent::DragLeave:
            stopDragAutoScroll();
            viewportDragLeaveEvent( (QDragLeaveEvent*)e );
            break;
        case QEvent::Drop:
            stopDragAutoScroll();
            viewportDropEvent( (QDropEvent*)e );
            break;
#endif // QT_NO_DRAGANDDROP
        case QEvent::ContextMenu:
            viewportContextMenuEvent( (QContextMenuEvent*)e );
	    if ( ((QContextMenuEvent*)e)->isAccepted() )
		return TRUE;
            break;
        case QEvent::ChildRemoved:
            removeChild((QWidget*)((QChildEvent*)e)->child());
            break;
        case QEvent::LayoutHint:
            d->autoResizeHint(this);
            break;
        default:
            break;
        }
    } else if ( d && d->rec((QWidget*)obj) ) {  // must be a child
        if ( e->type() == QEvent::Resize )
            d->autoResize(this);
        else if ( e->type() == QEvent::Move )
            d->autoMove(this);
    }
    return QFrame::eventFilter( obj, e );  // always continue with standard event processing
}

/*!
    This event handler is called whenever the QScrollView receives a
    mousePressEvent(): the press position in \a e is translated to be a point
    on the contents.
*/
void QScrollView::contentsMousePressEvent( QMouseEvent* e )
{
    e->ignore();
}

/*!
    This event handler is called whenever the QScrollView receives a
    mouseReleaseEvent(): the release position in \a e is translated to be a
    point on the contents.
*/
void QScrollView::contentsMouseReleaseEvent( QMouseEvent* e )
{
    e->ignore();
}

/*!
    This event handler is called whenever the QScrollView receives a
    mouseDoubleClickEvent(): the click position in \a e is translated to be a
    point on the contents.

    The default implementation generates a normal mouse press event.
*/
void QScrollView::contentsMouseDoubleClickEvent( QMouseEvent* e )
{
    contentsMousePressEvent(e);		    // try mouse press event
}

/*!
    This event handler is called whenever the QScrollView receives a
    mouseMoveEvent(): the mouse position in \a e is translated to be a point
    on the contents.
*/
void QScrollView::contentsMouseMoveEvent( QMouseEvent* e )
{
    e->ignore();
}

#ifndef QT_NO_DRAGANDDROP

/*!
    This event handler is called whenever the QScrollView receives a
    dragEnterEvent(): the drag position is translated to be a point
    on the contents.
*/
void QScrollView::contentsDragEnterEvent( QDragEnterEvent * )
{
}

/*!
    This event handler is called whenever the QScrollView receives a
    dragMoveEvent(): the drag position is translated to be a point on
    the contents.
*/
void QScrollView::contentsDragMoveEvent( QDragMoveEvent * )
{
}

/*!
    This event handler is called whenever the QScrollView receives a
    dragLeaveEvent(): the drag position is translated to be a point
    on the contents.
*/
void QScrollView::contentsDragLeaveEvent( QDragLeaveEvent * )
{
}

/*!
    This event handler is called whenever the QScrollView receives a
    dropEvent(): the drop position is translated to be a point on the
    contents.
*/
void QScrollView::contentsDropEvent( QDropEvent * )
{
}

#endif // QT_NO_DRAGANDDROP

/*!
    This event handler is called whenever the QScrollView receives a
    wheelEvent() in \a{e}: the mouse position is translated to be a
    point on the contents.
*/
#ifndef QT_NO_WHEELEVENT
void QScrollView::contentsWheelEvent( QWheelEvent * e )
{
    e->ignore();
}
#endif
/*!
    This event handler is called whenever the QScrollView receives a
    contextMenuEvent() in \a{e}: the mouse position is translated to
    be a point on the contents.
*/
void QScrollView::contentsContextMenuEvent( QContextMenuEvent *e )
{
    e->ignore();
}

/*!
    This is a low-level painting routine that draws the viewport
    contents. Reimplement this if drawContents() is too high-level
    (for example, if you don't want to open a QPainter on the
    viewport). The paint event is passed in \a pe.
*/
void QScrollView::viewportPaintEvent( QPaintEvent* pe )
{
    QWidget* vp = viewport();

    QPainter p(vp);
    QRect r = pe->rect();

    if ( d->clipped_viewport ) {
	QRect rr(
	    -d->clipped_viewport->x(), -d->clipped_viewport->y(),
	    d->viewport->width(), d->viewport->height()
	    );
	r &= rr;
	if ( r.isValid() ) {
	    int ex = r.x() + d->clipped_viewport->x() + d->contentsX();
	    int ey = r.y() + d->clipped_viewport->y() + d->contentsY();
	    int ew = r.width();
	    int eh = r.height();
	    drawContentsOffset(&p,
		d->contentsX()+d->clipped_viewport->x(),
		d->contentsY()+d->clipped_viewport->y(),
		ex, ey, ew, eh);
	}
    } else {
	r &= d->viewport->rect();
	int ex = r.x() + d->contentsX();
	int ey = r.y() + d->contentsY();
	int ew = r.width();
	int eh = r.height();
	drawContentsOffset(&p, d->contentsX(), d->contentsY(), ex, ey, ew, eh);
    }
}


/*!
    To provide simple processing of events on the contents, this
    function receives all resize events sent to the viewport.

    \sa QWidget::resizeEvent()
*/
void QScrollView::viewportResizeEvent( QResizeEvent* )
{
}

/*! \internal

  To provide simple processing of events on the contents, this
  function receives all mouse press events sent to the viewport,
  translates the event and calls contentsMousePressEvent().

  \sa contentsMousePressEvent(), QWidget::mousePressEvent()
*/
void QScrollView::viewportMousePressEvent( QMouseEvent* e )
{
    QMouseEvent ce(e->type(), viewportToContents(e->pos()),
        e->globalPos(), e->button(), e->state());
    contentsMousePressEvent(&ce);
    if ( !ce.isAccepted() )
	e->ignore();
}

/*!\internal

  To provide simple processing of events on the contents, this function
  receives all mouse release events sent to the viewport, translates
  the event and calls contentsMouseReleaseEvent().

  \sa QWidget::mouseReleaseEvent()
*/
void QScrollView::viewportMouseReleaseEvent( QMouseEvent* e )
{
    QMouseEvent ce(e->type(), viewportToContents(e->pos()),
        e->globalPos(), e->button(), e->state());
    contentsMouseReleaseEvent(&ce);
    if ( !ce.isAccepted() )
	e->ignore();
}

/*!\internal

  To provide simple processing of events on the contents, this function
  receives all mouse double click events sent to the viewport,
  translates the event and calls contentsMouseDoubleClickEvent().

  \sa QWidget::mouseDoubleClickEvent()
*/
void QScrollView::viewportMouseDoubleClickEvent( QMouseEvent* e )
{
    QMouseEvent ce(e->type(), viewportToContents(e->pos()),
        e->globalPos(), e->button(), e->state());
    contentsMouseDoubleClickEvent(&ce);
    if ( !ce.isAccepted() )
	e->ignore();
}

/*!\internal

  To provide simple processing of events on the contents, this function
  receives all mouse move events sent to the viewport, translates the
  event and calls contentsMouseMoveEvent().

  \sa QWidget::mouseMoveEvent()
*/
void QScrollView::viewportMouseMoveEvent( QMouseEvent* e )
{
    QMouseEvent ce(e->type(), viewportToContents(e->pos()),
        e->globalPos(), e->button(), e->state());
    contentsMouseMoveEvent(&ce);
    if ( !ce.isAccepted() )
	e->ignore();
}

#ifndef QT_NO_DRAGANDDROP

/*!\internal

  To provide simple processing of events on the contents, this function
  receives all drag enter events sent to the viewport, translates the
  event and calls contentsDragEnterEvent().

  \sa QWidget::dragEnterEvent()
*/
void QScrollView::viewportDragEnterEvent( QDragEnterEvent* e )
{
    e->setPoint(viewportToContents(e->pos()));
    contentsDragEnterEvent(e);
    e->setPoint(contentsToViewport(e->pos()));
}

/*!\internal

  To provide simple processing of events on the contents, this function
  receives all drag move events sent to the viewport, translates the
  event and calls contentsDragMoveEvent().

  \sa QWidget::dragMoveEvent()
*/
void QScrollView::viewportDragMoveEvent( QDragMoveEvent* e )
{
    e->setPoint(viewportToContents(e->pos()));
    contentsDragMoveEvent(e);
    e->setPoint(contentsToViewport(e->pos()));
}

/*!\internal

  To provide simple processing of events on the contents, this function
  receives all drag leave events sent to the viewport and calls
  contentsDragLeaveEvent().

  \sa QWidget::dragLeaveEvent()
*/
void QScrollView::viewportDragLeaveEvent( QDragLeaveEvent* e )
{
    contentsDragLeaveEvent(e);
}

/*!\internal

  To provide simple processing of events on the contents, this function
  receives all drop events sent to the viewport, translates the event
  and calls contentsDropEvent().

  \sa QWidget::dropEvent()
*/
void QScrollView::viewportDropEvent( QDropEvent* e )
{
    e->setPoint(viewportToContents(e->pos()));
    contentsDropEvent(e);
    e->setPoint(contentsToViewport(e->pos()));
}

#endif // QT_NO_DRAGANDDROP

/*!\internal

  To provide simple processing of events on the contents, this function
  receives all wheel events sent to the viewport, translates the
  event and calls contentsWheelEvent().

  \sa QWidget::wheelEvent()
*/
#ifndef QT_NO_WHEELEVENT
void QScrollView::viewportWheelEvent( QWheelEvent* e )
{
    /*
       Different than standard mouse events, because wheel events might
       be sent to the focus widget if the widget-under-mouse doesn't want
       the event itself.
    */
    QWheelEvent ce( viewportToContents(e->pos()),
        e->globalPos(), e->delta(), e->state());
    contentsWheelEvent(&ce);
    if ( ce.isAccepted() )
        e->accept();
    else
        e->ignore();
}
#endif

/*! \internal

  To provide simple processing of events on the contents, this function
  receives all context menu events sent to the viewport, translates the
  event and calls contentsContextMenuEvent().
*/
void QScrollView::viewportContextMenuEvent( QContextMenuEvent *e )
{
    QContextMenuEvent ce(e->reason(), viewportToContents(e->pos()), e->globalPos(), e->state() );
    contentsContextMenuEvent( &ce );
    if ( ce.isAccepted() )
        e->accept();
    else
        e->ignore();
}

/*!
    Returns the component horizontal scroll bar. It is made available
    to allow accelerators, autoscrolling, etc.

    It should not be used for other purposes.

    This function never returns 0.
*/
QScrollBar* QScrollView::horizontalScrollBar() const
{
    return d->hbar;
}

/*!
    Returns the component vertical scroll bar. It is made available to
    allow accelerators, autoscrolling, etc.

    It should not be used for other purposes.

    This function never returns 0.
*/
QScrollBar* QScrollView::verticalScrollBar() const {
    return d->vbar;
}


/*!
    Scrolls the content so that the point \a (x, y) is visible with at
    least 50-pixel margins (if possible, otherwise centered).
*/
void QScrollView::ensureVisible( int x, int y )
{
    ensureVisible(x, y, 50, 50);
}

/*!
    \overload

    Scrolls the content so that the point \a (x, y) is visible with at
    least the \a xmargin and \a ymargin margins (if possible,
    otherwise centered).
*/
void QScrollView::ensureVisible( int x, int y, int xmargin, int ymargin )
{
    int pw=visibleWidth();
    int ph=visibleHeight();

    int cx=-d->contentsX();
    int cy=-d->contentsY();
    int cw=d->contentsWidth();
    int ch=contentsHeight();

    if ( pw < xmargin*2 )
        xmargin=pw/2;
    if ( ph < ymargin*2 )
        ymargin=ph/2;

    if ( cw <= pw ) {
        xmargin=0;
        cx=0;
    }
    if ( ch <= ph ) {
        ymargin=0;
        cy=0;
    }

    if ( x < -cx+xmargin )
        cx = -x+xmargin;
    else if ( x >= -cx+pw-xmargin )
        cx = -x+pw-xmargin;

    if ( y < -cy+ymargin )
        cy = -y+ymargin;
    else if ( y >= -cy+ph-ymargin )
        cy = -y+ph-ymargin;

    if ( cx > 0 )
        cx=0;
    else if ( cx < pw-cw && cw>pw )
        cx=pw-cw;

    if ( cy > 0 )
        cy=0;
    else if ( cy < ph-ch && ch>ph )
        cy=ph-ch;

    setContentsPos( -cx, -cy );
}

/*!
    Scrolls the content so that the point \a (x, y) is in the top-left
    corner.
*/
void QScrollView::setContentsPos( int x, int y )
{
#if 0
    // bounds checking...
    if ( QApplication::reverseLayout() )
        if ( x > d->contentsWidth() - visibleWidth() ) x = d->contentsWidth() - visibleWidth();
    else
#endif
        if ( x < 0 ) x = 0;
    if ( y < 0 ) y = 0;
    // Choke signal handling while we update BOTH sliders.
    d->signal_choke=TRUE;
    moveContents( -x, -y );
    d->vbar->setValue( y );
    d->hbar->setValue( x );
    d->signal_choke=FALSE;
}

/*!
    Scrolls the content by \a dx to the left and \a dy upwards.
*/
void QScrollView::scrollBy( int dx, int dy )
{
    setContentsPos( QMAX( d->contentsX()+dx, 0 ), QMAX( d->contentsY()+dy, 0 ) );
}

/*!
    Scrolls the content so that the point \a (x, y) is in the center
    of visible area.
*/
void QScrollView::center( int x, int y )
{
    ensureVisible( x, y, 32000, 32000 );
}

/*!
    \overload

    Scrolls the content so that the point \a (x, y) is visible with
    the \a xmargin and \a ymargin margins (as fractions of visible
    the area).

    For example:
    \list
    \i Margin 0.0 allows (x, y) to be on the edge of the visible area.
    \i Margin 0.5 ensures that (x, y) is in middle 50% of the visible area.
    \i Margin 1.0 ensures that (x, y) is in the center of the the visible area.
    \endlist
*/
void QScrollView::center( int x, int y, float xmargin, float ymargin )
{
    int pw=visibleWidth();
    int ph=visibleHeight();
    ensureVisible( x, y, int( xmargin/2.0*pw+0.5 ), int( ymargin/2.0*ph+0.5 ) );
}


/*!
    \fn void QScrollView::contentsMoving(int x, int y)

    This signal is emitted just before the contents are moved to
    position \a (x, y).

    \sa contentsX(), contentsY()
*/

/*!
    Moves the contents by \a (x, y).
*/
void QScrollView::moveContents(int x, int y)
{
    if ( -x+visibleWidth() > d->contentsWidth() )
#if 0
        if( QApplication::reverseLayout() )
            x=QMAX(0,-d->contentsWidth()+visibleWidth());
        else
#endif
            x=QMIN(0,-d->contentsWidth()+visibleWidth());
    if ( -y+visibleHeight() > contentsHeight() )
        y=QMIN(0,-contentsHeight()+visibleHeight());

    int dx = x - d->vx;
    int dy = y - d->vy;

    if (!dx && !dy)
        return; // Nothing to do

    emit contentsMoving( -x, -y );

    d->vx = x;
    d->vy = y;

    if ( d->clipped_viewport || d->static_bg ) {
        // Cheap move (usually)
        d->moveAllBy(dx,dy);
    } else if ( /*dx && dy ||*/
         ( QABS(dy) * 5 > visibleHeight() * 4 ) ||
         ( QABS(dx) * 5 > visibleWidth() * 4 )
        )
    {
        // Big move
        if ( viewport()->isUpdatesEnabled() )
            viewport()->update();
        d->moveAllBy(dx,dy);
    } else if ( !d->fake_scroll || d->contentsWidth() > visibleWidth() ) {
        // Small move
        clipper()->scroll(dx,dy);
    }
    d->hideOrShowAll(this, TRUE );
}

#if (QT_VERSION-0 >= 0x040000)
#if defined(Q_CC_GNU)
#warning "Should rename contents{X,Y,Width,Height} to viewport{...}"
#endif
// Because it's the viewport rectangle that is "moving", not the contents.
#endif

/*!
    \property QScrollView::contentsX
    \brief the X coordinate of the contents that are at the left edge of
    the viewport.
*/
int QScrollView::contentsX() const
{
    return d->contentsX();
}

/*!
    \property QScrollView::contentsY
    \brief the Y coordinate of the contents that are at the top edge of
    the viewport.
*/
int QScrollView::contentsY() const
{
    return d->contentsY();
}

/*!
    \property QScrollView::contentsWidth
    \brief the width of the contents area
*/
int QScrollView::contentsWidth() const
{
    return d->contentsWidth();
}

/*!
    \property QScrollView::contentsHeight
    \brief the height of the contents area
*/
int QScrollView::contentsHeight() const
{
    return d->vheight;
}

/*!
    Sets the size of the contents area to \a w pixels wide and \a h
    pixels high and updates the viewport accordingly.
*/
void QScrollView::resizeContents( int w, int h )
{
    int ow = d->vwidth;
    int oh = d->vheight;
    d->vwidth = w;
    d->vheight = h;

    d->scrollbar_timer.start( 0, TRUE );

    if ( d->children.isEmpty() && d->policy == Default )
        setResizePolicy( Manual );

    if ( ow > w ) {
        // Swap
        int t=w;
        w=ow;
        ow=t;
    }
    // Refresh area ow..w
    if ( ow < visibleWidth() && w >= 0 ) {
        if ( ow < 0 )
            ow = 0;
        if ( w > visibleWidth() )
            w = visibleWidth();
        clipper()->update( d->contentsX()+ow, 0, w-ow, visibleHeight() );
    }

    if ( oh > h ) {
        // Swap
        int t=h;
        h=oh;
        oh=t;
    }
    // Refresh area oh..h
    if ( oh < visibleHeight() && h >= 0 ) {
        if ( oh < 0 )
            oh = 0;
        if ( h > visibleHeight() )
            h = visibleHeight();
        clipper()->update( 0, d->contentsY()+oh, visibleWidth(), h-oh);
    }
}

/*!
    Calls update() on a rectangle defined by \a x, \a y, \a w, \a h,
    translated appropriately. If the rectangle is not visible, nothing
    is repainted.

    \sa repaintContents()
*/
void QScrollView::updateContents( int x, int y, int w, int h )
{
    if ( testWState(WState_Visible|WState_BlockUpdates) != WState_Visible )
	return;

    QWidget* vp = viewport();

    // Translate
    x -= d->contentsX();
    y -= d->contentsY();

    // Clip to QCOORD space
    if ( x < 0 ) {
        w += x;
        x = 0;
    }
    if ( y < 0 ) {
        h += y;
        y = 0;
    }

    if ( w < 0 || h < 0 )
        return;
    if ( x > visibleWidth() || y > visibleHeight() )
	return;

    if ( w > visibleWidth() )
        w = visibleWidth();
    if ( h > visibleHeight() )
        h = visibleHeight();

    if ( d->clipped_viewport ) {
        // Translate clipper() to viewport()
        x -= d->clipped_viewport->x();
        y -= d->clipped_viewport->y();
    }

    vp->update( x, y, w, h );
}

/*!
    \overload

    Updates the contents in rectangle \a r
*/
void QScrollView::updateContents( const QRect& r )
{
    updateContents(r.x(), r.y(), r.width(), r.height());
}

/*!
    \overload
*/
void QScrollView::updateContents()
{
    updateContents( d->contentsX(), d->contentsY(), visibleWidth(), visibleHeight() );
}

/*!
    \overload

    Repaints the contents of rectangle \a r. If \a erase is TRUE the
    background is cleared using the background color.
*/
void QScrollView::repaintContents( const QRect& r, bool erase )
{
    repaintContents(r.x(), r.y(), r.width(), r.height(), erase);
}


/*!
    \overload

    Repaints the contents. If \a erase is TRUE the background is
    cleared using the background color.
*/
void QScrollView::repaintContents( bool erase )
{
    repaintContents( d->contentsX(), d->contentsY(), visibleWidth(), visibleHeight(), erase );
}


/*!
    Calls repaint() on a rectangle defined by \a x, \a y, \a w, \a h,
    translated appropriately. If the rectangle is not visible, nothing
    is repainted. If \a erase is TRUE the background is cleared using
    the background color.

    \sa updateContents()
*/
void QScrollView::repaintContents( int x, int y, int w, int h, bool erase )
{
    if ( testWState(WState_Visible|WState_BlockUpdates) != WState_Visible )
	return;

    QWidget* vp = viewport();

    // Translate logical to clipper()
    x -= d->contentsX();
    y -= d->contentsY();

    // Clip to QCOORD space
    if ( x < 0 ) {
        w += x;
        x = 0;
    }
    if ( y < 0 ) {
        h += y;
        y = 0;
    }

    if ( w < 0 || h < 0 )
        return;
    if ( w > visibleWidth() )
        w = visibleWidth();
    if ( h > visibleHeight() )
        h = visibleHeight();

    if ( d->clipped_viewport ) {
        // Translate clipper() to viewport()
        x -= d->clipped_viewport->x();
        y -= d->clipped_viewport->y();
    }

    vp->repaint( x, y, w, h, erase );
}


/*!
    For backward-compatibility only. It is easier to use
    drawContents(QPainter*,int,int,int,int).

    The default implementation translates the painter appropriately
    and calls drawContents(QPainter*,int,int,int,int). See
    drawContents() for an explanation of the parameters \a p, \a
    offsetx, \a offsety, \a clipx, \a clipy, \a clipw and \a cliph.
*/
void QScrollView::drawContentsOffset(QPainter* p, int offsetx, int offsety, int clipx, int clipy, int clipw, int cliph)
{
    p->translate(-offsetx,-offsety);
    drawContents(p, clipx, clipy, clipw, cliph);
}

/*!
    \fn void QScrollView::drawContents(QPainter* p, int clipx, int clipy, int clipw, int cliph)

    Reimplement this function if you are viewing a drawing area rather
    than a widget.

    The function should draw the rectangle (\a clipx, \a clipy, \a
    clipw, \a cliph) of the contents using painter \a p. The clip
    rectangle is in the scrollview's coordinates.

    For example:
    \code
    {
	// Fill a 40000 by 50000 rectangle at (100000,150000)

	// Calculate the coordinates...
	int x1 = 100000, y1 = 150000;
	int x2 = x1+40000-1, y2 = y1+50000-1;

	// Clip the coordinates so X/Windows will not have problems...
	if (x1 < clipx) x1=clipx;
	if (y1 < clipy) y1=clipy;
	if (x2 > clipx+clipw-1) x2=clipx+clipw-1;
	if (y2 > clipy+cliph-1) y2=clipy+cliph-1;

	// Paint using the small coordinates...
	if ( x2 >= x1 && y2 >= y1 )
	    p->fillRect(x1, y1, x2-x1+1, y2-y1+1, red);
    }
    \endcode

    The clip rectangle and translation of the painter \a p is already
    set appropriately.
*/
void QScrollView::drawContents(QPainter*, int, int, int, int)
{
}


/*!
    \reimp
*/
void QScrollView::frameChanged()
{
    updateScrollBars();
}


/*!
    Returns the viewport widget of the scrollview. This is the widget
    containing the contents widget or which is the drawing area.
*/
QWidget* QScrollView::viewport() const
{
    if ( d->clipped_viewport )
	return  d->clipped_viewport;
    return d->viewport;
}

/*!
    Returns the clipper widget. Contents in the scrollview are
    ultimately clipped to be inside the clipper widget.

    You should not need to use this function.

    \sa visibleWidth(), visibleHeight()
*/
QWidget* QScrollView::clipper() const
{
    return d->viewport;
}

/*!
    \property QScrollView::visibleWidth
    \brief the horizontal amount of the content that is visible
*/
int QScrollView::visibleWidth() const
{
    return clipper()->width();
}

/*!
    \property QScrollView::visibleHeight
    \brief the vertical amount of the content that is visible
*/
int QScrollView::visibleHeight() const
{
    return clipper()->height();
}


void QScrollView::changeFrameRect(const QRect& r)
{
    QRect oldr = frameRect();
    if (oldr != r) {
        QRect cr = contentsRect();
        QRegion fr( frameRect() );
        fr = fr.subtract( contentsRect() );
        setFrameRect( r );
        if ( isVisible() ) {
            cr = cr.intersect( contentsRect() );
            fr = fr.unite( frameRect() );
            fr = fr.subtract( cr );
            if ( !fr.isEmpty() )
                QApplication::postEvent( this, new QPaintEvent( fr, FALSE ) );
        }
    }
}


/*!
    Sets the margins around the scrolling area to \a left, \a top, \a
    right and \a bottom. This is useful for applications such as
    spreadsheets with "locked" rows and columns. The marginal space is
    \e inside the frameRect() and is left blank; reimplement
    drawFrame() or put widgets in the unused area.

    By default all margins are zero.

    \sa frameChanged()
*/
void QScrollView::setMargins(int left, int top, int right, int bottom)
{
    if ( left == d->l_marg &&
         top == d->t_marg &&
         right == d->r_marg &&
         bottom == d->b_marg )
        return;

    d->l_marg = left;
    d->t_marg = top;
    d->r_marg = right;
    d->b_marg = bottom;
    updateScrollBars();
}


/*!
    Returns the left margin.

    \sa setMargins()
*/
int QScrollView::leftMargin() const
{
    return d->l_marg;
}


/*!
    Returns the top margin.

    \sa setMargins()
*/
int QScrollView::topMargin() const
{
    return d->t_marg;
}


/*!
    Returns the right margin.

    \sa setMargins()
*/
int QScrollView::rightMargin() const
{
    return d->r_marg;
}


/*!
    Returns the bottom margin.

    \sa setMargins()
*/
int QScrollView::bottomMargin() const
{
    return d->b_marg;
}

/*!
    \reimp
*/
bool QScrollView::focusNextPrevChild( bool next )
{
    //  Makes sure that the new focus widget is on-screen, if
    //  necessary by scrolling the scroll view.

    // first set things up for the scan
    QFocusData *f = focusData();
    QWidget *startingPoint = f->home();
    QWidget *candidate = 0;
    QWidget *w = next ? f->next() : f->prev();
    QSVChildRec *r;

    // then scan for a possible focus widget candidate
    while( !candidate && w != startingPoint ) {
        if ( w != startingPoint &&
             (w->focusPolicy() & TabFocus) == TabFocus
             && w->isEnabled() &&!w->focusProxy() && w->isVisible() )
            candidate = w;
        w = next ? f->next() : f->prev();
    }

    // if we could not find one, maybe super or parentWidget() can?
    if ( !candidate )
        return QFrame::focusNextPrevChild( next );

    // we've found one.
    r = d->ancestorRec( candidate );
    if ( r && ( r->child == candidate ||
                candidate->isVisibleTo( r->child ) ) ) {
        QPoint cp = r->child->mapToGlobal(QPoint(0,0));
        QPoint cr = candidate->mapToGlobal(QPoint(0,0)) - cp;
        ensureVisible( r->x+cr.x()+candidate->width()/2,
                       r->y+cr.y()+candidate->height()/2,
                       candidate->width()/2,
                       candidate->height()/2 );
    }

    candidate->setFocus();
    return TRUE;
}



/*!
    When a large numbers of child widgets are in a scrollview,
    especially if they are close together, the scrolling performance
    can suffer greatly. If \a y is TRUE the scrollview will use an
    extra widget to group child widgets.

    Note that you may only call enableClipper() prior to adding
    widgets.

    For a full discussion, see this class's \link #enableclipper
    detailed description\endlink.
*/
void QScrollView::enableClipper(bool y)
{
    if ( !d->clipped_viewport == !y )
	return;
    if ( d->children.count() )
	qFatal("May only call QScrollView::enableClipper() before adding widgets");
    if ( y ) {
	d->clipped_viewport = new QClipperWidget(clipper(), "qt_clipped_viewport", d->flags);
	d->clipped_viewport->setGeometry(-coord_limit/2,-coord_limit/2,
					 coord_limit,coord_limit);
	d->clipped_viewport->setBackgroundMode( d->viewport->backgroundMode() );
	d->viewport->setBackgroundMode(NoBackground); // no exposures for this
	d->viewport->removeEventFilter( this );
	d->clipped_viewport->installEventFilter( this );
	d->clipped_viewport->show();
    } else {
	delete d->clipped_viewport;
	d->clipped_viewport = 0;
    }
}

/*!
    Sets the scrollview to have a static background if \a y is TRUE,
    or a scrolling background if \a y is FALSE. By default, the
    background is scrolling.

    Be aware that this mode is quite slow, as a full repaint of the
    visible area has to be triggered on every contents move.

    \sa hasStaticBackground()
*/
void  QScrollView::setStaticBackground(bool y)
{
    d->static_bg = y;
}

/*!
    Returns TRUE if QScrollView uses a static background; otherwise
    returns FALSE.

    \sa setStaticBackground()
*/
bool QScrollView::hasStaticBackground() const
{
    return d->static_bg;
}

/*!
    \overload

    Returns the point \a p translated to a point on the viewport()
    widget.
*/
QPoint QScrollView::contentsToViewport( const QPoint& p ) const
{
    if ( d->clipped_viewport ) {
        return QPoint( p.x() - d->contentsX() - d->clipped_viewport->x(),
                       p.y() - d->contentsY() - d->clipped_viewport->y() );
    } else {
        return QPoint( p.x() - d->contentsX(),
                       p.y() - d->contentsY() );
    }
}

/*!
    \overload

    Returns the point on the viewport \a vp translated to a point in
    the contents.
*/
QPoint QScrollView::viewportToContents( const QPoint& vp ) const
{
    if ( d->clipped_viewport ) {
        return QPoint( vp.x() + d->contentsX() + d->clipped_viewport->x(),
                       vp.y() + d->contentsY() + d->clipped_viewport->y() );
    } else {
        return QPoint( vp.x() + d->contentsX(),
                       vp.y() + d->contentsY() );
    }
}


/*!
    Translates a point (\a x, \a y) in the contents to a point (\a vx,
    \a vy) on the viewport() widget.
*/
void QScrollView::contentsToViewport( int x, int y, int& vx, int& vy ) const
{
    const QPoint v = contentsToViewport(QPoint(x,y));
    vx = v.x();
    vy = v.y();
}

/*!
    Translates a point (\a vx, \a vy) on the viewport() widget to a
    point (\a x, \a y) in the contents.
*/
void QScrollView::viewportToContents( int vx, int vy, int& x, int& y ) const
{
    const QPoint c = viewportToContents(QPoint(vx,vy));
    x = c.x();
    y = c.y();
}

/*!
    \reimp
*/
QSize QScrollView::sizeHint() const
{
    if ( d->use_cached_size_hint && d->cachedSizeHint.isValid() )
	return d->cachedSizeHint;

    constPolish();
    int f = 2 * frameWidth();
    int h = fontMetrics().height();
    QSize sz( f, f );
    if ( d->policy > Manual ) {
	QSVChildRec *r = d->children.first();
	if ( r ) {
	    QSize cs = r->child->sizeHint();
	    if ( cs.isValid() )
		sz += cs.boundedTo( r->child->maximumSize() );
	    else
		sz += r->child->size();
        }
    } else {
	sz += QSize( d->contentsWidth(), contentsHeight() );
    }
    if (d->hMode == AlwaysOn)
	sz.setWidth(sz.width() + d->hbar->sizeHint().width());
    if (d->vMode == AlwaysOn)
	sz.setHeight(sz.height() + d->hbar->sizeHint().height());
    return sz.expandedTo( QSize(12 * h, 8 * h) )
	     .boundedTo( QSize(36 * h, 24 * h) );
}


/*!
    \reimp
*/
QSize QScrollView::minimumSizeHint() const
{
    int h = fontMetrics().height();
    if ( h < 10 )
	h = 10;
    int f = 2 * frameWidth();
    return QSize( (6 * h) + f, (4 * h) + f );
}


/*!
    \reimp

    (Implemented to get rid of a compiler warning.)
*/
void QScrollView::drawContents( QPainter * )
{
}

#ifndef QT_NO_DRAGANDDROP

/*!
  \internal
*/
void QScrollView::startDragAutoScroll()
{
    if ( !d->autoscroll_timer.isActive() ) {
        d->autoscroll_time = initialScrollTime;
        d->autoscroll_accel = initialScrollAccel;
        d->autoscroll_timer.start( d->autoscroll_time );
    }
}


/*!
  \internal
*/
void QScrollView::stopDragAutoScroll()
{
    d->autoscroll_timer.stop();
}


/*!
  \internal
*/
void QScrollView::doDragAutoScroll()
{
    QPoint p = d->viewport->mapFromGlobal( QCursor::pos() );

    if ( d->autoscroll_accel-- <= 0 && d->autoscroll_time ) {
        d->autoscroll_accel = initialScrollAccel;
        d->autoscroll_time--;
        d->autoscroll_timer.start( d->autoscroll_time );
    }
    int l = QMAX( 1, ( initialScrollTime- d->autoscroll_time ) );

    int dx = 0, dy = 0;
    if ( p.y() < autoscroll_margin ) {
        dy = -l;
    } else if ( p.y() > visibleHeight() - autoscroll_margin ) {
        dy = +l;
    }
    if ( p.x() < autoscroll_margin ) {
        dx = -l;
    } else if ( p.x() > visibleWidth() - autoscroll_margin ) {
        dx = +l;
    }
    if ( dx || dy ) {
        scrollBy(dx,dy);
    } else {
        stopDragAutoScroll();
    }
}


/*!
    \property QScrollView::dragAutoScroll
    \brief whether autoscrolling in drag move events is enabled

    If this property is set to TRUE (the default), the QScrollView
    automatically scrolls the contents in drag move events if the user
    moves the cursor close to a border of the view. Of course this
    works only if the viewport accepts drops. Specifying FALSE
    disables this autoscroll feature.
*/

void QScrollView::setDragAutoScroll( bool b )
{
    d->drag_autoscroll = b;
}

bool QScrollView::dragAutoScroll() const
{
    return d->drag_autoscroll;
}

#endif // QT_NO_DRAGANDDROP

/*!\internal
 */
void QScrollView::setCachedSizeHint( const QSize &sh ) const
{
    if ( isVisible() && !d->cachedSizeHint.isValid() )
	d->cachedSizeHint = sh;
}

/*!\internal
 */
void QScrollView::disableSizeHintCaching()
{
    d->use_cached_size_hint = FALSE;
}

/*!\internal
 */
QSize QScrollView::cachedSizeHint() const
{
    return d->use_cached_size_hint ? d->cachedSizeHint : QSize();
}

#endif // QT_NO_SCROLLVIEW
