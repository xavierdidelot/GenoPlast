#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H
//
#include "ui_mainwindow.h"
#include "progressimpl.h"
#include "runparamimpl.h"
#include "selectmovesimpl.h"
#include "plotimpl.h"
#include "paramcons.h"
#include "gelmanrubinimpl.h"
#include "ancestralimpl.h"
#include "editpriorimpl.h"
#include "editinitimpl.h"
#include "power.h"
#include <QtSvg>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QPrinter>
//
class MainWindowImpl : public QMainWindow, public Ui::MainWindow {
      Q_OBJECT
    public:
      MainWindowImpl( QWidget * parent = 0, Qt::WindowFlags f = Qt::WindowFlags() );
      virtual ~MainWindowImpl();
    public slots:
      void run(int burnin,int additional,int thinin,QString outfile,bool randOrder);
      void setMall(vector<Move*> * mall);
    private slots:
      void on_actionHelp_contents_triggered();
      void on_actionEdit_priors_triggered();
      void on_actionGelman_Rubin_test_triggered();
      void on_actionEdit_list_of_moves_triggered();
      void on_actionSave_current_triggered();
      void on_actionSave_tree_triggered();
      void on_actionSave_binary_data_triggered();
      void on_actionSimulate_tree_triggered();
      void on_actionSimulate_data_triggered();
      void on_actionConsensus_triggered();
      void on_actionSave_output_file_triggered();
      void on_actionExit_triggered();
      void on_actionSave_picture_triggered();
      void on_actionOpen_output_file_triggered();
      void on_actionLoad_tree_triggered();
      void on_actionLoad_binary_data_triggered();
      void on_actionRun_algorithm_triggered();
      void on_actionAbout_triggered();
      void on_actionLoad_names_triggered();
      void on_actionSet_initial_values_triggered();
      void on_actionPower_study_triggered();
      void on_menuPlot_triggered(QAction* action);
      void on_menuVisualisation_triggered(QAction* action);
      void on_actionReinitiate_random_generator_triggered();
      void on_actionAncestral_states_triggered();
      void on_actionCombine_output_files_triggered();

  protected:
      Param * param;
      vector<Move*> * mall;
      QDomDocument * domDoc;
      int currentIteration;
      int maxIteration;
      void paintEvent(QPaintEvent*);
      void openXMLFile(QString qstr);
      void loadIteration();
  };
#endif



