#pragma once

#include <vector>
#include <algorithm>
#include <Arduino.h>
#include "Const.h"

using namespace std;

class IMenu
{ 
public:
   	IMenu* _prev;
    String _name_str;
    int    _colum;

    virtual bool is_branch();
    virtual void add_elem(IMenu* component) = 0;
    virtual String return_json_menu()       = 0;
    virtual ~IMenu();
};

class Branch: public IMenu
{
public:
    Branch(String name_str, int colum);

	vector<IMenu*> parent_vec;

    bool is_branch();
    void add_elem(IMenu* component); 
    String return_json_menu();
  
    virtual ~Branch();
};

class Parent: public IMenu
{
public:
    Parent(String name_str, int colum);

    void add_elem(IMenu* component);
    String return_json_menu();

    virtual ~Parent();
};

class KeyboardTelegram
{
public:
    KeyboardTelegram();

    vector<IMenu*> all_paret;
    IMenu* menu;
    IMenu* prev_menu;
    String prev_json_menu;
  
    void init_keyboard();
	void add_paret_menu(IMenu* branch, IMenu* paret);
    IMenu* find_paret(const String& command);
	String pointer_paret(IMenu* paret, const String& change_name = "");
    String view_json_keyboard(const String& command, const String& change_name = ""); 

    ~KeyboardTelegram();
};
