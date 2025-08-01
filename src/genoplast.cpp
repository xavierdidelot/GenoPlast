#include <QApplication>
#include "mainwindowimpl.h"
#include <stdio.h> 
#include <unistd.h> 

static const char * help=
  "\
  Usage: genoplast [OPTIONS] treefile datafile outputfile\n\
  \n\
  Options:\n\
  -x NUM      Sets the number of burn-in iterations (default is 10000)\n\
  -y NUM      Sets the number of iterations after burn-in (default is 10000)\n\
  -z NUM      Sets the number of iterations between samples (default is 10)\n\
  -s NUM      Use given seed to initial random number generator\n\
  -A          Output ancestral states\n\
  -v          Verbose mode\n\
  ";
  
bool verbose=false;

int main(int argc, char *argv[])
{
  srand(time(NULL));
      int burn=10000,nbit=10000,thin=10;
      bool power=false;
      bool exportC=false;
      //optind=0;
      int c;
      while ((c = getopt (argc, argv, "x:y:z:s:vA")) != -1)
        switch (c)
          {
            case('x'):burn=atoi(optarg);break;
            case('y'):nbit=atoi(optarg);break;
            case('z'):thin=atoi(optarg);break;
            case('s'):srand(atoi(optarg));break;
//            case('P'):power=true;break;
            case('A'):exportC=true;break;
            case('v'):verbose=true;break;
            case '?':cout<<"Wrong arguments."<<endl<<help<<endl;return 1;
            default:
              abort ();
          }

      //Power test
      if (power==true) {
          Power * power=new Power();
          Param * param=new Param();
          power->setParam(param);
          power->config();
          param->exportXMLbegin(&cout);
          param->exportXMLiter(&cout);
          param->exportXMLend(&cout);
          power->runTest(burn,nbit,thin);
          delete(param);
          return 0;
        }

  if (argc-optind==3) {
      //Command line execution mode
      string treefile=string(argv[optind++]);
      string datafile=string(argv[optind++]);
      string outfile =string(argv[optind++]);
      
      Tree * tree=new Tree(treefile);
      BinData * bindata=new BinData(datafile);
      Param*p=new Param(tree,bindata);
      p->setExportC(exportC);
      if (!verbose) QObject::connect(p,SIGNAL(progress(int)),p,SLOT(echoProgress(int)));
      vector<Move*> * mall=SelectMovesImpl::initMoves(p);
      p->metropolis(*mall,burn,nbit,thin,outfile,true);
      cout<<endl;
      for (unsigned int i=0;i<mall->size();i++) delete(mall->at(i));
      delete(mall);
      delete(p);
      delete(tree);
      delete(bindata);
      return 0;
    }
    
  //GUI execution mode
  QApplication app(argc, argv);
  MainWindowImpl mainwindowimpl;
  mainwindowimpl.show();
  return app.exec();
}
