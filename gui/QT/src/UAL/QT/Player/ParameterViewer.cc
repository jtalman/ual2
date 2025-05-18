#include <iostream>
#include <string>

#include <qapplication.h>
#include <qlabel.h>
#include <qtable.h>
#include <qmessagebox.h>

#include "UAL/UI/Arguments.hh"
#include "UAL/ADXF/ConstantManager.hh"
#include "muParserBase.h"
#include "muParser.h"
//#include "muParserDef.h"

#include "UAL/QT/Player/ParameterViewer.hh"

typedef MUP_BASETYPE value_type;
typedef MUP_STRING_TYPE string_type;
typedef string_type::value_type char_type;
typedef std::map<string_type, value_type> valmap_type;

UAL::QT::ParameterViewer::ParameterViewer(QWidget* parent, const char *name)
  : UAL::QT::BasicEditor(parent, name)
{
  // Parent table
  initTable();
}

void UAL::QT::ParameterViewer::initTable()
{
std::cerr << __FILE__ << ": " << __LINE__ << " enter void UAL::QT::ParameterViewer::initTable()\n";
  label->setText( tr( "ADXF Constants" ) );

  table->setNumCols(2);

  table->horizontalHeader()->setLabel(0, tr("Constant"));
  table->setColumnReadOnly(0, true);
  table->setColumnWidth(0, 175);

  table->horizontalHeader()->setLabel(1, tr("Value"));
  table->setColumnReadOnly(1, true);
  table->setColumnWidth(1, 100);

  table->setColumnStretchable(0, false);
  table->setColumnStretchable(1, false);

std::cerr << __LINE__ << "\n";
  mu::Parser& muParser = UAL::ADXFConstantManager::getInstance()->muParser;

std::cerr << __LINE__ << "\n";
//const mu::Parser::valmap_type constants = muParser.GetConst();
  const             valmap_type constants = muParser.GetConst();

std::cerr << "\nParser constants:\n";
std::cerr <<   "-----------------\n";
std::cerr << "Number: " << (int) constants.size() << "\n";

std::cerr << __LINE__ << "\n";
  table->setNumRows(constants.size());

std::cerr << __LINE__ << "\n";
  int counter = 0;
std::cerr << __LINE__ << "\n";
//mu::Parser::valmap_type::const_iterator item = constants.begin();
              valmap_type::const_iterator item = constants.begin();
std::cerr << __LINE__ << "\n";
  for (; item!=constants.end(); ++item){
    QString qstr(item->first);
    table->setText(counter, 0, tr(qstr));
    table->setText(counter, 1, QString::number(item->second));
    // cout << counter << ", Name: " << item->first << ", Value: " << item->second << "\n";
    counter++;
  }
/*
*/

std::cerr << __FILE__ << ": " << __LINE__ << " leave void UAL::QT::ParameterViewer::initTable()\n";
}

void UAL::QT::ParameterViewer::setValue(int row, int col)
{
}

void UAL::QT::ParameterViewer::activateChanges()
{
}






