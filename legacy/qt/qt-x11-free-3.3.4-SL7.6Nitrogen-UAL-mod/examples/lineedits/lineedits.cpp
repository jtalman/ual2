/****************************************************************************
** $Id: qt/lineedits.cpp   3.3.4   edited Jun 19 2003 $
**
** Copyright (C) 1992-2003 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "lineedits.h"

#include <qlineedit.h>
#include <qcombobox.h>
#include <qframe.h>
#include <qvalidator.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qhbox.h>

/*
 * Constructor
 *
 * Creates child widgets of the LineEdits widget
 */

LineEdits::LineEdits( QWidget *parent, const char *name )
    : QGroupBox( 0, Horizontal, "Line edits", parent, name )
{
    setMargin( 10 );

    QVBoxLayout* box = new QVBoxLayout( layout() );

    QHBoxLayout *row1 = new QHBoxLayout( box );
    row1->setMargin( 5 );

    // Create a Label
    QLabel* label = new QLabel( "Echo Mode: ", this);
    row1->addWidget( label );

    // Create a Combobox with three items...
    combo1 = new QComboBox( FALSE, this );
    row1->addWidget( combo1 );
    combo1->insertItem( "Normal" );
    combo1->insertItem( "Password" );
    combo1->insertItem( "No Echo" );
    // ...and connect the activated() SIGNAL with the slotEchoChanged() SLOT to be able
    // to react when an item is selected
    connect( combo1, SIGNAL( activated( int ) ), this, SLOT( slotEchoChanged( int ) ) );

    // insert the first LineEdit
    lined1 = new QLineEdit( this );
    box->addWidget( lined1 );

    // another widget which is used for layouting
    QHBoxLayout *row2 = new QHBoxLayout( box );
    row2->setMargin( 5 );

    // and the second label
    label = new QLabel( "Validator: ", this );
    row2->addWidget( label );

    // A second Combobox with again three items...
    combo2 = new QComboBox( FALSE, this );
    row2->addWidget( combo2 );
    combo2->insertItem( "No Validator" );
    combo2->insertItem( "Integer Validator" );
    combo2->insertItem( "Double Validator" );
    // ...and again the activated() SIGNAL gets connected with a SLOT
    connect( combo2, SIGNAL( activated( int ) ), this, SLOT( slotValidatorChanged( int ) ) );

    // and the second LineEdit
    lined2 = new QLineEdit( this );
    box->addWidget( lined2 );

    // yet another widget which is used for layouting
    QHBoxLayout *row3 = new QHBoxLayout( box );
    row3->setMargin( 5 );

    // we need a label for this too
    label = new QLabel( "Alignment: ", this );
    row3->addWidget( label );

    // A combo box for setting alignment
    combo3 = new QComboBox( FALSE, this );
    row3->addWidget( combo3 );
    combo3->insertItem( "Left" );
    combo3->insertItem( "Centered" );
    combo3->insertItem( "Right" );
    // ...and again the activated() SIGNAL gets connected with a SLOT
    connect( combo3, SIGNAL( activated( int ) ), this, SLOT( slotAlignmentChanged( int ) ) );

    // and the third lineedit
    lined3 = new QLineEdit( this );
    box->addWidget( lined3 );

    // exactly the same for the fourth
    QHBoxLayout *row4 = new QHBoxLayout( box );
    row4->setMargin( 5 );

    // we need a label for this too
    label = new QLabel( "Input mask: ", this );
    row4->addWidget( label );

    // A combo box for choosing an input mask
    combo4 = new QComboBox( FALSE, this );
    row4->addWidget( combo4 );
    combo4->insertItem( "No mask" );
    combo4->insertItem( "Phone number" );
    combo4->insertItem( "ISO date" );
    combo4->insertItem( "License key" );

    // ...this time we use the activated( const QString & ) signal
    connect( combo4, SIGNAL( activated( int ) ),
	     this, SLOT( slotInputMaskChanged( int ) ) );

    // and the fourth lineedit
    lined4 = new QLineEdit( this );
    box->addWidget( lined4 );

    // last widget used for layouting
    QHBox *row5 = new QHBox( this );
    box->addWidget( row5 );
    row5->setMargin( 5 );

    // last label
    (void)new QLabel( "Read-Only: ", row5 );

    // A combo box for setting alignment
    combo5 = new QComboBox( FALSE, row5 );
    combo5->insertItem( "False" );
    combo5->insertItem( "True" );
    // ...and again the activated() SIGNAL gets connected with a SLOT
    connect( combo5, SIGNAL( activated( int ) ), this, SLOT( slotReadOnlyChanged( int ) ) );

    // and the last lineedit
    lined5 = new QLineEdit( this );
    box->addWidget( lined5 );

    // give the first LineEdit the focus at the beginning
    lined1->setFocus();
}

/*
 * SLOT slotEchoChanged( int i )
 *
 * i contains the number of the item which the user has been chosen in the
 * first Combobox. According to this value, we set the Echo-Mode for the
 * first LineEdit.
 */

void LineEdits::slotEchoChanged( int i )
{
    switch ( i ) {
    case 0:
	lined1->setEchoMode( QLineEdit::Normal );
        break;
    case 1:
	lined1->setEchoMode( QLineEdit::Password );
        break;
    case 2:
	lined1->setEchoMode( QLineEdit::NoEcho );
        break;
    }

    lined1->setFocus();
}

/*
 * SLOT slotValidatorChanged( int i )
 *
 * i contains the number of the item which the user has been chosen in the
 * second Combobox. According to this value, we set a validator for the
 * second LineEdit. A validator checks in a LineEdit each character which
 * the user enters and accepts it if it is valid, else the character gets
 * ignored and not inserted into the lineedit.
 */

void LineEdits::slotValidatorChanged( int i )
{
    switch ( i ) {
    case 0:
	lined2->setValidator( 0 );
        break;
    case 1:
	lined2->setValidator( new QIntValidator( lined2 ) );
        break;
    case 2:
	lined2->setValidator( new QDoubleValidator( -999.0, 999.0, 2,
						    lined2 ) );
        break;
    }

    lined2->setText( "" );
    lined2->setFocus();
}


/*
 * SLOT slotAlignmentChanged( int i )
 *
 * i contains the number of the item which the user has been chosen in
 * the third Combobox.  According to this value, we set an alignment
 * third LineEdit.
 */

void LineEdits::slotAlignmentChanged( int i )
{
    switch ( i ) {
    case 0:
	lined3->setAlignment( QLineEdit::AlignLeft );
        break;
    case 1:
	lined3->setAlignment( QLineEdit::AlignCenter );
        break;
    case 2:
	lined3->setAlignment( QLineEdit::AlignRight );
        break;
    }

    lined3->setFocus();
}

/*
 * SLOT slotInputMaskChanged( const QString &mask )
 *
 * i contains the number of the item which the user has been chosen in
 * the third Combobox.  According to this value, we set an input mask on
 * third LineEdit.
 */

void LineEdits::slotInputMaskChanged( int i )
{
    switch( i ) {
    case 0:
	lined4->setInputMask( QString::null );
	break;
    case 1:
	lined4->setInputMask( "+99 99 99 99 99;_" );
	break;
    case 2:
	lined4->setInputMask( "0000-00-00" );
	lined4->setText( "00000000" );
	lined4->setCursorPosition( 0 );
	break;
    case 3:
	lined4->setInputMask( ">AAAAA-AAAAA-AAAAA-AAAAA-AAAAA;#" );
	break;
    }
    lined4->setFocus();
}

/*
 * SLOT slotReadOnlyChanged( int i )
 *
 * i contains the number of the item which the user has been chosen in
 * the fourth Combobox.  According to this value, we toggle read-only.
 */

void LineEdits::slotReadOnlyChanged( int i )
{
    switch ( i ) {
    case 0:
	lined5->setReadOnly( FALSE );
        break;
    case 1:
	lined5->setReadOnly( TRUE );
        break;
    }

    lined5->setFocus();
}

