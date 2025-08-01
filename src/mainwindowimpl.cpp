#include "mainwindowimpl.h"
//
MainWindowImpl::MainWindowImpl( QWidget * parent, Qt::WFlags f):QMainWindow(parent, f) {
  param=new Param();
  domDoc=NULL;
  mall=SelectMovesImpl::initMoves(NULL);
  currentIteration=1;
  maxIteration=1;
  setupUi(this);
}

MainWindowImpl::~MainWindowImpl() {
  delete(param);
  delete(domDoc);
  if (mall!=NULL) {for (unsigned int i=0;i<mall->size();i++) delete(mall->at(i));delete(mall);}
}

void MainWindowImpl::on_actionLoad_tree_activated() {
  QString qstr = QFileDialog::getOpenFileName(this, tr("Open File"),".","Newick file (*.nwk *.newick *.tree);;All files (*)");
  if (qstr==NULL) return;
  delete(domDoc);
  domDoc=NULL;
  string newickfile=qstr.toStdString();
  param->setTree(new Tree(newickfile));
  repaint();
}

void MainWindowImpl::on_actionLoad_binary_data_activated() {
  QString qstr = QFileDialog::getOpenFileName(this, tr("Open File"),".","");
  if (qstr==NULL) return;
  param->setBinData(new BinData(qstr.toStdString ()));
}

void MainWindowImpl::on_actionRun_algorithm_activated() {
  if (param->getTree()==NULL) {QMessageBox::about(0, "Information","Need a tree first.");return;}
  RunParamImpl *r=new RunParamImpl();
  r->show();
  QObject::connect(r,SIGNAL(setExportC(bool)),param,SLOT(setExportC(bool)));
  QObject::connect(r,SIGNAL(getRunning(int,int,int,QString,bool)),this,SLOT(run(int,int,int,QString,bool)));
}

void MainWindowImpl::run(int burnin,int additional,int thinin,QString outfile,bool randOrder) {
  ProgressImpl progress;
  progress.show();
  QObject::connect(param,SIGNAL(progress(int)),&progress,SLOT(updateProgress(int)));
  QObject::connect(param,SIGNAL(progress(int)),this,SLOT(repaint()));
  QObject::connect(&progress,SIGNAL(cancel()),param,SLOT(cancelled()));
  for (unsigned int i=0;i<mall->size();i++) mall->at(i)->setParam(param);
  param->metropolis(*mall,burnin,additional,thinin,outfile.toStdString(),randOrder);
  openXMLFile(outfile);
}

void MainWindowImpl::on_actionAbout_activated() {
  QMessageBox::about(0, "About...","This is GenoPlast version 1.0.");
}

void MainWindowImpl::paintEvent(QPaintEvent*) {
  param->display(this);
}

void MainWindowImpl::on_actionOpen_output_file_activated() {
  QString qstr = QFileDialog::getOpenFileName(this, tr("Open output file"),".","XML files (*.xml);;All files (*)");
  if (qstr==NULL) return;
  openXMLFile(qstr);
}

void MainWindowImpl::openXMLFile(QString qstr) {
  QFile file(qstr);
  delete(domDoc);
  domDoc=new QDomDocument();
  file.open(QIODevice::ReadOnly);
  domDoc->setContent(&file);
  file.close();
  QDomElement root = domDoc->documentElement();
  QDomNode treeDN=root.elementsByTagName("Tree").at(0);
  QDomText n=treeDN.firstChild().toText();
  string s=n.data().toStdString();
  while (s.at(0)==10 || s.at(0)==13) s=s.substr(1,s.length()-1);
  while (s.at(s.size()-1)==10 || s.at(s.size()-1)==13) s=s.substr(0,s.length()-1);
  param->setTree(new Tree(s,false));
  currentIteration=1;
  QDomNodeList allits=root.elementsByTagName("Iteration");
  maxIteration=allits.size();
  loadIteration();
  repaint();
}

void MainWindowImpl::loadIteration() {
  if (currentIteration>maxIteration) currentIteration=maxIteration;
  if (currentIteration<1) currentIteration=1;
  if (domDoc==NULL) return;
  QDomElement it=domDoc->elementsByTagName("Iteration").at(currentIteration-1).toElement();
  Param * param2=new Param(param->getTree(),param->getBinData(),&it);
  delete(param);
  param=param2;
  statusBar()->showMessage(tr("Iteration ")+QString::number(currentIteration));
}

void MainWindowImpl::on_actionSave_output_file_activated() {
  if (domDoc==NULL) {QMessageBox::about(0, "Information","Need some data first.");return;}
  QString qstr = QFileDialog::getSaveFileName(this, tr("Save output file"),".","XML files (*.xml);;All files (*)");
  if (qstr==NULL) return;
  QFile file(qstr);
  if ( !file.open(QIODevice::WriteOnly)) return;
  QTextStream ts( &file );
  ts << domDoc->toString();
  file.close();
}

void MainWindowImpl::on_actionExit_activated() {
  close();
}

void MainWindowImpl::on_actionSave_picture_activated() {
  QString qstr = QFileDialog::getSaveFileName(this, tr("Save picture file"),".",tr("Joint Photographic Experts Group (*.jpg *.jpeg);;Windows Bitmap (*.bmp);;Portable Network Graphics (*.png);;Portable Pixmap (*.ppm);;X11 Bitmap (*.xbm *.xpm);;SVG Format (*.svg);;PostScript Format (*.ps);;Abode PDF Format (*.pdf)"));
  if (qstr==NULL) return;
  if (qstr.endsWith("svg")) {
  	QSvgGenerator qsvg;
	qsvg.setFileName(qstr);
	qsvg.setSize(QSize(width(),height()));
      param->display(&qsvg);
      return;
    }
    if (qstr.endsWith("ps") || qstr.endsWith("pdf")) {
  	QPrinter qprint;
	qprint.setOutputFileName(qstr);
	qprint.setOrientation( QPrinter::Landscape);
      param->display(&qprint);
      return;
    }
  QImage image(width(),height(),QImage::Format_RGB32);
  image.invertPixels();//Fill image in white
  param->display(&image);
  image.save(qstr);
}

void MainWindowImpl::on_actionConsensus_activated() {
  if (domDoc==NULL) {QMessageBox::about(0,"Information","Need some data first.");return;}
  ParamCons * paramcons=new ParamCons(param->getTree(),param->getBinData());
  for (currentIteration=1;currentIteration<=maxIteration;currentIteration++) {loadIteration();paramcons->add(param);}
  paramcons->compute();
  delete(param);
  param=paramcons;
  repaint();
}

void MainWindowImpl::on_actionSimulate_tree_activated() {
  bool ok;
  int n=QInputDialog::getInteger(this,"Enter n","Enter number of isolates:",10,3,1000,1,&ok);
  if (!ok) return;
  param->setTree(new Tree(n));
  repaint();
}

void MainWindowImpl::on_actionSimulate_data_activated() {
  if (param->getTree()==NULL) {QMessageBox::about(0,"Information","Need a tree first.");return;}
  EditInitImpl * edit=new EditInitImpl(this);
  edit->setParam(param);
  edit->setSimu();
  edit->show();
}

void MainWindowImpl::on_actionSave_current_activated() {
  if (param->getRp()==NULL) {QMessageBox::about(0,"Information","Nothing to save.");return;}
  QString qstr = QFileDialog::getSaveFileName(this, tr("Save current state"),".","XML files (*.xml);;All files (*)");
  if (qstr==NULL) return;
  ofstream output;
  output.open(qstr.toStdString().data());
  param->exportXMLbegin(&output);
  param->exportXMLiter(&output);
  param->exportXMLend(&output);
  output.close();
}

void MainWindowImpl::on_actionSave_tree_activated() {
  if (param->getTree()==NULL) {QMessageBox::about(0,"Information","No tree to save.");return;}
  QString qstr = QFileDialog::getSaveFileName(this, tr("Save tree file"),".","Newick file (*.nwk *.newick *.tree);;All files (*)");
  if (qstr==NULL) return;
  QFile file(qstr);
  if ( !file.open(QIODevice::WriteOnly)) return;
  QTextStream ts( &file );
  ts << QString::fromStdString(param->getTree()->newick());
  file.close();
}

void MainWindowImpl::on_actionSave_binary_data_activated() {
  if (param->getBinData()==NULL) {QMessageBox::about(0,"Information","No data to save.");return;}
  QString qstr = QFileDialog::getSaveFileName(this, tr("Save data file"),".","All files (*)");
  if (qstr==NULL) return;
  QFile file(qstr);
  if ( !file.open(QIODevice::WriteOnly)) return;
  QTextStream ts( &file );
  ts << QString::fromStdString(param->getBinData()->output());
  file.close();
}

void MainWindowImpl::on_actionEdit_list_of_moves_activated() {
  SelectMovesImpl *r=new SelectMovesImpl(this,0);
  r->setMall(mall);
  QObject::connect(r,SIGNAL(done(vector<Move*>*)),this,SLOT(setMall(vector<Move*>*)));
  r->show();
}

void MainWindowImpl::setMall(vector<Move*> * m) {
  if (mall!=NULL) {for (unsigned int i=0;i<mall->size();i++) delete(mall->at(i));delete(mall);}
  mall=m;
}

void MainWindowImpl::on_actionGelman_Rubin_test_activated() {
  if (domDoc==NULL) {QMessageBox::about(0, "Information","Need some data first.");return;}
  QStringList files  = QFileDialog::getOpenFileNames(this, tr("Select File(s)"),".","XML files (*.xml);;All files (*)");
  if (files.size()==0) return;
  GelmanRubinImpl * gr=new GelmanRubinImpl(this);
  gr->compute(param->getTree(),domDoc,&files);
  gr->show();
}

void MainWindowImpl::on_actionEdit_priors_activated() {
  EditPriorImpl * edit=new EditPriorImpl(this);
  edit->setPrior(param->getPrior());
  edit->show();
}

void MainWindowImpl::on_actionHelp_contents_activated() {
QMessageBox::about(0,"Help","The documentation is available online at the following URL: http://www2.warwick.ac.uk/fac/sci/statistics/staff/research/didelot/genoplast/doc");
}

void MainWindowImpl::on_actionLoad_names_activated() {
  QString qstr = QFileDialog::getOpenFileName(this, tr("Open File"),".","All files (*)");
  if (qstr==NULL) return;
  param->getTree()->loadNames(qstr);
  repaint();
}

void MainWindowImpl::on_actionSet_initial_values_activated() {
  if (param->getTree()==NULL) {QMessageBox::about(0, "Information","Need a tree first.");return;}
  EditInitImpl * edit=new EditInitImpl(this);
  edit->setParam(param);
  edit->show();
}

void MainWindowImpl::on_actionPower_study_activated() {
  Power * power=new Power();
  ProgressImpl * progress=new ProgressImpl();
  QObject::connect(power,SIGNAL(signalProgress(int)),progress,SLOT(updateProgress(int)));
  QObject::connect(progress,SIGNAL(cancel()),power,SLOT(cancelled()));
  QObject::connect(power,SIGNAL(updateMain()),this,SLOT(update()));
  progress->show();
  power->setParam(param);
  power->config();
  power->runTest(1000,1000,1);
}

void MainWindowImpl::on_menuPlot_triggered(QAction* a) {
  if (domDoc==NULL) {QMessageBox::about(0, "Information","Need some data first.");return;}
  PlotImpl * pi=new PlotImpl(this);
  if (a==action11 || a==action12) pi->extractValuesNumCP(domDoc,true);
  if (a==action21 || a==action22) pi->extractValuesNumCP(domDoc,false);
  if (a==action31 || a==action32) pi->extractValuesTop(domDoc,true);
  if (a==action41 || a==action42) pi->extractValuesTop(domDoc,false);
  if (a==action51 || a==action52) pi->extractValuesLlhood(domDoc);
  if (a==action11 || a==action21 || a==action31 || a==action41 || a==action51) pi->setMode(0); else pi->setMode(1);
  pi->show();
}

void MainWindowImpl::on_menuVisualisation_triggered(QAction* a) {
  if (a==actionSave_picture || a==actionConsensus || a==actionShow_node_ids || a==actionAncestral_states) return;
  if (a==actionNext_iteration) currentIteration++;
  if (a==actionPrevious_iteration) currentIteration--;
  if (a==actionFirst_iteration) currentIteration=1;
  if (a==actionLast_iteration) currentIteration=maxIteration;
  if (a==actionScale_up) param->incrRateScale();
  if (a==actionScale_down) param->decrRateScale();
  if (a==actionScale_up || a==actionScale_down)
    statusBar()->showMessage(tr("Scale=")+QString::number(param->getRateScale()));
  else loadIteration();
  repaint();
}

void MainWindowImpl::on_actionReinitiate_random_generator_activated() {
  bool ok;
  int n=QInputDialog::getInteger(this,"Enter seed","Enter seed for random number generator:",0,0,100000,1,&ok);
  if (!ok) return;
  srand(n);
}

void MainWindowImpl::on_actionAncestral_states_activated()
{
  if (domDoc==NULL) {QMessageBox::about(0, "Information","Need some data first.");return;}
  if (param->getBinData()==NULL) {QMessageBox::about(0, "Information","Need some data first.");return;}
  QDomElement root = domDoc->documentElement();
  if (root.elementsByTagName("Iteration").at(0).toElement().elementsByTagName("ancstate").length()==0) 
  {QMessageBox::about(0,"Information","No ancestral state data found in current file.");return;}
  AncestralImpl * a=new AncestralImpl(this);
  a->compute(&root,param);
  a->show();
}

void MainWindowImpl::on_actionCombine_output_files_activated()
{
  //Select files to combine
  QStringList files  = QFileDialog::getOpenFileNames(this, tr("Select File(s)"),".","XML files (*.xml);;All files (*)");
  if (files.size()==0) return;
  //Open them and combine them
  openXMLFile(files.at(0));
  QDomDocument * dd=domDoc;
  domDoc=NULL;
  for (int i=1;i<files.size();i++) {
    openXMLFile(files.at(i));
    QDomNodeList list=domDoc->documentElement().elementsByTagName("Iteration");
    for (int j=0;j<list.size();j++) dd->documentElement().appendChild(list.at(j).cloneNode());
  }
  //Save result
  QString qstr = QFileDialog::getSaveFileName(this, tr("Save output file"),".","XML files (*.xml);;All files (*)");
  if (qstr==NULL) return;
  QFile file(qstr);
  if ( !file.open(QIODevice::WriteOnly)) return;
  QTextStream ts( &file );
  ts << dd->toString();
  file.close();
  delete(dd);
  //Open result
  openXMLFile(qstr);
}
