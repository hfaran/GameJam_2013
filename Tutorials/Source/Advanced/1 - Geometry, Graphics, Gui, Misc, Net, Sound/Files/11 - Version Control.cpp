/******************************************************************************/
#include "stdafx.h"
/******************************************************************************

   Version Control is a system allowing to store files,
      with the ability to restore a previously overwritten file version.

   In this tutorial we'll write a sample file, shortly after it, we'll overwrite the file with another data,
      and then we'll access both file versions (the most recent, and the previously overwritten).

/******************************************************************************/
VersionControl VC; // Version Control System

Str      latest_string ; // helper string  used for reading data from the latest version  of a sample file
Memb<Str>   all_strings; // helper strings used for reading data from all        versions of a sample file
/******************************************************************************/
void InitPre()
{
   App.name("Version Control");
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;
   DefaultTextStyle.scale =0.07;

   // initialize version control system to a specified path
   VC.init("LocalData/VersionControl");

   // store a simple file into the Version Control
   {
      File f; f.writeMem();      // open the file for writing in memory
      f.putStr("First Version"); // write a sample string
      f.pos   (              0); // reset the file position to the start

      DateTime modification_time_utc; modification_time_utc.getUTC(); // set file modification time from the current time

      VC.put("sample file",f,false,modification_time_utc,"Bruce Lee"); // store the source 'f' file as "sample file" in the version control system using 'modification_time_utc' and specyfing a user name which is performing the operation
   }

   // overwrite the old file with new data
   {
      File f; f.writeMem();
      f.putStr("Second Version"); // this time store a different string
      f.pos   (               0);

      DateTime modification_time_utc; modification_time_utc.getUTC();

      VC.put("sample file", f, false, modification_time_utc, "Super Man"); // store the new data to the same file as previously in order to overwrite it, for the testing we'll use a different user name performing the operation
   }

   // now the first file version has been overwritten with a newer one
   // so let's perform some test operations on accessing the created files

   // access the latest file version
   {
      File                      f; f.writeMem();                                    // open the file for writing in memory
      Bool             compressed;                                                  // this will hold the information wheter the file is compressed and would need to be decopressed using 'Decompress', but in the above codes we're not using compression so we can skip it
      DateTime operation_time_utc,                                                  // this will hold the operation    time of the latest file version
                  modify_time_utc;                                                  // this will hold the modification time of the latest file version
      if(VC.get("sample file", f, compressed, operation_time_utc, modify_time_utc)) // access the latest file version
      {
         // if the data has been accessed properly
         f.pos(0);                           // reset the file position
         latest_string=S+'"'+f.getStr()+'"'; // read sample data and surround it by quotes
      }
   }

   // access all file versions
   {
      Memc<VersionControl::History>    history;  // declare a memory container which will hold the file version history information
      if(VC.historyFile("sample file", history)) // access "sample file" file version history
      {
         FREPA(history) // iterate through all previous file versions (they are sorted from the newest to the oldest)
         {
            if(history[i].removed()) // if i-th operation meant removing the file then it has no data
            {
               // so lets just add an info string about the fact that the file has been removed
               all_strings.New()="REMOVED";
            }
            else // if not, then i-th operation was a version update
            {
               // let's read the data from i-th file version
               Bool          compressed;
               DateTime modify_time_utc;
               File     f; f.writeMem();
               if(VC.getVersion("sample file", f, compressed, history[i].operation_time_utc, modify_time_utc)) // here use 'getVersion' method (instead of 'get' like before) to manually specify which version we'd like to access
               {
                  // if successfully opened
                  f.pos(0); // reset the file position
                  all_strings.New()=S+'"'+f.getStr()+"\", Modified by: \""+history[i].user_name+"\", Operation Time: "+history[i].operation_time_utc.asText(true); // read the data of the i-th file version, and add info about the user which performed the operation and time of modification
               }
               else // if there has been an error while opening the previous version
               {
                  all_strings.New()=S+"ERROR Opening version: "+history[i].operation_time_utc.asText(true);
               }
            }
         }
      }
   }

   return true;
}
/******************************************************************************/
void Shut()
{
}
/******************************************************************************/
Bool Update()
{
   if(Kb.bp(KB_ESC))return false;
   return true;
}
/******************************************************************************/
void Draw()
{
   D.clear(WHITE);

   // display loaded strings
   D.text(0, 0.9f, "Latest String:");
   D.text(0, 0.8f,  latest_string  );

   D.text(0, 0.6f, "All Strings:");
   FREPA(all_strings)D.text(0, 0.5f-i*0.1f, all_strings[i]);
}
/******************************************************************************/
