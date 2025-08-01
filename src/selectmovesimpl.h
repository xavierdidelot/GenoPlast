#ifndef SELECTMOVESIMPL_H
#define SELECTMOVESIMPL_H
//
#include "ui_SelectMoves.h"
#include <iostream>
#include "moveaddcp.h"
#include "moveremovecp.h"
#include "movevaluecp.h"
#include "moveposcp.h"
//#include "movevalueboth.h"

//

using namespace std;

class SelectMovesImpl : public QDialog, public Ui::SelectMoves
{
Q_OBJECT
public:
	SelectMovesImpl( QWidget * parent = 0, Qt::WindowFlags f = Qt::WindowFlags() );
	static vector<Move*> * initMoves(Param*p);
	void setMall(vector<Move*> * mall);
protected:
	vector<Move*> * mall;
private slots:
	void on_cancelButton_clicked();
	void on_okButton_clicked();
	void on_resetButton_clicked();
	void on_x2Button_clicked();
	void on_upButton_clicked();
	void on_downButton_clicked();
	void deleteCurrentItem();
signals:
	void done(vector<Move*> * m);
};
#endif
