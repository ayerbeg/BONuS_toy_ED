// EVENT DISPLAY FOR BONuS RTPC
// ORIGINAL BY HUAN YAO 
// MODIFIED BY SCOTT BARCUS, EVAN MCCLELLAN AND CARLOS AYERBE 2016-17
// FOR THE GRINCH PROTOTYPE DETECTOR


#include "TTree.h"
#include "TGLHistPainter.h"
#include "TGLBoxPainter.h"

#define file "../files/nt_P-10T-10_200ns_0.6mm.root"

int Event;

const Int_t maxHitsPerEvent = 250;
int* Hit_Events=NULL;
TCanvas* c;

int N_evts;

int N_ROW = 200;
int N_COL = 100;

// Number of PADs on RTPC
const int N_PMT= N_ROW*N_COL;

Int_t i,j;

Int_t next_evt=0;
Int_t usr_evt;

TBox**PAD = new TBox*[N_PMT];

Int_t    eventEntries;
TTree   *eventtree;
TBranch *branch;
Int_t    nHits;
Int_t    channelID;
int      pad;

Int_t event_cnt; 


void event_display()
{
  //*************************************
  //DOUBLE LOOP FOR EVENT DISPLAY CREATION
 
  double x,y;//Lower corner pad

  Int_t k=0;

  double pad_z = 4;
  double pad_phi = 2.8;

  //  cout<<"Creating Event Display"<<endl;
  for (i=0; i<N_ROW; i++ ) 
    {
      for (j=0; j<N_COL; j++ ) 
	{
	  double shift = i%4;
	  x = j*pad_z+shift;
	  y = i*pad_phi;
	  
	  PAD[k] = new TBox (x, y,x+pad_z,y+pad_phi);
	  PAD[k]->SetFillStyle(0);
	  PAD[k]->SetLineColor(9);
	  //	  cout<<"k: "<<k<<" X: "<<x<<" Y: "<<y<<endl;
	  if ( i==0 && j==0 )
	    PAD[k]->Draw();
	  else
	    PAD[k]->Draw("same");
	  k++;
	}
    }  //End building event display geometry.

  TText *x_tit = new TText(200,-20,"Z-direction");
  x_tit->SetTextAlign(22);
  x_tit->SetTextColor(kRed+2);
  x_tit->SetTextFont(43);
  x_tit->SetTextSize(20);
  x_tit->Draw("same");
  
  TText *y_tit = new TText(-15,280,"Phi-direction");
  y_tit->SetTextAlign(22);
  y_tit->SetTextColor(kRed+2);
  y_tit->SetTextFont(43);
  y_tit->SetTextSize(20);
  y_tit->SetTextAngle(90);
  y_tit->Draw("same");
  //  cout<<"End Event Display Creation"<<endl;
  //*************************************
}


void show(int option=0)
{
  /*
  // ************************************************************************************************************
  // command line input  (USED IN GRINCH)
  // ************************************************************************************************************
  //  string pre = "../decodefiles/minimal_"; //this is the path line to the datafiles and the prefix of the file
  
  string pre = "./minimal_";
  string post = ".root";
  
  pre.append(filenumber);
  string file_str = pre.append(post);
  const char *file = file_str.c_str();
  
  cout<< "File to analyze: " << file_str << endl;
  
  TFile *infile= new TFile(file);
  // ************************************************************************************************************
  // end command line input
  // ************************************************************************************************************
  */


  
  // hard code data file 

  TFile *infile=new TFile(file); //Definition on top of the code

  eventtree=(TTree*)infile->Get("ep"); //change tree according your root file

  cout<<" Option: "<<option<<endl;   
  eventEntries = eventtree->GetEntries();
 
 
  //Get entries from eventtree
  
  cout<<"eventEntries: "<<eventEntries<<endl;
  
  N_evts = eventEntries-1;//Max index of events is number of entries-1  
  
  cout<<"PREVIOUS EVENT: #"<<event_cnt<<endl;


  //SELECTION

  if ( option==-2 )   //First Event
    { 
      fill_event(0);
      event_cnt = 0;
    }
  else if ( option==-3 ) //Last Event 
    { 
      fill_event(N_evts);
      event_cnt = N_evts;
    }
  else if ( option==0 ) //Ask Event
    { 
      Printf("Please Enter Event Number:");
      cin>>usr_evt;
      fill_event(usr_evt);
      event_cnt = usr_evt;
    }

 
  else if ( option == 1)
    {
      event_cnt++;
      if ( event_cnt<0 ) 
	{
	  Printf("You reach the first event");
	  fill_event(N_evts)
	    ;	  event_cnt = N_evts;
	}
      
      else if ( event_cnt>N_evts ) 
	{
	  Printf("You reach the last event");
	  fill_event(0);
	  event_cnt = 0;
	}
      else 
	{ 
	  fill_event(event_cnt);
	}
    }

  else if ( option == -1)
    {
      event_cnt--;
      if ( event_cnt<0 ) 
	{
	  Printf("You reach the first event");
	  fill_event(N_evts);
	  event_cnt = N_evts;
	}
      
      else if ( event_cnt>N_evts ) 
	{
	  Printf("You reach the last event");
	  fill_event(0);
	  event_cnt = 0;
	}
      else 
	{ 
	  fill_event(event_cnt);
	}
    }

   
}



void fill_event(Int_t Event)
{
  cout<<"Event # = "<<Event<<endl;  

  nHits = 0;
  
  if ( c )
    {
      c -> Clear();
    }
  else 
    {
      c = new TCanvas("c");
    }
  
  c->Range(-30,-40, N_COL*4 +10, N_ROW*2.8+10);

  event_display();
  
  eventtree->GetEntry(Event);
  Int_t numHits = eventtree->GetLeaf("HitNum")->GetValue();
  cout << "numHits = " << numHits << endl;
  
  
  for(int j=0; j<numHits; j++)
    {  
     
      pad = eventtree->GetLeaf("StepID") ->GetValue(j); //CHANGE NUMBER OF VARIABLE ACCORDING YOUR ROOT DATA

      if(pad>0&&pad<20000)//THIS IS NECESSARY SINCE THERE ARE PADs NUMBERS OUT OF RANGE
	{
	  Int_t d = pad;	
	  
	  PAD[d]->SetFillStyle(1001);	
	  PAD[d]->SetFillColor(2);
	  PAD[d]->Draw("same");
	}
    }

  c->SetTitle(Form("Event %d",Event));


}

//Global variable (datafile number ONLY IF COMMAND INPUT IS USED)
string filenumber;

void BONuS_Event_Display() 
{

  // command line input.
  // cout<<"File number to analyze: ";
  // cin>>filenumber;
  
  
  //Create GUI
  bar = new TControlBar("vertical", "Event Displayer");
  bar->AddButton("Event Displayer", "");
  bar->AddButton("", "");
  bar->AddButton("First", "show(-2)");
  bar->AddButton("Next", "show(1)");
  bar->AddButton("Prev", "show(-1)");
  bar->AddButton("Last", "show(-3)");
  bar->AddButton("", "");
  bar->AddButton("Event #", "show(0)");
  bar->AddButton("", "");
  bar->AddButton("Exit",".q");
  bar->Show();
 
}
