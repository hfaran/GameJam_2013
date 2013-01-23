/******************************************************************************/
#include "stdafx.h"
/******************************************************************************

   This tutorial presents the basics of patching game files.

   First, you need to upload all your files to the server using "Uploader" tool.

   Once files are available, you need to use 'Patcher' class,
      which will help you perform the patching:
         -remove   obsolete files
         -download updated  files

   For test purposes we'll use files that have been uploaded on Esenthel server,
      using "Uploader" tool with following options:
         Http Directory: "http://www.esenthel.com/download/Patcher"
         Upload Name   : "Test"
         Secure        :  NULL

/******************************************************************************/
Patcher                  patcher; // define patcher object
Str                      local_dir="LocalData/Patch"; // define where we'll store data files
Memc<Patcher::LocalFile> local_files; // list of files currently present in the data folder
Bool                     compared=false; // helper variable used to specify if we have compared local with server files
/******************************************************************************/
void InitPre()
{
   App.name("Patching Files");
   Paks.add("../data/engine.pak");
}
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;

   FCreateDirs(local_dir); // make sure our data folder exists
   Patcher::SetList(local_files, local_dir); // automatically generate list of files present in that folder

   patcher.create("http://www.esenthel.com/download/Patcher", "Test", NULL); // initialize the patcher, those parameters must exactly match those specified in the "Uploader" tool (!! strings are case-sensitive in this case !!)
   patcher.downloadIndex(); // request files index to be downloaded

   return true;
}
/******************************************************************************/
void Shut()
{
}
/******************************************************************************/
Bool Update()
{
   if(Kb.bp(KB_ESC  ))return false;
   if(Kb.bp(KB_SPACE))OSLaunch(local_dir); // preview data folder when space pressed

   // check if the index was downloaded
   if(C Pak *pak=patcher.index()) // if index is available
   {
      if(!compared) // if we haven't compared the files yet
      {
         compared=true; // set compared, so we won't process this again next frame

         // compare files
         Memc<Int> local_remove  , // list of indexes in 'local_files' container that should be removed
                  server_download; // list of indexes in 'pak->file'   container that should be downloaded
         patcher.compare(local_files, local_remove, server_download); // compare information about local files with server files, and get list of which files should be removed, and which should be downloaded

         // remove files
         REPA(local_remove)
         {
            Patcher::LocalFile &lf       =local_files[local_remove[i]];
            Str                 full_name=local_dir.tailSlash(true) + lf.full_name;
            switch(lf.type)
            {
               case Patcher::LocalFile::STD_DIR : FDelDirs(full_name); break; // if it's a folder then remove using 'FDelDirs'
               case Patcher::LocalFile::STD_FILE: FDel    (full_name); break; // if it's a folder then remove using 'FDel'
            }
         }

         // download files
         REPA(server_download)patcher.downloadFile(server_download[i]); // request the files to be downloaded
      }
   }

   // process all files that have been downloaded
   Patcher::Downloaded downloaded;
   if(patcher.getNextDownload(downloaded)) // if there is a file downloaded
   {
      if(downloaded.success)
      {
         // file was downloaded successfully
         Str full_name=local_dir.tailSlash(true) + downloaded.full_name;
         if(downloaded.dir)
         {
            // if it's a directory then just create it
            FCreateDirs(full_name);
         }else
         {
            // if it's a file then create directory to its path
            FCreateDirs(GetPath(full_name));

            // and store file data
            File f;
            if(f.writeTry(full_name))
               if(downloaded.data.copy(f))
            {
               f.del(); // release the file handle
               FSetTimeUTC(full_name, downloaded.modify_time_utc); // set the file modification time so it matches the one from the server
            }
         }
      }else
      {
         // file wasn't properly downloaded, probable causes:
         //    -we got disconnected from the server
         //    -file was removed or was of different version than expected (this can happen if during our download, there was a newer version uploaded)
         // what we can do:
         //    -try downloading the file again - call 'downloadFile'  again (can help if we got disconnected, but won't help if the server has a newer version of the file)
         //    -redownload the index           - call 'downloadIndex' again, which should have latest information about all updates files
      }
   }

   return true;
}
/******************************************************************************/
void Draw()
{
   D.clear(WHITE);
   
   Str index_state;
   switch(patcher.indexState())
   {
      case DWNL_NONE    : index_state="None"                  ; break;
      case DWNL_WAIT    : index_state="Waiting for connection"; break;
      case DWNL_DOWNLOAD: index_state="Downloading"           ; break;
      case DWNL_DONE    : index_state="Finished"              ; break;
      case DWNL_ERROR   : index_state="Error encountered"     ; break;
   }

   D.text(0, 0.9f, S+"Server Index Download State: "+index_state);
   D.text(0, 0.8f, S+"Local Files Before Patching: "+local_files.elms());
   if(C Pak *pak=patcher.index())
   {
      D.text(0, 0.7f, S+"Server Files: "+pak->files());
   }
   D.text(0, 0.6f, S+"Files Left To Download: "+patcher.filesLeft());
   D.text(0, 0.5f, S+"Files Download Progress: "+patcher.progress()+" / "+patcher.filesSize());
   D.text(0, 0   , S+"Press Space to preview local data folder");
}
/******************************************************************************/
