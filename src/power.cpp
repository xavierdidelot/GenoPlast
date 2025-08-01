#include "power.h"
//
Power::Power() {
  stop=false;
}

void Power::config() {
  param->setTree(new Tree(15));
  param->setValue(1,0,1.0,1.0);
  vector<double> * locvec;
  vector<double> * valvec;
  int i=0;
  param->getRp()->getCompoBranch(i,&locvec,&valvec);
  while (locvec->size()==1) {
      i++;
      param->getRp()->getCompoBranch(i,&locvec,&valvec);
    }
  toChange=&(valvec->at(0));
  *toChange=param->getRp()->getTopval()*2.0;
  param->getRp()->propagate();
  emit(updateMain());
}

void Power::runTest(int burnin,int additional,int thinin) {
  srand(time(NULL));//Reinitiate rng no matter what
  for (int i=0;i<11;i++) {
      for (int j=0;j<9;j++) {
          int f=i*50;
          double newVal=pow(2.0,(j-4.0)/2.0)*param->getRp()->getTopval();//param->getRp()->getTopval()*(j/5.0);
          double score=0.0;
          *toChange=newVal;
          param->getRp()->propagate();
          emit(updateMain());
          for (int k=0;k<5;k++) score+=simuAndMetro(burnin,additional,thinin,f)/5.0;
          cout<<score<<" ";
          cout.flush();
          if (stop) return;
        }
      cout<<endl;
      cout.flush();
    }
}

double Power::simuAndMetro(int burnin,int additional,int thinin,int f) {
//Simulate data
  param->simulateData(f);
//Metropolis algorithm
  infer=new Param();
  QObject::connect(infer,SIGNAL(progress(int)),this,SLOT(slotProgress(int)));
  vector<Move*>*moves=SelectMovesImpl::initMoves(infer);
  infer->setTree(param->getTree());
  infer->setBinData(param->getBinData());
  infer->metropolis(*moves,burnin,additional,thinin,string("/tmp/power.out"),true);
  delete(infer);
  //Calculate score
  int score;
  QFile file(QString("/tmp/power.out"));
  QDomDocument * domDoc=new QDomDocument();
  file.open(QIODevice::ReadOnly);
  domDoc->setContent(&file);
  file.close();
  QDomElement root = domDoc->documentElement();
  QDomNodeList allits=root.elementsByTagName("Iteration");
  int numIteration=allits.size();
  score=numIteration;
  for (int i=0;i<numIteration;i++) if (allits.at(i).toElement().elementsByTagName("rp").at(0).toElement().elementsByTagName("ChangePoint").size()==0) score--;
  delete(domDoc);
  return 1.0*score/numIteration;
}

void Power::slotProgress(int i) {
  emit(signalProgress(i));
}

void Power::cancelled() {
  stop=true;
  infer->cancelled();
}
