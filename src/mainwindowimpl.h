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
//
class MainWindowImpl : public QMainWindow, public Ui::MainWindow {
      Q_OBJECT
    public:
      MainWindowImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
      virtual ~MainWindowImpl();
    public slots:
      void run(int burnin,int additional,int thinin,QString outfile,bool randOrder);
      void setMall(vector<Move*> * mall);
    private slots:
      void on_actionHelp_contents_activated();
      void on_actionEdit_priors_activated();
      void on_actionGelman_Rubin_test_activated();
      void on_actionEdit_list_of_moves_activated();
      void on_actionSave_current_activated();
      void on_actionSave_tree_activated();
      void on_actionSave_binary_data_activated();
      void on_actionSimulate_tree_activated();
      void on_actionSimulate_data_activated();
      void on_actionConsensus_activated();
      void on_actionSave_output_file_activated();
      void on_actionExit_activated();
      void on_actionSave_picture_activated();
      void on_actionOpen_output_file_activated();
      void on_actionLoad_tree_activated();
      void on_actionLoad_binary_data_activated();
      void on_actionRun_algorithm_activated();
      void on_actionAbout_activated();
      void on_actionLoad_names_activated();
      void on_actionSet_initial_values_activated();
      void on_actionPower_study_activated();
      void on_menuPlot_triggered(QAction* action);
      void on_menuVisualisation_triggered(QAction* action);
      void on_actionReinitiate_random_generator_activated();
      void on_actionAncestral_states_activated();
      void on_actionCombine_output_files_activated();

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



