#include <iostream>
#include <Instruction.h>
#include <Program.h>
#include <Directive.h>
#include <Label.h>
#include <fstream>
#include <Basic_block.h>
#include <Function.h>


int main(int argc, char * argv[])
{
  if (argc < 2) {
    cout << "erreur : pas de fichier assembleur" << endl;
  }	  
  Program prog(argv[1]);
  Function* functmp;
  list <Function*> myfunc; 
  list <Basic_block*> myBB;

  cout<<"Le programme a "<<prog.size()<<" lignes\n"<<endl;

  prog.comput_function();
  cout<<"nombre de fonctions : "<<prog.nbr_func()<<endl;

  list<Function*>::iterator itfct;
  list<Basic_block*>::iterator itbb;
  Basic_block *bb;
  int i, j;
  list<int> frees;
  Dfg *d;
  Cfg *c;
	
  for(itfct=prog.function_list_begin(), i=0;
      itfct!=prog.function_list_end(); itfct++, i++){
    functmp=*itfct;
    cout<<"------------Function DISPLAY----------\n" <<endl;
    functmp->display();
    functmp->comput_basic_block();
    functmp->comput_label();
    functmp->comput_succ_pred_BB();
	   	   
    cout<<"========== Function "<<i<<"================================="<<endl;
    cout<<"============================================================"<<endl;

    // Normal blocks
    j=0;
    for(itbb=functmp->bb_list_begin(); 
	itbb!=functmp->bb_list_end(); itbb++, j++){
      bb=*itbb;
      cout<<"----------BB "<<j<<"-----------"<<endl;
      bb->display();
      bb->link_instructions();
      bb->comput_pred_succ_dep();
      bb->compute_use_def();

      cout<<"---nb_cycles : "<<bb->nb_cycles()<<"-----------"<<endl;
      d = new Dfg(bb);
      cout<<"comput critical path"<<endl;
      d->comput_critical_path();
      cout<<"critical path "<<d->get_critical_path()<<endl;
    }
    
    // Renaming registers
    functmp->compute_live_var();
    cout<<"----- APRES RENOMMAGE ------"<<endl;
    j=0;
    for(itbb=functmp->bb_list_begin(); 
	itbb!=functmp->bb_list_end(); itbb++, j++){
      bb=*itbb;
      cout<<"----------BB "<<j<<"-----------"<<endl;
      bb->compute_def_liveout();
      bb->reg_rename();
      bb->display();
      bb->reset_pred_succ_dep();
      bb->comput_pred_succ_dep();
      cout<<"---nb_cycles--"<<bb->nb_cycles()<<"-----------"<<endl;
      bb->compute_use_def();
      d= new Dfg(bb);
      cout<<"comput critical path"<<endl;
      d->comput_critical_path();
      cout<<"critical path "<<d->get_critical_path()<<endl;

      d->scheduling(false);
      d->apply_scheduling();
      cout<<"---- BB with new scheduling: -----"<<endl;
      bb->display();
	     
      cout<<"----nb_cycles--"<<bb->nb_cycles()<<"-----------"<<endl;
      
    }
  }

  return 0;
}
