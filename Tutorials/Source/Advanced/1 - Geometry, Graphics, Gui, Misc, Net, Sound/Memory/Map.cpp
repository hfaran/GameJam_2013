/******************************************************************************/
#include "stdafx.h"
/******************************************************************************

   'Map' is a container which stores elements of unique 'Keys' and custom 'Data'
      (there cannot be 2 elements in the same Map which have the same key)

   'Key' and 'Data' can be any kind of type.

   'Map' just like 'Memx' and 'Cache' containers preserve elements memory address on all operations.

   Accessing elements by 'Key' is fast because it's based on binary search.

/******************************************************************************/
struct Key
{
   Int key;

   Key(       ) {T.key=0  ;}
   Key(Int key) {T.key=key;}
};

struct Data
{
   Str name;
   Int parameter;
};

static Bool Create(Data &data, Key &key, Ptr user) // create element from 'key'
{
   data.name     =S+"Name made from key:"+key.key;
   data.parameter=Random(100); // some random value
   return true; // element has been created successfully, so return true
}

static Int Compare(C Key &a, C Key &b) // compare 2 keys
{
   if(a.key<b.key)return -1;
   if(a.key>b.key)return +1;
                  return  0;
}

Map<Key, Data> map(Create, Compare, NULL);
/******************************************************************************/
void InitPre()
{
   App.name("Map");
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;

   Str name_of_key_12=map(Key(12))->name; // this will access the element of 'key=12', because this element was not yet created, 'Create' function will be called automatically

   map(Key(1)); // access element of 'key=1'

   map(Key(12)); // again access element of 'key=12', the element was already created earlier, so 'Create' will not be called, but only pointer returned

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
   D.text (0, 0, S+"Number of elements in map: "+map.elms());
}
/******************************************************************************/
