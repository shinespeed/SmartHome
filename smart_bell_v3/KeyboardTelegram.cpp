#include "KeyboardTelegram.h"

IMenu::~IMenu()
{}

bool IMenu::is_branch()
{
    return false;
}

Branch::Branch(String name_str, int colum)
{
    this->_name_str = name_str;
    this->_colum = colum;
    this->_prev = this;
}

bool Branch::is_branch()
{
    return true;
}

void Branch::add_elem(IMenu* component)
{
    component->_prev = this;
    parent_vec.push_back(component);
}

String Parent::return_json_menu() 
{
    return this->_name_str;
}

void Parent::add_elem(IMenu* component) {}

String Branch::return_json_menu()
{
    vector<String> str_vec;
    sort(parent_vec.begin(), parent_vec.end(), [](IMenu* s1, IMenu* s2) {
      return  s1->_colum < s2->_colum;
    });

    String buff_str = "[";
    int a = 0;
    String bufa;
    str_vec.push_back(bufa);
    for (int i = 0; i < parent_vec.size()-1; i++) 
    {
        if (parent_vec[i]->_colum == parent_vec[i + 1]->_colum) 
        {
            str_vec[a] += "\"";
            str_vec[a] += parent_vec[i]->_name_str;
            str_vec[a] += "\", ";
        }
        else 
        {
            str_vec[a] += "\"";
            str_vec[a] += parent_vec[i]->_name_str;
            str_vec[a] += "\"],";
            a++;
            String bufa;
            str_vec.push_back(bufa);
        }
    }
    str_vec[a] += "\"";
    str_vec[a] += parent_vec[parent_vec.size()-1]->_name_str;
    str_vec[a] += "\"],";

    for (String str : str_vec) 
    {
        buff_str += "[";
        buff_str += str;
    }
  
    if(this->_prev == this)
    {
        buff_str[buff_str.length() - 1] = ']';
    }
    else
    {
        buff_str+="[\"/back\"]]";
    }
    return buff_str;
}

Branch::~Branch()
{
    for (IMenu* paret: parent_vec)
    {
        if (paret != nullptr)
        {
            delete paret;
            paret = nullptr;
        }
    }
}

Parent::Parent(String name_str, int colum)
{
    this->_name_str = name_str;
    this->_colum = colum;
}

Parent::~Parent(){}

KeyboardTelegram::KeyboardTelegram()
{
    menu = new Branch("/start", 1);
    menu->_prev = menu;
    prev_menu = menu;
    all_paret.push_back(menu);

    init_keyboard();
}

void KeyboardTelegram::init_keyboard()
{   
    if (status_bell) 
	    add_paret_menu(menu, new Parent("/off_bell", 1));
    else
	    add_paret_menu(menu, new Parent("/on_bell",  1));
  

	add_paret_menu(menu, new Parent("/weather", 2));

	IMenu* smart_room = new Branch("/smart_room", 2);
	add_paret_menu(menu, smart_room);

	add_paret_menu(smart_room, new Parent("/on_lamp", 1));
	add_paret_menu(smart_room, new Parent("/off_lamp", 1));
	add_paret_menu(smart_room, new Parent("/up_roller", 2));
	add_paret_menu(smart_room, new Parent("/down_roller", 2));

    add_paret_menu(menu, new Parent("/status", 3)); 
  
    IMenu* setting = new Branch("/setting", 3);
    add_paret_menu(menu, setting);
  
    add_paret_menu(setting, new Parent("/dir",             1));
    add_paret_menu(setting, new Parent("/write_spiffs",    1));
    add_paret_menu(setting, new Parent("/update_firmware", 2));
    add_paret_menu(setting, new Parent("/update_spiffs",   2));
    add_paret_menu(setting, new Parent("/setting_call",     3));
    add_paret_menu(setting, new Parent("/format",          3));

    IMenu* calling = new Branch("/calling", 4);
    add_paret_menu(menu, calling);
  
    IMenu* telegraph =      new Branch("/telegraph", 1);
    add_paret_menu(calling, new Parent("/call",      1));
    add_paret_menu(calling, telegraph);

    add_paret_menu(telegraph, new Parent("/stop_telegraph", 1));

    IMenu* mute_timer = new Branch("/mute_timer", 4);
    add_paret_menu(menu, mute_timer);
  
    add_paret_menu(mute_timer, new Parent("/on_timer_mute",   1));
    add_paret_menu(mute_timer, new Parent("/begin_time_mute", 2));
    add_paret_menu(mute_timer, new Parent("/end_time_mute",   2));  
}

IMenu* KeyboardTelegram::find_paret(const String& command)
{
    for (IMenu* paret: all_paret)
    {
        if (paret->_name_str == command) 
        {
            return paret;
        }
    }
    return NULL;
}

void KeyboardTelegram::add_paret_menu(IMenu* branch, IMenu* paret)
{
    all_paret.push_back(paret);
    branch->add_elem(paret);  
}

String KeyboardTelegram::pointer_paret(IMenu* paret, const String& change_name)
{
	if (paret != nullptr)
	{
		if ((paret->is_branch()))
		{
			prev_menu = paret;
		}
		else 
		{
			prev_menu = paret->_prev;
		}
		if (change_name.length() != 0)
		{
			paret->_name_str = change_name;
		}
		String keyboard_json;
		if (paret->is_branch())
		{
			keyboard_json = paret->return_json_menu();
		}
		else
		{
			keyboard_json = paret->_prev->return_json_menu();
		}
		prev_json_menu = keyboard_json;
		return keyboard_json;
	}
	else
	{
		return prev_json_menu;
	}
}

String KeyboardTelegram::view_json_keyboard(const String& command, const String& change_name)
{
    if (command != "/back")
    {
        IMenu* paret = find_paret(command);
		return pointer_paret(paret, change_name);
    }
    else
    {
		prev_menu = prev_menu->_prev;
        return pointer_paret(prev_menu, change_name);
    }
}

KeyboardTelegram::~KeyboardTelegram()
{
    if (menu != nullptr)
    {
        delete menu; 
    }
}
